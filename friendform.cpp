#include "friendform.h"
#include "ui_friendform.h"
#include "client.h"
#include "logger.h"
#include "protocol.h"
#include <QInputDialog>
#include <QMessageBox>
#include <cstring>

FriendForm::FriendForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendForm)
{
    ui->setupUi(this);
     refreshFriendList();
}

FriendForm::~FriendForm()
{
    delete ui;
}

OnlineUserForm &FriendForm::getOnlineUserForm()
{
    return onlineUser;
}

void FriendForm::updateFriends(const QStringList& list)
{
    ui->friendListLW->clear();
    ui->friendListLW->addItems(list);
}

void FriendForm::on_findFriendPB_clicked()
{
    QString name =  QInputDialog::getText(this, "查找", "查找用户");
    if(name.isEmpty()) return;
    std::string sname = name.trimmed().toStdString();
    PDU* pdu = makePDU(0);
    pdu->msgType = EnMsgType::FIND_FRIEND_MSG;
    memcpy(pdu->data, sname.c_str(), sname.size());
    Client::getInstance().sendPDU(pdu);
    free(pdu);
}

void FriendForm::on_onlineFriendPb_clicked()
{
    if(this->onlineUser.isHidden()) {
        this->onlineUser.show();
    }

    PDU* pdu = makePDU(0);
    pdu->msgType = EnMsgType::GET_ONLINE_USERS_MSG;
    Client::getInstance().sendPDU(pdu);
    free(pdu);
}



void FriendForm::on_refreshPb_clicked()
{
    refreshFriendList();
}

void FriendForm::refreshFriendList()
{
    std::string curname = Client::getInstance().getName().toStdString();
    PDU* pdu = makePDU(0);
    pdu->msgType = EnMsgType::GET_FRIENDS_MSG;
    memcpy(pdu->data, curname.c_str(), curname.size());
    Client::getInstance().sendPDU(pdu);
    free(pdu);
}

void FriendForm::on_deleteFriendPB_clicked()
{
    QListWidgetItem *item =  ui->friendListLW->currentItem();
    if(item == nullptr) {
        QMessageBox::information(this, "删除好友", "请选择要删除的好友");
        return;
    }
    std::string fname =  item->text().toStdString();
    PDU* pdu = makePDU(0);
    pdu->msgType = EnMsgType::REMOVE_FRIENDS_MSG;
    std::string curname = Client::getInstance().getName().toStdString();
    memcpy(pdu->data, curname.c_str(), curname.size());
    memcpy(pdu->data + 32, fname.c_str(), fname.size());

    Client::getInstance().sendPDU(pdu);
    free(pdu);
}

void FriendForm::on_chatPB_clicked()
{
    QListWidgetItem *item =  ui->friendListLW->currentItem();
    if(item == nullptr) {
        QMessageBox::information(this, "聊天", "请选择要聊天的好友");
        return;
    }

    this->chatForm.setFriendName(item->text());
    this->chatForm.show();
}
