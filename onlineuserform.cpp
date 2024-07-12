#include "onlineuserform.h"
#include "ui_onlineuserform.h"
#include "client.h"
#include "logger.h"

OnlineUserForm::OnlineUserForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OnlineUserForm)
{
    ui->setupUi(this);
}

OnlineUserForm::~OnlineUserForm()
{
    delete ui;
}

void OnlineUserForm::updateOnlineUsers(QStringList &onlineUsers)
{
    ui->onlineUserLW->clear();
    ui->onlineUserLW->addItems(onlineUsers);

}

void OnlineUserForm::on_onlineUserLW_itemDoubleClicked(QListWidgetItem *item)
{
    std::string curName = Client::getInstance().getName().toStdString();
    std::string tarName = item->text().toStdString();
    INFO << "好友请求" << Client::getInstance().getName() << " " << item->text();
    PDU* pdu= makePDU(0);
    pdu->msgType = EnMsgType::ADD_FRIEND_MSG;
    memcpy(pdu->data, curName.c_str(), curName.size());
    memcpy(pdu->data + 32, tarName.c_str(), curName.size());
    Client::getInstance().sendPDU(pdu);
    free(pdu);
}
