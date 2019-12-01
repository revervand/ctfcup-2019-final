#define PACKET_SIZE_ERR 0x1101

#define ERR_MSG_IDENT "\xde\xad\xbe\xef"

// cmd's errors
#define ERR_CMD_ID 0x1201

// registration errors
#define ERR_REG_USERNAME_SIZE 0x1301
#define ERR_REG_STATUS_SIZE   0x1302
#define ERR_REG_EMPTY_TOKEN   0x1303
#define ERR_REG_INVALID_TOKEN_SIZE 0x1304
#define ERR_REG_INVALID_TOKEN_VALUE 0x1305
#define ERR_REG_ACCESS_TOKEN_GEN 0x1306
#define ERR_REG_USER_EXIST 0x1307
#define ERR_REG_ADD_USER_ERROR 0x1308

// view last users errors
#define ERR_VIEW_TOKEN_SIZE 0x1401
#define ERR_VIEW_INVALID_TOKEN_VALUE 0x1402
#define ERR_VIEW_NO_USERS 0x1403
#define ERR_VIEW_USER_LIST_INIT 0x1404

// logins error
#define ERR_LOGIN_USER_NOT_EXIST 0x1501
#define ERR_LOGIN_TOKEN_INVALID 0x1502

// reports error
#define REPORT_NOT_SAVED 0x1601
#define FILE_NOT_FOUND 0x1602
#define ERR_SAVE_REP_PASSWORD_SIZE 0x1603

#define ERROR_SEARCHED_NAME 0x1701
#define ERR_SEARCH_USER_LIST_INIT 0x1702
#define ERR_SEARCH_NO_USERS 0x1703