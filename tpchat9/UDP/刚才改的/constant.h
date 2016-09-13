#ifndef CONSTANT_H
#define CONSTANT_H

enum MessageType {
    SENT_LOGIN,
    SENT_CHAT,
    SENT_LOGOUT,
    RESP_LOGIN,
    RESP_CHAT,
    TYPE_ERR,
};

enum ErrCode {
    ERR_NO        = 100,
    ERR_RECEIVE   = 201,
    ERR_MSGLENGTH = 202,
    ERR_JSON      = 401,
    ERR_MAXLENGTH = 402,
    ERR_LOGIN     = 501,
};

enum CommandDecode {
    NO_COMMAND,
    NEW_JSON_ERR,
    NEW_TPC_ERR,
    NEW_LOGIN_REQ,
    NEW_LOGIN_JSON,
    NEW_LOGIN_JSON_SEND,
    NEW_LOGIN_DUP,
    NEW_LOGIN_OK,
    NEW_LOGOUT_REQ,
    NEW_CHAT_REQ,
    NEW_CHAT_JSON,
    NEW_CHAT_JSON_SEND,
    NEW_CHAT_DST,
    NEW_CHAT_LEN,
    NEW_CHAT_BODYLEN,
    NEW_CHAT_OK,
};

#endif // CONSTANT_H
