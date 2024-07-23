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
    void addFriendHandle(PDU* pdu);
    void askFriendHandle(PDU* pdu);
    void agreeFriendHandle(PDU* pdu);
    void getFriendsList(PDU* pdu);
    void removeFriendsList(PDU* pdu);
    void chatMsgHandle(PDU* pdu);
    void makeDirHandle(PDU* pdu);
    void getFilesHandle(PDU* pdu);
    void delDirHandle(PDU* pdu);
    void renameFileHandle(PDU* pdu);
    void printPDU(PDU* pdu);
};

#endif // MSGHANDLER_H
