#include "msghandler.h"
#include "mainform.h"
#include "client.h"
#include <cstring>
#include <QString>
#include <QMessageBox>
#include "logger.h"

MsgHandler::MsgHandler()
{

}

void MsgHandler::handleMsg(PDU *pdu)
{
    switch(pdu->msgType) {
            case EnMsgType::REGIST_RESPOND : {
                registHandle(pdu);

                break;
            }
            case EnMsgType::LOGIN_RESPOND : {
                loginHandle(pdu);
                break;
            }
            case EnMsgType::FIND_FRIEND_RESPONE : {
                findFriendHandle(pdu);

                break;
            }

            case EnMsgType::GET_ONLINE_USERS_RESPONE : {
                onlineUsersHandle(pdu);
                break;
            }
            default :{
                INFO << "未知消息";
            }


    }
}

void MsgHandler::registHandle(PDU *pdu)
{
    bool ret;
    memcpy(&ret, pdu->data, sizeof(ret));
    QString info = ret ? "注册成功" : "注册失败";
    QMessageBox::information(&Client::getInstance(), "注册", info);
}

void MsgHandler::loginHandle(PDU *pdu)
{
    bool ret;
    memcpy(&ret, pdu->data, sizeof(ret));
    QString info = ret ? "登录成功" : "账号或密码错误";
    QMessageBox::information(&Client::getInstance(), "注册", info);
    if(ret) {
        MainForm::getInstance().show();
        Client::getInstance().hide();
    }

}

void MsgHandler::findFriendHandle(PDU *pdu)
{
    int ret = 0;
    memcpy(&ret, pdu->data, sizeof(ret));
    QString info;
    if(ret == -1) {
        info = "用户不存在";
    } else if(ret == 0) {
        info = "用户在线";
    } else {
        info = "用户不在线";
    }
    QMessageBox::information(&Client::getInstance(), "查找用户", info);
}

void MsgHandler::onlineUsersHandle(PDU *pdu)
{
    int userNum = pdu->msgLen / 32;
    QStringList onlineUsers;
    for(int i = 0; i < userNum; i++) {
        char buf[32] = {};
        memcpy(buf, pdu->msg + 32 * i, 32);
        if(Client::getInstance().getName() == buf) continue;
        onlineUsers.append(buf);
    }
    MainForm::getInstance()
            .getFriendForm()
            ->getOnlineUserForm()
            .updateOnlineUsers(onlineUsers);
}
