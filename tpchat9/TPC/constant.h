#ifndef CONSTANT_H
#define CONSTANT_H

/** TPC协议规定的最大消息长度 */
#define MAXLEN 1024

/** 发送消息类型 */
enum MessageType {
    TYPE_ERR,       // 无类型
    SENT_LOGIN,     // 发送登录广播
    SENT_CHAT,      // 发送聊天请求
    SENT_LOGOUT,    // 发送下线广播
    RESP_LOGIN,     // 发送登录响应或解析错误响应
    RESP_CHAT,      // 发送聊天响应
};

/** 错误码 */
enum ErrCode {
    ERR_NO        = 100,    // 无错误
    ERR_RECEIVE   = 201,    // 聊天接收人错误
    ERR_MSGLENGTH = 202,    // 消息长度与消息内容长度不匹配
    ERR_JSON      = 401,    // json解析错误
    ERR_MAXLENGTH = 402,    // 消息长度超过TPC最大限制
    ERR_LOGIN     = 501,    // 网络中存在同名用户
};

/** 接收信息解析结果 */
enum CommandDecode {
    NO_COMMAND,             // 无指令
    NEW_JSON_ERR,           // 消息json解析错误
    NEW_TPC_ERR,            // 消息TPC解析错误
    NEW_LOGIN_REQ,          // 收到正常的登录广播
    NEW_LOGIN_JSON,         // 收到登录响应：json解析错误
    NEW_LOGIN_JSON_SEND,    // 收到json解析错误的登录广播
    NEW_LOGIN_DUP,          // 收到登录响应：网络同名
    NEW_LOGIN_OK,           // 收到登录响应：可以登录
    NEW_LOGOUT_REQ,         // 收到离线广播
    NEW_CHAT_REQ,           // 收到正常的聊天请求
    NEW_CHAT_JSON,          // 收到聊天响应：json解析错误
    NEW_CHAT_JSON_SEND,     // 收到json解析错误的聊天请求
    NEW_CHAT_DST,           // 收到聊天响应：目标用户错误
    NEW_CHAT_LEN,           // 收到聊天响应：消息长度超过最大限制
    NEW_CHAT_BODYLEN,       // 收到聊天响应：消息长度与消息内容不匹配
    NEW_CHAT_OK,            // 收到聊天响应：正常聊天
};

/** 头像ID */
enum PortraitID {
    PORTRAIT_DEFAULT,
    PORTRAIT_01,
    PORTRAIT_02,
    PORTRAIT_03,
    PORTRAIT_04,
    PORTRAIT_05,
    PORTRAIT_06,
    PORTRAIT_07,
    PORTRAIT_08,
    PORTRAIT_09,
};

#endif // CONSTANT_H
