#include "friendform.h"
#include "ui_friendform.h"
#include "client.h"
#include "logger.h"
#include "protocol.h"
#include <QInputDialog>
#include <cstring>

FriendForm::FriendForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendForm)
{
    ui->setupUi(this);
}

FriendForm::~FriendForm()
{
    delete ui;
}

void FriendForm::on_findFriendPB_clicked()
{
    QString name =  QInputDialog::getText(this, "查找", "查找用户");
    if(name.isEmpty()) return;
    std::string sname = name.trimmed().toStdString();
    PDU* pdu = makePDU(0);
    pdu->msgType = EnMsgType::FIND_FRIEND_MSG;
    memcpy(pdu->data, sname.c_str(), sname.size());
    Client::getInstance().getSocket().write(reinterpret_cast<char*>(pdu), pdu->totalLen);
}
