class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    DIALOG = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

BANNER = '''
--------------------------------------------------------------------------------------------------------------------------
  ______ _           _        _      _ _            _____            _             _    _____                 _          
 |  ____| |         | |      (_)    (_) |          / ____|          | |           | |  / ____|               (_)         
 | |__  | | ___  ___| |_ _ __ _  ___ _| |_ _   _  | |     ___  _ __ | |_ _ __ ___ | | | (___   ___ _ ____   ___  ___ ___ 
 |  __| | |/ _ \/ __| __| '__| |/ __| | __| | | | | |    / _ \| '_ \| __| '__/ _ \| |  \___ \ / _ \ '__\ \ / / |/ __/ _ \ 
 | |____| |  __/ (__| |_| |  | | (__| | |_| |_| | | |___| (_) | | | | |_| | | (_) | |  ____) |  __/ |   \ V /| | (_|  __/
 |______|_|\___|\___|\__|_|  |_|\___|_|\__|\__, |  \_____\___/|_| |_|\__|_|  \___/|_| |_____/ \___|_|    \_/ |_|\___\___|
                                            __/ |                                                                        
                                           |___/                                                                         
--------------------------------------------------------------------------------------------------------------------------
'''
BANNER = bcolors.HEADER + BANNER + bcolors.ENDC

LOGIN_MENU = '''1. Login
2. Register
3. View latest registrations
4. Check username
5. Exit
> '''
LOGIN_MENU = bcolors.OKBLUE + LOGIN_MENU + bcolors.ENDC

USER_MENU = '''1. View profile
2. Add info
3. Save report
4. View report
5. Search user
6. View last reports
7. Exit
> '''

USER_MENU = bcolors.OKBLUE + USER_MENU + bcolors.ENDC

NORMAL_EXIT = '[+] Goodbye!'
NORMAL_EXIT = bcolors.OKBLUE + NORMAL_EXIT + bcolors.ENDC

# errors
ERROR_LOGIN_ATTEMPTS = '''[-] You have exceeded the count of authorization attempts\n[-] Exit'''
ERROR_LOGIN_ATTEMPTS = bcolors.FAIL + ERROR_LOGIN_ATTEMPTS + bcolors.ENDC


# dialog msgs
ENTER_USERNAME = '[?] Enter username: '
ENTER_USERNAME = bcolors.DIALOG + ENTER_USERNAME + bcolors.ENDC

ENTER_STATUS = '[?] Enter status: '
ENTER_STATUS = bcolors.DIALOG + ENTER_STATUS + bcolors.ENDC

ENTER_TOKEN = '[?] Enter access token: '
ENTER_TOKEN = bcolors.DIALOG + ENTER_TOKEN + bcolors.ENDC

ENTER_NEW_INFO = "[?] Enter new info: "
ENTER_NEW_INFO = bcolors.DIALOG + ENTER_NEW_INFO + bcolors.ENDC

ENTER_REPORT_NAME = "[?] Enter report name: "
ENTER_REPORT_NAME = bcolors.DIALOG + ENTER_REPORT_NAME + bcolors.ENDC

ENTER_REPORT_TYPE = "[?] Enter report type: "
ENTER_REPORT_TYPE = bcolors.DIALOG + ENTER_REPORT_TYPE + bcolors.ENDC

ENTER_REPORT_DATA = "[?] Enter report data: "
ENTER_REPORT_DATA = bcolors.DIALOG + ENTER_REPORT_DATA + bcolors.ENDC

ENTER_REPORT_PASSWORD = "[?] Enter report password: "
ENTER_REPORT_PASSWORD = bcolors.DIALOG + ENTER_REPORT_PASSWORD + bcolors.ENDC

ENTER_NAME_TO_SEARCH = "[?] Enter a name to search: "
ENTER_NAME_TO_SEARCH = bcolors.DIALOG + ENTER_NAME_TO_SEARCH + bcolors.ENDC


LOGIN_VALID = "[+] Login is successful!"
LOGIN_VALID = bcolors.OKGREEN + LOGIN_VALID + bcolors.ENDC

REGISTRATION_VALID = "[+] Registration is successful!"
REGISTRATION_VALID = bcolors.OKGREEN + REGISTRATION_VALID + bcolors.ENDC

REPORT_SAVED = '[+] Report saved is successful!'
REPORT_SAVED = bcolors.OKGREEN + REPORT_SAVED + bcolors.ENDC

REGISTRATION_INVALID = "[-] Registration error!" 
REGISTRATION_INVALID = bcolors.FAIL + REGISTRATION_INVALID + bcolors.ENDC

ERROR_TO_MANY_ACTIONS = '[-] To many actions!'
ERROR_TO_MANY_ACTIONS = bcolors.FAIL + ERROR_TO_MANY_ACTIONS + bcolors.ENDC


MSG_REPORT_NOT_SAVED = '[-] Report not saved!'
MSG_REPORT_NOT_SAVED = bcolors.FAIL + MSG_REPORT_NOT_SAVED + bcolors.ENDC


ERROR_VIEW_REPORT = '[-] Report view error!'
ERROR_VIEW_REPORT = bcolors.FAIL + ERROR_VIEW_REPORT + bcolors.ENDC

ERROR_SEARCH_NAME = '[-] Error in name search!'
ERROR_SEARCH_NAME = bcolors.FAIL + ERROR_SEARCH_NAME + bcolors.ENDC


WRONG_REPORT_NAME = "[-] Wrong report name!"
WRONG_REPORT_NAME = bcolors.FAIL + WRONG_REPORT_NAME + bcolors.ENDC


WRONG_REPORT_TYPE = "[-] Wrong report type!"
WRONG_REPORT_TYPE = bcolors.FAIL + WRONG_REPORT_TYPE + bcolors.ENDC


WRONG_REPORT_PASSWORD = "[-] Wrong password!"
WRONG_REPORT_PASSWORD = bcolors.FAIL + WRONG_REPORT_PASSWORD + bcolors.ENDC