from wrapper import Wrapper
import consts	 
import err

from checklib import *

import secrets
from telnetlib import Telnet
import socket
import time
import random


FRONT_PORT = 1338
BACKEND_PORT = 1337

main_menu = [
	b'View profile', b'Add info',
	b'Save report', b'View report',
	b'Search user', b'View last reports',
	b'Exit'
]

table = [ '#', "Name", "Type", "Owner" ]


report_template = '''XXXXXX REPORT #%d XXXXXX
Report owner: %s
Report time: %s
Report type: %s
Report object: Electro-station #%d
Report sign: %s
XXXXXX REPORT END XXXXXX'''

private_report = '%s'

class bcolors:
    HEADER = b'\033[95m'
    OKBLUE = b'\033[94m'
    OKGREEN = b'\033[92m'
    WARNING = b'\033[93m'
    FAIL = b'\033[91m'
    ENDC = b'\033[0m'
    BOLD = b'\033[1m'
    UNDERLINE = b'\033[4m'

all_colors = [
    	bcolors.HEADER,
    	bcolors.OKBLUE,
    	bcolors.OKGREEN,
    	bcolors.WARNING,
    	bcolors.FAIL,
    	bcolors.ENDC,
    	bcolors.BOLD,
    	bcolors.UNDERLINE
]
    

def recv_all( sock ):
	sock.settimeout( 0.35 )
	data = b''

	while True:
		try:
			data += sock.recv( 1024 * 16 )
		except:
			break
		
	for color in all_colors:
		data = data.replace( color, b'' )

	return data


class ECSFrontend:

	def __init__( self, host, port ):
		
		self.host = host
		self.port = port 

	def check_menu( self, sock, username ):

		# search username
		sock.write( b"5\n" ) 
		data = sock.read_until( b': ' + bcolors.ENDC, 
			timeout=2 )

		# send username
		sock.write( username[0:5].encode( 'utf-8' ) + b'\n' )
		data = sock.read_until( b'> ' + bcolors.ENDC, 
			timeout=2 ).decode( 'utf-8' )

		if username not in data:
			cquit( Status.MUMBLE, "Frontend: Could not find user!" )

		sock.write( b"5\n" )
		data = sock.read_until( b': ' + bcolors.ENDC, 
			timeout=2 )

		rnd_status = secrets.choice( consts.STATUS_LIST ).encode( 'utf-8' )

		sock.write( rnd_status + b'\n' )
		data = sock.read_until( b'> ' + bcolors.ENDC, 
			timeout=2 )

		if len( data ) == 0:
			cquit( Status.MUMBLE, "Search user is broken!" )

		# send view last reports
		sock.write( b"6\n" )
		data = sock.read_until( b"> " + bcolors.ENDC,
			timeout=2 ).decode( 'utf-8' )

		for i in table:
			if i not in data:
				cquit( Status.MUMBLE, "Table header is changed!" )

		# exit 
		sock.write( b"7\n" )
		data = sock.read_until( b"> " + bcolors.ENDC,
			timeout=2 )
		sock.write( b"5\n" )
		sock.close()

		return None

	def login( self, username, a_token ):
		try:
			sock = Telnet( self.host, self.port )
		except:
			cquit( Status.DOWN, "Frontend connection error!" )

		data = sock.read_until( b'> ' + bcolors.ENDC, timeout=2 )

		if len( data ) == 0:
			cquit( Status.MUMBLE, "Frontend menu is broken!" )
	
		sock.write( b"1\n" ) # send login cmd

		# send user name
		data = sock.read_until( b': ' + bcolors.ENDC,
			timeout=2 )
		sock.write( username.encode( 'utf-8' ) + b'\n' )


		# send access token
		data = sock.read_until( b': ' + bcolors.ENDC, 
			timeout=2 )
		sock.write( a_token.encode( 'utf-8' ) + b'\n' )


		# get login result 
		data = sock.read_until( b'> ' + bcolors.ENDC,
			timeout=2 )


		if b"[+] Login is successful!" not in data:
			cquit( Status.MUMBLE, "Could not login!" )

		for i in main_menu:
			if i not in data:
				cquit( Status.MUMBLE, "User menu is broken!" )

		return sock

	def view_last_users( self, username='' ):
		
		try:
			sock = Telnet( self.host, self.port )
		except:
			cquit( Status.DOWN, "Frontend connection error!" )

		data = sock.read_until( b'> ' + bcolors.ENDC, timeout=2 )

		if len( data ) == 0:
			cquit( Status.MUMBLE, "Frontend menu is broken!" )

		# send view_last_regs
		sock.write( b"3\n" ) 

		data = sock.read_until( b'> ' + bcolors.ENDC, timeout=2 )
		
		sock.write( b"5\n" )
		sock.close()

		if username != '':
			if username not in data.decode( 'utf-8' ):
				cquit( Status.MUMBLE, "View last regs is broken!" )

		if len( data ) == 0:
			cquit( Status.MUMBLE, "View last regs is broken!" )

		return 1

	def register_in_service( self ):		
		
		status = secrets.choice( consts.STATUS_LIST )
		login = rnd_username()
	
		data = b''

		try:
			sock = Telnet( self.host, self.port )
		except:
			cquit( Status.DOWN, "Frontend connection error!" )


		data = sock.read_until( b'> ' + bcolors.ENDC, timeout=2 )
		
		if len( data ) == 0:
			cquit( Status.MUMBLE, "Frontend menu is broken!" )


		# send registration cmd
		sock.write( b"2\n" ) 
		sock.read_until( b': ' + bcolors.ENDC, 
			timeout=2 )


		# send user name
		sock.write( login.encode( 'utf-8' ) + b'\n' )
		sock.read_until( b': ' + bcolors.ENDC, 
			timeout=2 )


		# send user status 
		sock.write( status.encode( 'utf-8' ) + b'\n' )
		data = sock.read_until( b'> ' + bcolors.ENDC, 
			timeout=2 )


		# check registration is successful
		if b"[+] Registration is successful!" not in data:
			cquit( Status.MUMBLE, "Could not register" )

		# get acces_token from recv data
		access_token = data.decode( 'utf-8' ).split( '\n' )[ 2 ].split( ": " )[ 1 ]

		user_info = { 
			'username' : login,
			'access_token' : access_token
		}

		sock.write( b"5\n" )
		sock.close()

		return user_info

	def change_info( self, sock, info ):

		sock.write( b"2\n" ) # set info
		data = sock.read_until( b': ' + bcolors.ENDC,
			timeout=2 )
		sock.write( info.encode( 'utf-8' ) + b'\n' )

		data = sock.read_until( b'> ' + bcolors.ENDC,
			timeout=2 )
		
		sock.write( b"1\n" ) # view info 
		data = sock.read_until( b'> ' + bcolors.ENDC,
			timeout = 2 ).decode( 'utf-8' )

		if info not in data:
			cquit( Status.MUMBLE, "Cloud not put flag" )

		sock.write( b"7\n" )
		sock.read_until( b'> ' + bcolors.ENDC, 
			timeout = 2 )
		sock.write( b"5\n" )

		sock.close()

	def view_info( self, sock ):
		sock.write( b"1\n" )

		data = sock.read_until( b"> " + bcolors.ENDC,
			timeout=2 ).decode( 'utf-8' )

		return data

	def check_info_flag( self, sock, flag ):

		data = self.view_info( sock )
		#print( "data: %s" % data )
		
		if flag not in data:
			cquit( Status.CORRUPT, "Could not get flag" )

	def exit( self, sock, is_login=True ):
		if is_login:
			sock.write( b"7\n" )
			sock.read_until( b"> " + bcolors.ENDC )

			sock.write( b"5\n" )
		else:
			sock.write( b"5\n" )

		sock.close() 


class ECSBackend:

	def __init__( self, host, port ):

		self.host = host 
		self.port = port

	def get_private_report( self, username, a_token, report_name, report_pass, flag='' ):
		
		wrapper = Wrapper( self.host, self.port )

		if not wrapper.check_connection():
			cquit( Status.DOWN, "Backend connection error!" )

		Report = { 
			'Name' : report_name,
			'Password' : report_pass,
			'Type' : 'private'
		}

		result, errno = wrapper.view_report( username, a_token, Report )

		if errno == err.VIEW_REPORT_SUCCESS:
			if flag != '':
			 	if flag not in result[ 'Data' ]:
			 		cquit( Status.CORRUPT, "Flag in report is not valid!" )

		else:
			if flag == '':
				cquit( Status.MUMBLE, "Could not get private report!" )
			else:
				cquit( Status.CORRUPT, "Could not get private report!" )


	def get_public_report( self, username, a_token, report_name, flag='' ):

		wrapper = Wrapper( self.host, self.port )

		if not wrapper.check_connection():
			cquit( Status.DOWN, "Backend connection error!" )

		Report = { 
			'Name' : report_name,
			'Type' : 'public'
		}

		result, errno = wrapper.view_report( username, a_token, Report )

		if errno == err.VIEW_REPORT_SUCCESS:
			if flag != '':
			 	if flag not in result[ 'Data' ]:
			 		cquit( Status.CORRUPT, "Flag in report is not valid!" )

		else:
			if flag == '':
				cquit( Status.MUMBLE, "Could not get public report!" )
			else:
				cquit( Status.CORRUPT, "Could not get public report!" )
			
	def save_private_report( self, username, a_token, report_name, report_pass, flag ):

		wrapper = Wrapper( self.host, self.port )

		if not wrapper.check_connection():
			cquit( Status.DOWN, "Backend connection error!" )

		report_data = report_template % ( 
			random.randint( 100, 1000 ),
			username,
			str( round( time.time() ) ),
			'private',
			random.randint( 100, 1000 ),
			flag
		)

		Report = {
			'Name' : report_name,
			'Type' : 'private',
			'Password' : report_pass,
			'Data' : report_data
		}

		result, errno = wrapper.save_report( username, a_token, Report )

		if errno == err.REPORT_NOT_SAVED:
			cquit( Status.MUMBLE, "Could not save private report!" )

		if errno != err.SAVE_REPORT_SUCCESS:
			cquit( Status.MUMBLE, "Could not save private report!" )


	def save_public_report( self, username, a_token, report_name, flag ):

		wrapper = Wrapper( self.host, self.port )

		if not wrapper.check_connection():
			cquit( Status.DOWN, "Backend connection error!" )

		report_data = report_template % ( 
			random.randint( 100, 1000 ),
			username,
			str( round( time.time() ) ),
			'public',
			random.randint( 100, 1000 ),
			flag
		)

		Report = {
			'Name' : report_name,
			'Type' : 'public',
			'Data' : report_data
		}

		result, errno = wrapper.save_report( username, a_token, Report )

		if errno == err.REPORT_NOT_SAVED:
			cquit( Status.MUMBLE, "Could not save public report!" )

		if errno != err.SAVE_REPORT_SUCCESS:
			print_f( Status.MUMBLE, "Could not save public report!" )


	def login( self, username, a_token ):

		wrapper = Wrapper( self.host, self.port )

		if not wrapper.check_connection():
			cquit( Status.DOWN, "Backend connection error!" )

		result, errno = wrapper.login( username, a_token )

		posible_errors = [ err.CONNECTION_ERROR, err.SEND_ERROR, 
			err.SERVER_RESPONSE_EMPTY_ERROR,
			err.SERVER_ERROR ]
		
		if errno in posible_errors:
			cquit( Status.MUMBLE, "Could not login to backend!" )

	def search_user( self, username, a_token ):

		wrapper = Wrapper( self.host, self.port )

		if not wrapper.check_connection():
			cquit( Status.DOWN, "Backend connection error!" )

		result, errno = wrapper.search_user( 
			username, 
			a_token, 
			username[0:5] 
		)

		if errno != err.SEARCH_NAME_SUCCESS:
			cquit( Status.MUMBLE, "Backend: Could not find user!" )

	def view_last_reports( self, username, a_token ):
		
		wrapper = Wrapper( self.host, self.port )

		if not wrapper.check_connection():
			cquit( Status.DOWN, "Backend connection error!" )

		result, errno = wrapper.view_last_users()

		if errno == err.CONNECTION_ERROR:
			cquit( Status.DOWN, "Backend connection error!" )

		if errno == err.SEND_ERROR:
			cquit( Status.MUMBLE, "Could not send data to backend!" )

		if errno == err.SERVER_RESPONSE_EMPTY_ERROR:
			cquit( Status.MUMBLE, "Could not find reports!" )

		if len( result ) <= 0:
			cquit( Status.MUMBLE, "Could not find reports!" )

