#!/usr/bin/env python3

import sys
import os

import handlers
import err

from msg import *
from consts import *
from wrapper import Wrapper
from utils import *


g_Wrapper = Wrapper( '10.0.0.102', 1337 )

DEBUG = True
bad_chars = [ chr( i ) for i in range( 0, 48 ) ] + [ chr( i ) for i in range( 123, 256 ) ]

def check_token( Token ):
	if len( Token ) > MAX_TOKEN_SIZE:
		return False

	for i in Token:
		if i in bad_chars:
			return False

	return True

def check_username( Username ):
	if len( Username ) > MAX_USERNAME_SIZE:
		return False

	for i in Username:
		if i in bad_chars:
			return False

	return True

def check_status( Status ):
	if len( Status ) > MAX_STATUS_SIZE:
		return False

	for i in Status:
		if i in bad_chars:
			return False

	if Status not in STATUS_LIST:
		return False

	return True

def check_report_type( ReportType ):
	if len( ReportType ) > 16:
		return False

	if ReportType != 'private' and ReportType != 'public':
		return False

	return True


def check_report_name( ReportName ):
	if len( ReportName) <= 4 or len( ReportName ) > 32:
		return False

	for i in ReportName:
		if i in bad_chars:
			return False

	return True

def check_report_pass( Pass ):
	if len( Pass ) <= 4 or len( Pass ) > 16:
		return False

	for i in Pass:
		if i in bad_chars:
			return None

	return True

def check_searched_name( Name ):
	if len( Name ) <= 4 or len( Name ) > 32:
		return False

	for i in Name:
		if i in bad_chars:
			return False

	return True

def view_last_reports( Username, AccessToken ):
	global g_Wrapper

	result, errno = g_Wrapper.view_last_reports( 
		Username, 
		AccessToken
	)
	
	from prettytable import PrettyTable
	
	reports_table = PrettyTable( [ '#', 'Name', 'Owner', 'Type' ] )

	for i in range( len( result ) ):

		_id = i
		name = result[ i ][ 'Name' ]
		owner = result[ i ][ 'Owner' ]
		_type = result[ i ][ 'Type' ]

		rep = [ _id, name, owner, _type ]

		reports_table.add_row( rep )
	
	print( reports_table )

	return result, errno 

def search_user( Username, AccessToken ):
	global g_Wrapper

	SearchedName = input( ENTER_NAME_TO_SEARCH )

	if not check_searched_name( SearchedName ):
		print_errf( "[-] Bad username!" )
		return None, err.ERROR_NAME

	result, errno = g_Wrapper.search_user( 
		Username, 
		AccessToken, 
		SearchedName 
	)

	if errno == err.SEARCH_NAME_SUCCESS:
		print_f( "-- Result of search --" )

		for i in range( len( result ) ):
			print_f( "%d. %s (%s)" % ( 
				i,
				result[ i ][ 'Username' ].decode( 'utf-8' ),
				result[ i ][ 'Status' ].decode( 'utf-8' )
			) )

		print_f( "----------------------" )

	else:
		print_f( "-- Result of search --" )
		print_f( "[Empty]" )
		print_f( "----------------------" )

	return result, errno

def view_report( Username, AccessToken ):
	global g_Wrapper

	ReportName = input( ENTER_REPORT_NAME )

	if not check_report_name( ReportName ):
		print_f( WRONG_REPORT_NAME )
		return None, err.REPORT_NAME_ERROR

	ReportType = input( ENTER_REPORT_TYPE ).lower()

	if not check_report_type( ReportType ):
		print_f( WRONG_REPORT_TYPE )
		return None, err.REPORT_TYPE_ERROR

	Report = { 
		'Name' : ReportName,
		'Type' : ReportType
	}

	if ReportType == "private":
		ReportPassword = input( ENTER_REPORT_PASSWORD )		

		if check_report_pass( ReportPassword ):
			Report[ 'Password' ] = ReportPassword
		else:
			print_errf( WRONG_REPORT_PASSWORD )
			return None, err.REPORT_PASSWORD_ERROR

	result, errno = g_Wrapper.view_report( Username, AccessToken, Report )

	if errno == err.VIEW_REPORT_SUCCESS:
		print_f( "--- Report data ---" )
		print_f( result[ 'Data' ] )
		print_f( "-------------------" )

	else:
		print_f( ERROR_VIEW_REPORT )

	return result, errno 

def save_report( Username, AccessToken ):
	global g_Wrapper

	ReportName = input( ENTER_REPORT_NAME )

	if not check_report_name( ReportName ):
		print_f( WRONG_REPORT_NAME )
		return None, err.REPORT_NAME_ERROR

	ReportType = input( ENTER_REPORT_TYPE ).lower()

	if not check_report_type( ReportType ):
		print_f( WRONG_REPORT_TYPE )
		return None, err.REPORT_TYPE_ERROR

	ReportData = input( ENTER_REPORT_DATA )

	Report = { 
		'Name' : ReportName,
		'Type' : ReportType,
		'Data' : ReportData 
	}

	if ReportType == "private":
		ReportPassword = input( ENTER_REPORT_PASSWORD )
		
		if check_report_pass( ReportPassword ):
			Report[ 'Password' ] = ReportPassword
		else:
			print_errf( WRONG_REPORT_PASSWORD )
			return None, err.REPORT_PASSWORD_ERROR

	result, errno = g_Wrapper.save_report( Username, AccessToken, Report )

	if errno == err.SAVE_REPORT_SUCCESS:
		print_f( REPORT_SAVED )

	elif errno == err.REPORT_NOT_SAVED:
		print_f( MSG_REPORT_NOT_SAVED )

	return result, errno 

def change_info( Username ):
	fd = None

	try:
		fd = open( "info/" + Username + ".info", 'wb' )
	except:
		return None

	NewInfo = input( ENTER_NEW_INFO ).strip()

	if len( NewInfo ) > 0 and len( NewInfo ) < 1024:
		fd.write( NewInfo.encode( 'utf-8' ) )

	fd.close()

def load_info( Username ):
	fd = None

	try:
		fd = open( "info/" + Username + ".info", 'rb' )
	except:
		return None

	Info = fd.read()
	fd.close()

	return Info.decode( 'utf-8' )

def logined_user_menu( UserDesc ):
	
	Username    = UserDesc[ 'Username' ]
	AccessToken = UserDesc[ 'AccessToken' ]
	
	while 1:
		Info = load_info( Username )

		option = input( USER_MENU )

		try:
			option = int( option, 10 )
		except:
			option = -1

		if option == 1:
			print_f( "----- Profile -----" )
			print_f( "Username: %s" % Username )
			print_f( "AccessToken: %s" % AccessToken )
			print_f( "Info: %s" % Info )
			print_f( "-------------------" )

			continue 

		elif option == 2:
			change_info( Username )
			continue

		elif option == 3:
			result, errno = save_report( Username, AccessToken )
			continue

		elif option == 4:
			result, errno = view_report( Username, AccessToken )
			continue

		elif option == 5:
			result, errno = search_user( Username, AccessToken )
			continue 

		elif option == 6:
			result, errno = view_last_reports( Username, AccessToken )
			continue

		elif option == 7:
			print_f( NORMAL_EXIT )
			return True


def login():
	global g_Wrapper

	Username = input( ENTER_USERNAME )
	
	if not check_username( Username ):
		return None, err.USERNAME_ERORR

	AccessToken = input( ENTER_TOKEN )

	if not check_token( AccessToken ):
		return None, err.TOKEN_ERROR

	if not g_Wrapper.check_connection():
		print_errf( "[-] g_Wrapper is not connected to server!" )
		return None, err.SERVER_CONNECTION_ERROR

	result, errno = g_Wrapper.login( Username, AccessToken )

	posible_errors = [ err.CONNECTION_ERROR, err.SEND_ERROR, 
		err.SERVER_RESPONSE_EMPTY_ERROR,
		err.SERVER_ERROR ]
	
	if errno in posible_errors :
		return None, errno

	return result, errno

def view_last_users():
	global g_Wrapper

	if not g_Wrapper.check_connection():
		print_errf( "[-] g_Wrapper is not connected to server!" )
		return None, err.SERVER_CONNECTION_ERROR
	
	result, errno = g_Wrapper.view_last_users()

	if errno == err.CONNECTION_ERROR:
		return None, errno

	if errno == err.SEND_ERROR:
		return None, errno

	if errno == err.SERVER_RESPONSE_EMPTY_ERROR:
		return None, errno

	if len( result ) <= 0:
		return None, err.PARSING_ERROR

	for i in range( len( result ) ):
		print_f( "%d. %s (%s)" % ( 
			i,
			result[ i ][ 'Username' ].decode( 'utf-8' ),
			result[ i ][ 'Status' ].decode( 'utf-8' )
		) )

	return result, err.VIEW_LAST_USERS_SUCCESS

def registration():
	global g_Wrapper

	Username = input( ENTER_USERNAME )

	if not check_username( Username ):
		return None, err.USERNAME_ERORR

	Status = input( ENTER_STATUS )

	if not check_status( Status ):
		return None, err.STATUS_ERROR

	if not g_Wrapper.check_connection():
		print_errf( "[-] g_Wrapper is not connected to server!" )
		return None, err.REGISTRATION_SERVER_CONNECTION_ERROR

	result, errno = g_Wrapper.register( Username, Status )

	if errno in err.REGISTRATION_ERROR_LIST:
		return None, errno

	return result, err.REGISTRATION_SUCCESS

if __name__ == "__main__":

	if DEBUG:
		print_errf( "Debug run!" )
		# place debug code here

	else:
		print_errf( "[-] Use this file as a library, not a standalone script!!!" )