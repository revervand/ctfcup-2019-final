#!/usr/bin/env python3

import sys
import os
import json

from msg import *
import handlers
import err
from consts import *
from utils import *

login_menu_options = [ 1, 2, 3, 4, 5 ]
login_attempts = 0
all_actions = 0

if __name__ == '__main__':
	
	init()
    
	print_f( BANNER )
    
	while 1:
		all_actions += 1 

		if all_actions > 1000:
			print_f( ERROR_TO_MANY_ACTIONS)
			sys.exit( err.TO_MANY_ACTIONS_ERROR )

		if login_attempts == 3:
			print_f( ERROR_LOGIN_ATTEMPTS )
			sys.exit( login_attempts )

		option = input( LOGIN_MENU )
		
		try:
			option = int( option, 10 )
		except:
			option = -1

		if option not in login_menu_options:
			login_attempts += 1 
			continue

		if option == 1:
			result, errno = handlers.login()

			if errno == err.LOGIN_SUCCESS:
				result = json.loads( result )

				if result[ 'logined' ]:
					print_f( LOGIN_VALID )

					handlers.logined_user_menu( result )
				else:
					print_errf( "[-] Login error!" )
			else:
				print_errf( "[-] Login error!" )

			continue

		# registration
		if option == 2:
			result, errno = handlers.registration()

			if errno == err.REGISTRATION_SUCCESS:
				result = json.loads( result )

				if 'AccessToken' in result.keys():
					print_f( REGISTRATION_VALID )
					print_f( "[+] Username: %s" % result[ 'Username' ] )
					print_f( "[+] AccessToken: %s" % result[ 'AccessToken' ] )
				else:
					print_errf( "[-] No access token in server response!" )

			else:
				print_f( REGISTRATION_INVALID )
				print_f( "Error: %s" % errno )

			continue

		# view last users
		if option == 3:
			result, errno = handlers.view_last_users()
			continue

		# check username
		if option == 4:
			Username = input( ENTER_USERNAME )

			if not handlers.check_username( Username ):
				print_errf( "[-] This username is invalid!" )
			else:
				print_f( "[+] This username is correct!" )

			continue

		# Exit
		if option == 5:
			print_f( NORMAL_EXIT )
			sys.exit( 1 )