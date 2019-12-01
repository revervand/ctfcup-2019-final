import socket
import sys
import json
import binascii

from utils import *
from consts import *
import err

class ServerErrors:
	PACKET_SIZE_ERR = 0x1101

	ERR_CMD_ID = 0x1201

	ERR_REG_USERNAME_SIZE = 0x1301
	ERR_REG_STATUS_SIZE   = 0x1302
	ERR_REG_EMPTY_TOKEN   = 0x1303
	ERR_REG_INVALID_TOKEN_SIZE = 0x1304
	ERR_REG_INVALID_TOKEN_VALUE = 0x1305
	ERR_REG_ACCESS_TOKEN_GEN = 0x1306
	ERR_REG_USER_EXIST = 0x1307
	ERR_REG_ADD_USER_ERROR = 0x1308

	err_map = { 
		PACKET_SIZE_ERR : '[-] Packet size error!',
		ERR_CMD_ID : '[-] Error server command!',
		ERR_REG_USERNAME_SIZE : '[-] Error username size value!',
		ERR_REG_STATUS_SIZE : '[-] Error status size value!',
		ERR_REG_EMPTY_TOKEN : '[-] Token is empty!',
		ERR_REG_INVALID_TOKEN_SIZE : '[-] Invalid token size!',
		ERR_REG_INVALID_TOKEN_VALUE : '[-] Invalid token value!',
		ERR_REG_ACCESS_TOKEN_GEN : '[-] Error in access token generation!',
		ERR_REG_USER_EXIST : '[-] User with same name is exist!',
		ERR_REG_ADD_USER_ERROR : '[-] Error in user registration!'
	}

class ServerCommands:
	REGISTER 		  = p_16_le( 0x7E12 )
	LOGIN    		  = p_16_le( 0x7E13 )
	VIEW_LAST_USERS   = p_16_le( 0x7E14 )
	SAVE_REPORT 	  = p_16_le( 0x7E15 )
	VIEW_REPORT       = p_16_le( 0x7E16 )
	SEARCH_NAME 	  = p_16_le( 0x7E17 )
	VIEW_LAST_REPORTS = p_16_le( 0x7E18 )

	ERR_MSG = p_32_be( 0xdeadbeef )

class Wrapper:

	sock = None
	host = ''
	port = 0
	is_connected = False

	def __init__( self, inp_host, inp_port ):

		if type( inp_host ) != str or type( inp_port ) != int:
			print_errf( "[-] Wrapper init error!" )
			return None

		self.sock = socket.socket()

		if not self.sock:
			print_errf( "[-] Some internal error!" )
			sys.exit( err.UNDEF_INTERNAL_ERROR )

		self.sock.settimeout( CONNECTION_TIMEOUT )

		try:
			self.sock.connect( ( inp_host, inp_port ) )
		except:
			print_errf( "[-] Connection error!" )
			return None

		self.host = inp_host
		self.port = inp_port

		self.sock.settimeout( DEFAULT_NET_TIMEOUT )
		# bad trick
		self.is_connected = True
		
		self.sock.close()

	def view_last_reports( self, Username, AccessToken ):

		payload = ServerCommands.VIEW_LAST_REPORTS

		payload += p_8( len( Username ) )
		payload += Username.encode( 'utf-8' )

		payload += p_8( len( AccessToken ) )
		payload += AccessToken.encode( 'utf-8' )

		packet = p_32_le( len( payload ) ) + payload

		self.connect_to_server()

		if not self.check_connection():
			return None, err.CONNECTION_ERROR

		server_resp_data = ''

		if self.send_to_server( packet ):
			server_resp_data = self.recv_from_server()
		else:
			return None, err.SEND_ERROR

		if len( server_resp_data ) == 0:
			return None, err.SERVER_RESPONSE_EMPTY_ERROR

		parse_err = self.decode_server_resp( server_resp_data )
		parse_err = json.loads( parse_err )

		result = None

		if parse_err[ 'type' ] == 'error':
			return None, parse_err[ 'errcode' ]

		else:
			result = self.parse_last_reports( server_resp_data )

		self.close_connection()
		
		return result, err.VIEW_LAST_REPORTS_SUCCESS

	def search_user( self, Username, AccessToken, SearchedName ):
		
		payload = ServerCommands.SEARCH_NAME

		payload += p_8( len( Username ) )
		payload += Username.encode( 'utf-8' )

		payload += p_8( len( AccessToken ) )
		payload += AccessToken.encode( 'utf-8' )

		payload += p_8( len( SearchedName ) )
		payload += SearchedName.encode( 'utf-8' )

		packet = p_32_le( len( payload ) ) + payload

		self.connect_to_server()

		if not self.check_connection():
			return None, err.CONNECTION_ERROR

		server_resp_data = ''

		if self.send_to_server( packet ):
			server_resp_data = self.recv_from_server()
		else:
			return None, err.SEND_ERROR

		if len( server_resp_data ) == 0:
			return None, err.SERVER_RESPONSE_EMPTY_ERROR

		parse_err = self.decode_server_resp( server_resp_data )
		parse_err = json.loads( parse_err )

		if parse_err[ 'type' ] == 'error':
			return None, parse_err[ 'errcode' ]

		users_list = self.parse_user_list( server_resp_data )
		self.close_connection()

		return users_list, err.SEARCH_NAME_SUCCESS

	def view_report( self, Username, AccessToken, Report ):
		
		payload = ServerCommands.VIEW_REPORT

		payload += p_8( len( Username ) )
		payload += Username.encode( 'utf-8' )

		payload += p_8( len( AccessToken ) )
		payload += AccessToken.encode( 'utf-8' )

		payload += p_8( len( Report[ 'Name' ] ) )
		payload += Report[ 'Name' ].encode( 'utf-8' )

		payload += p_8( len( Report[ 'Type' ] ) )
		payload += Report[ 'Type' ].encode( 'utf-8' )

		if Report[ 'Type' ] == 'private':
			payload += p_8( len( Report[ 'Password' ] ) )
			payload += Report[ 'Password' ].encode( 'utf-8' )
			
		packet = p_32_le( len( payload ) ) + payload

		self.connect_to_server()

		if not self.check_connection():
			return None, err.CONNECTION_ERROR

		server_resp_data = ''

		if self.send_to_server( packet ):
			server_resp_data = self.recv_from_server()
		else:
			return None, err.SEND_ERROR

		if len( server_resp_data ) == 0:
			return None, err.SERVER_RESPONSE_EMPTY_ERROR

		server_answer = self.decode_server_resp( server_resp_data )

		_tmp = json.loads( server_answer )

		if _tmp[ 'type' ] == 'error':
			return None, err.SERVER_ERROR

		self.close_connection()

		return json.loads( server_answer ), err.VIEW_REPORT_SUCCESS

	def save_report( self, Username, AccessToken, Report ):

		payload = ServerCommands.SAVE_REPORT

		payload += p_8( len( Username ) )
		payload += Username.encode( 'utf-8' )

		payload += p_8( len( AccessToken ) )
		payload += AccessToken.encode( 'utf-8' )

		payload += p_8( len( Report[ 'Name' ] ) )
		payload += Report[ 'Name' ].encode( 'utf-8' )

		payload += p_8( len( Report[ 'Type' ] ) )
		payload += Report[ 'Type' ].encode( 'utf-8' )

		payload += p_8( len( Report[ 'Data' ] ) )
		payload += Report[ 'Data' ].encode( 'utf-8' )

		if Report[ 'Type' ] == 'private':
			payload += p_8( len( Report[ 'Password' ] ) )
			payload += Report[ 'Password' ].encode( 'utf-8' )

		packet = p_32_le( len( payload ) ) + payload

		self.connect_to_server()

		if not self.check_connection():
			return None, err.CONNECTION_ERROR

		server_resp_data = ''

		if self.send_to_server( packet ):
			server_resp_data = self.recv_from_server()
		else:
			return None, err.SEND_ERROR

		if len( server_resp_data ) == 0:
			return None, err.SERVER_RESPONSE_EMPTY_ERROR

		server_answer = self.decode_server_resp( server_resp_data )

		_tmp = json.loads( server_answer )

		if _tmp[ 'type' ] == 'error':
			return None, err.SERVER_ERROR

		self.close_connection()

		if _tmp[ 'saved' ]:
			return server_answer, err.SAVE_REPORT_SUCCESS
		else:
			return server_answer, err.REPORT_NOT_SAVED

	def login( self, Username, AccessToken ):

		payload = ServerCommands.LOGIN

		payload += p_8( len( Username ) )
		payload += Username.encode( 'utf-8' )

		payload += p_8( len( AccessToken ) )
		payload += AccessToken.encode( 'utf-8' )

		packet_size = len( payload )

		packet = p_32_le( packet_size ) + payload

		self.connect_to_server()

		if not self.check_connection():
			return None, err.CONNECTION_ERROR

		server_resp_data = ''

		if self.send_to_server( packet ):
			server_resp_data = self.recv_from_server()
		else:
			return None, err.SEND_ERROR

		if len( server_resp_data ) == 0:
			return None, err.SERVER_RESPONSE_EMPTY_ERROR

		server_answer = self.decode_server_resp( server_resp_data )

		_tmp = json.loads( server_answer )

		if _tmp[ 'type' ] == 'error':
			return None, err.SERVER_ERROR

		self.close_connection()

		return server_answer, err.LOGIN_SUCCESS

	def view_last_users( self ):
		payload = ServerCommands.VIEW_LAST_USERS

		payload += p_8( len( SERVER_CONNECTION_TOKEN ) )
		payload += SERVER_CONNECTION_TOKEN

		packet_size = len( payload )

		packet = p_32_le( packet_size ) + payload

		self.connect_to_server()

		if not self.check_connection():
			return None, err.CONNECTION_ERROR

		if self.send_to_server( packet ):
			server_resp_data = self.recv_from_server()
		else:
			return None, err.SEND_ERROR

		if len( server_resp_data ) == 0:
			return None, err.SERVER_RESPONSE_EMPTY_ERROR

		# server_answer = self.decode_server_resp( server_resp_data )
		users_list = self.parse_user_list( server_resp_data )
		self.close_connection()

		return users_list, err.VIEW_LAST_USERS_SUCCESS

	def parse_user_list( self, server_resp_data ):

		resp_off = 0
		total_users_count = u_32_le( server_resp_data[ resp_off : resp_off + 4 ] )
		resp_off += 4

		users_list = []

		for i in range( 0, total_users_count ):
			user_entry_size = u_32_le( server_resp_data[ resp_off : resp_off + 4 ] )
			resp_off += 4

			user_id = u_32_le( server_resp_data[ resp_off : resp_off + 4 ] )
			resp_off += 4 

			username_size =server_resp_data[ resp_off ]
			resp_off += 1

			username_value = server_resp_data[ resp_off : resp_off + username_size ]
			resp_off += username_size

			atoken_size = server_resp_data[ resp_off ]
			resp_off += 1

			atoken_value = server_resp_data[ resp_off : resp_off + atoken_size ]
			resp_off += atoken_size

			status_size = server_resp_data[ resp_off ]
			resp_off += 1

			status_value = server_resp_data[ resp_off : resp_off + status_size ]
			resp_off += status_size

			padding_filed = u_32_le( server_resp_data[ resp_off : resp_off + 4 ] )
			resp_off += 4

			if user_id != 1:
				user_list_entry = {
					'Id': user_id, 
					'Username' : username_value,
					'Status' : status_value
				}

				users_list.append( user_list_entry )

		return users_list
 
	def register( self, Username, Status ):
		
		payload = ServerCommands.REGISTER

		payload += p_8( len( Username ) )
		payload += Username.encode( 'utf-8' )

		payload += p_8( len( Status ) )
		payload += Status.encode( 'utf-8' )

		payload += p_8( len( SERVER_CONNECTION_TOKEN ) )
		payload += SERVER_CONNECTION_TOKEN

		packet_size = len( payload )

		packet = p_32_le( packet_size ) + payload

		self.connect_to_server()

		if not self.check_connection():
			return err.REGISTRATION_SERVER_CONNECTION_ERROR

		server_resp_data = ''

		if self.send_to_server( packet ):
			server_resp_data = self.recv_from_server()
		else:
			return None, err.REGISTRATION_SERVER_CONNECTION_ERROR

		if len( server_resp_data ) == 0:
			return None, err.REGISTRATION_SERVER_RESPONSE_EMPTY_ERROR

		server_answer = self.decode_server_resp( server_resp_data )

		# check if packet is error
		_tmp = json.loads( server_answer )

		if _tmp[ 'type' ] == 'error':
			return None, err.REGISTRATION_SERVER_ERROR

		self.close_connection()

		return server_answer, err.REGISTRATION_SUCCESS

	def send_to_server( self, buf ):

		if len( buf ) == 0:
			print_errf( "[-] Error buffer len!" )
			return None

		if not self.sock:
			print_errf( "[-] self.sock is not init!" )
			return None

		self.sock.settimeout( SEND_TIMEOUT )
		nbytes = 0 

		try:
			nbytes = self.sock.send( buf )
		except:
			print_errf( "[-] Send data timeout!" )
			return None

		if nbytes != len( buf ):
			print_errf( "[-] Send data error!" )
			return None

		return True

	def recv_from_server( self ):

		res = b''
		self.sock.settimeout( RECV_TIMEOUT )
		last_len = 0

		while 1:
			try:
				res += self.sock.recv( RECV_SIZE )

				if last_len == len( res ):
					break
		
				last_len = len( res )

			except socket.timeout:
				break

		self.sock.settimeout( DEFAULT_NET_TIMEOUT )

		return res

	def connect_to_server( self ):

		self.sock = socket.socket()

		if not self.sock:
			print_errf( "[-] Some internal error!" )
			sys.exit( err.UNDEF_INTERNAL_ERROR )

		self.sock.settimeout( CONNECTION_TIMEOUT )

		try:
			self.sock.connect( ( self.host, self.port ) )
		except:
			print_errf( "[-] Connection error!" )
			self.is_connected = False
			return err.CONNECTION_ERROR

		self.is_connected = True

	def close_connection( self ):
		try:
			self.sock.close()
		except:
			pass

	def check_connection( self ):
		return self.is_connected

	def check_error( self, errcode ):

		for i in ServerErrors.err_map.keys():
			if i == errcode:
				print_errf( ServerErrors.err_map[ i ] )
				break 

	def decode_server_resp( self, buf ):
		packet_offset = 0

		first_4_byte = buf[ 0 : 4 ]

		if first_4_byte == ServerCommands.ERR_MSG:
			errcode = u_16_le( buf[ 4 : 6 ] )

			res = self.check_error( errcode )

			parsed_packet = { 'type': 'error', 'errcode': str( errcode ) }

			return json.dumps( parsed_packet )


		packet_size = u_32_le( buf[ packet_offset : packet_offset + 4 ] )
		packet_offset += 4

		packet_cmd = buf[ packet_offset : packet_offset + 2 ]
		packet_offset += 2

		if packet_cmd == ServerCommands.VIEW_REPORT:
			RecordSize = buf[ packet_offset ]
			packet_offset += 1

			RecordData = buf[ packet_offset : packet_offset + RecordSize ]
			
			try:
				RecordData = RecordData.decode( 'utf-8' )
			except:
				RecordData = binascii.hexlify( RecordData ).decode( 'utf-8' )

			parsed_packet = { 'type' : 'view_rep',
				'Data' : RecordData
			}

			return json.dumps( parsed_packet )

		if packet_cmd == ServerCommands.SAVE_REPORT:
			UsernameSize = buf[ packet_offset ]
			packet_offset += 1

			Username = buf[ packet_offset : packet_offset + UsernameSize ]
			Username = Username.decode( 'utf-8 ' )
			packet_offset += UsernameSize

			saved = buf[ packet_offset ]

			parsed_packet = { 'type' : 'save_rep',
				'Username' : Username,
				'saved' : saved 
			}

			return json.dumps( parsed_packet )

		if packet_cmd == ServerCommands.LOGIN:
			UsernameSize = buf[ packet_offset ]
			packet_offset += 1 
			
			Username = buf[ packet_offset : packet_offset + UsernameSize ]
			Username = Username.decode( 'utf-8' )
			packet_offset += UsernameSize 

			AccessTokenSize = buf[ packet_offset ]
			packet_offset += 1

			AccessToken = buf[ packet_offset : packet_offset + AccessTokenSize ]
			AccessToken = AccessToken.decode( 'utf-8' )
			packet_offset += AccessTokenSize

			logined = buf[ packet_offset ]

			parsed_packet = { 'type':'login', 
				'Username' : Username, 
				'AccessToken' : AccessToken, 
				'logined' : logined 
			}

			return json.dumps( parsed_packet )

		if packet_cmd == ServerCommands.REGISTER:
			UsernameSize = buf[ packet_offset ]
			packet_offset += 1 

			Username = buf[ packet_offset : packet_offset + UsernameSize ]
			Username = Username.decode( 'utf-8' )
			packet_offset += UsernameSize 

			StatusSize = buf[ packet_offset ]
			packet_offset += 1 

			Status = buf[ packet_offset : packet_offset + StatusSize ] 
			Status = Status.decode( 'utf-8' )
			packet_offset += StatusSize

			AccessTokenSize = buf[ packet_offset ]
			packet_offset += 1

			AccessToken = buf[ packet_offset : packet_offset + AccessTokenSize ]
			AccessToken = AccessToken.decode( 'utf-8' )

			parsed_packet = { 'type':'reg', 
				'Username' : Username, 
				'Status' : Status, 
				'AccessToken' : AccessToken  
			}

			return json.dumps( parsed_packet )

		else:
			undef_packet = { 'type' : 'undefined' }
			return json.dumps( undef_packet )

	def parse_last_reports( self, buf ):

		correct_names = []

		for line in buf.split( b'\n' ):
			try:
				buf = line.split( b' ' )[ -1 ]
				if b"priv" in buf or b"pub" in buf:
					correct_names.append( buf ) 
			except:
				continue

		reports = []

		for rep in correct_names:

			report = {}

			if b"priv" in rep:
				report[ 'Type' ] = "Private"
			elif b"pub" in rep:
				report[ 'Type' ] = "Public"
			else:
				continue

			owner = rep.split( b'_' )[ 0 ]
			report[ 'Owner' ] = owner.decode( 'utf-8' )

			name = rep.split( b'_' )[ 1 ].split( b'.' )[ 0 ]
			report[ 'Name' ] = name.decode( 'utf-8' )

			reports.append( report )

		return reports
