#ifndef MSGHANDLER_H
#define MSGHANDLER_H
#include "protocol.h"

class MsgHandler
{
public:
    MsgHandler();
    void handleMsg(PDU* pdu);
    void registHandle(PDU* pdu);
    void loginHandle(PDU* pdu);
    void findFriendHandle(PDU* pdu);
    void onlineUsersHandle(PDU* pdu);
};

#endif // MSGHANDLER_H
