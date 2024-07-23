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
            case EnMsgType::ADD_FRIEND_RESPONE : {
                addFriendHandle(pdu);
                break;
            }

            case EnMsgType::ADD_FRIEND_MSG : {
                askFriendHandle(pdu);
                break;
            }
            case EnMsgType::AGREE_FRIEND_RESPONE : {
                agreeFriendHandle(pdu);
                break;
            }
            case EnMsgType::GET_FRIENDS_RESPONE : {
                getFriendsList(pdu);
                break;
            }
            case EnMsgType::REMOVE_FRIENDS_RESPONE : {
                removeFriendsList(pdu);
                break;
            }
            case EnMsgType::CHAT_MSG : {
                chatMsgHandle(pdu);
                break;
            }
            case EnMsgType::MKDIR_RESPONE : {
                makeDirHandle(pdu);
                break;
            }
            case EnMsgType::GET_FILES_RESPONE : {
                getFilesHandle(pdu);
                break;
            }
            case EnMsgType::DEL_DIR_RESPONE :
            case EnMsgType::DEL_FILE_RESPONE : {
                delDirHandle(pdu);
                break;
            }
            case EnMsgType::RENAME_FILE_RESPONE : {
                renameFileHandle(pdu);
                break;
            }
            default :{
                 INFO << "未知消息 " << (int)pdu->msgType;
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
        MainForm::getInstance().setWindowTitle(Client::getInstance().getName());
        MainForm::getInstance().show();
        Client::getInstance().hide();
    }

}

void MsgHandler::findFriendHandle(PDU *pdu)
{
    int ret = 0;
    memcpy(&ret, pdu->data, sizeof(ret));
    QString info;
    INFO << "ret = " << ret;
    if(ret == -1) {
        info = "用户不存在";
    } else if(ret == 1) {
        int choice = QMessageBox::information(&Client::getInstance(), "查找用户", "用户在线", "添加好友", "取消");
        if(choice == 1) return;
        PDU* addFriendPDU = makePDU(0);
        addFriendPDU->msgType = EnMsgType::ADD_FRIEND_MSG;
        std::string curname = Client::getInstance().getName().toStdString();
        memcpy(addFriendPDU->data, curname.c_str(), curname.size());
        std::string tarname = pdu->data + 32;
        memcpy(addFriendPDU->data + 32, tarname.c_str(), tarname.size());

        Client::getInstance().sendPDU(addFriendPDU);
        free(addFriendPDU);
        return;
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

void MsgHandler::addFriendHandle(PDU *pdu)
{
    int ret = 0;
    memcpy(&ret, pdu->data, sizeof ret);
    switch(ret) {
       case -1:
            QMessageBox::information(&MainForm::getInstance(), "提示", "服务器内部错误");
            break;

        case 0 : {
            QMessageBox::information(&MainForm::getInstance(), "提示", "好友请求已发送");
            break;
        }
        case 1 : {
            QMessageBox::information(&MainForm::getInstance(), "提示", "用户不存在");
            break;
        }
        case 2 : {
            QMessageBox::information(&MainForm::getInstance(), "提示", "用户不在线");
            break;
        }
        case 3 : {
            QMessageBox::information(&MainForm::getInstance(), "提示", "已经是好友了");
            break;
        }
    }
}

void MsgHandler::askFriendHandle(PDU *pdu)
{
    if(QMessageBox::Yes ==
            QMessageBox::question(&MainForm::getInstance(),
            "好友请求", QString("是否同意 %1 的好友请求").arg(pdu->msg))) {
        pdu->msgType = EnMsgType::AGREE_FRIEND_MSG;
        Client::getInstance().sendPDU(pdu);
    }
}

void MsgHandler::agreeFriendHandle(PDU *pdu)
{
    QString name1 = pdu->data;
    QString name2 = pdu->data + 32;
    if(name1 == Client::getInstance().getName()) {
        QMessageBox::information(&MainForm::getInstance(), "好友", QString("你已和 %1 成为好友").arg(name2));
    } else {
        QMessageBox::information(&MainForm::getInstance(), "好友", QString("你已和 %1 成为好友").arg(name1));

    }
    MainForm::getInstance().getFriendForm()->refreshFriendList();
}

void MsgHandler::getFriendsList(PDU *pdu)
{
    QStringList list;
    int size = pdu->msgLen / 32;

    for(int i = 0; i < size; i++) {
        list.append(pdu->msg + i * 32);
    }
    MainForm::getInstance().getFriendForm()->updateFriends(list);
}

void MsgHandler::removeFriendsList(PDU *pdu)
{
    int ret;
    memcpy(&ret, pdu->data, sizeof(ret));
    QString info = "删除好友成功";
    if(ret == 0) {
        info = "删除好友失败";
    } else if(ret == -1) {
        info = "服务器内部异常";
    }
    QMessageBox::information(&MainForm::getInstance(), "删除好友", info);
    MainForm::getInstance().getFriendForm()->refreshFriendList();
}

void MsgHandler::chatMsgHandle(PDU *pdu)
{
    ChatForm* chatform = MainForm::getInstance().getChatForm();
    chatform->setMsg(pdu->data, pdu->msg);

    chatform->setFriendName(pdu->data);
    if(chatform->isHidden()) {
        chatform->show();
    }
}

void MsgHandler::makeDirHandle(PDU *pdu)
{
    int ret = 0;
    memcpy(&ret, pdu->data, sizeof ret);
    if(ret) {
        QMessageBox::information(&MainForm::getInstance(), "提示", "创建文件夹成功");
    } else {
        QMessageBox::information(&MainForm::getInstance(), "提示", "创建文件夹失败");

    }
    MainForm::getInstance().getFileForm()->sendGetFilesRequest();

}

void MsgHandler::getFilesHandle(PDU *pdu)
{
    int cnt = pdu->msgLen / sizeof (FileInfo);
    QList<FileInfo*> fileInfoList;
    for(int i = 0; i < cnt; i++) {
        FileInfo* cur = reinterpret_cast<FileInfo*>(pdu->msg) + i;
        fileInfoList.append(cur);
    }
    MainForm::getInstance().getFileForm()->refreshList(fileInfoList);

}

void MsgHandler::delDirHandle(PDU *pdu)
{
    bool ret;
    memcpy(&ret, pdu->data, sizeof(ret));
    if(ret) {
        MainForm::getInstance().getFileForm()->sendGetFilesRequest();
    } else {
        QMessageBox::information(MainForm::getInstance().getFileForm(), "删除文件夹", "删除失败");
    }
}

void MsgHandler::renameFileHandle(PDU *pdu)
{
    int ret = 0;
    memcpy(&ret, pdu->data, sizeof(ret));
    if(ret == 1) {
        MainForm::getInstance().getFileForm()->sendGetFilesRequest();

    } else {
        QMessageBox::information(MainForm::getInstance().getFileForm(), "重命名", "重命名失败");

    }
}

void MsgHandler::printPDU(PDU *pdu) {
    INFO << "总长度: " << pdu->totalLen ;
    INFO << "消息类型: " << (int)pdu->msgType;
    INFO << "数据1" << pdu->data;
    INFO << "数据2" << pdu->data + 32;
    INFO << "消息长度" << pdu->msgLen;
    INFO << "消息内容" << pdu->msg;

}
