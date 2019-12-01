#!/usr/bin/env python3

from ecs_lib import *
import sys

def put( host, flag_id, flag, vuln ):
    
    esc_front = ECSFrontend( host, FRONT_PORT )
    ecs_back  = ECSBackend( host, BACKEND_PORT )

    reg_data = esc_front.register_in_service()

    username = ''
    a_token = ''

    try:
        username = reg_data[ 'username' ]
        a_token = reg_data[ 'access_token' ]
    except:
        cquit( Status.MUMBLE, "Could not register" )

    view_last_users = esc_front.view_last_users( username )
    ecs_back.search_user( username, a_token )

    ## check user login
    report_pass = rnd_string( random.randint( 6, 14 ) )
    report_name = rnd_string( random.randint( 6, 15 ) )

    report_type = 'NOT'

    if vuln == "1":
        session = esc_front.login( username, a_token )
        esc_front.change_info( session, flag )
    
    elif vuln == "2":
        report_type = 'public'
        ecs_back.save_public_report( username, a_token, report_name, flag )

    else:
        report_type = 'private'
        ecs_back.save_private_report( username, a_token, report_name, report_pass, flag )
        
    cquit( Status.OK, f'{username}:{a_token}:{report_type}:{report_name}:{report_pass}' )


def get( host, flag_id, flag, vuln ):
    
    items = flag_id.split( ":" )

    username = items[ 0 ]
    a_token  = items[ 1 ]
    report_type = items[ 2 ]
    report_name = items[ 3 ]
    report_pass = items[ 4 ]

    if vuln == "1":
        esc_front = ECSFrontend( host, FRONT_PORT )

        session = esc_front.login( username, a_token )
        esc_front.check_info_flag( session, flag )
        esc_front.exit( session )

    elif vuln == "2":
        report_type = 'public'
        ecs_back  = ECSBackend( host, BACKEND_PORT )
        ecs_back.get_public_report( username, a_token, report_name, flag )
    
    else:
        report_type = 'private'
        ecs_back  = ECSBackend( host, BACKEND_PORT )
        ecs_back.get_private_report( username, a_token, report_name, report_pass, flag )

    cquit( Status.OK )

def check( host ):
    
    ## check frontend
    ## check registration
    esc_front = ECSFrontend( host, FRONT_PORT )
    reg_data = esc_front.register_in_service()

    username = ''
    a_token = ''

    try:
        username = reg_data[ 'username' ]
        a_token = reg_data[ 'access_token' ]
    except:
        cquit( Status.MUMBLE, "Could not register" )

    ## check view_last_users
    view_last_users = esc_front.view_last_users( username )

    ## check user login
    session = esc_front.login( username, a_token )

    ## check user menu
    esc_front.check_menu( session, username )

    ## check backend
    ## check connection
    ecs_back = ECSBackend( host, BACKEND_PORT )
    
    ## check login
    ecs_back.login( username, a_token )

    ## check search
    ecs_back.search_user( username, a_token )

    cquit( Status.OK )

if __name__ == '__main__':

    action, *args = sys.argv[ 1: ]

    # try:
    if action == "check":
        host, = args
        check( host )

    elif action == "put":
        host, flag_id, flag, vuln = args
        put( host, flag_id, flag, vuln )
    
    elif action == "get":
        host, flag_id, flag, vuln = args
        get( host, flag_id, flag, vuln )
    
    else:
        quit( Status.ERROR, 'System error', 'Unknown action: ' + action )
    
    cquit( Status.ERROR )

    # except socket.timeout:
    #     cquit( Status.DOWN, 'Connection error' )
   
    # except SystemError as e:
    #     raise
   
    # except Exception as e:
    #     cquit( Status.ERROR, 'System error', str( e ) )