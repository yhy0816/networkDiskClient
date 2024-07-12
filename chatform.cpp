#include "chatform.h"
#include "ui_chatform.h"
#include "protocol.h"
#include "client.h"
#include "logger.h"
ChatForm::ChatForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatForm)
{
    ui->setupUi(this);
}

ChatForm::~ChatForm()
{
    delete ui;
}

void ChatForm::setMsg(const QString &userName, const QString &msg)
{
    ui->msgTE->setPlainText(ui->msgTE->toPlainText() + QString("%1 : %2\r\n").arg(userName).arg(msg));
}


void ChatForm::on_sendPB_clicked()
{

//    INFO << "clicked";
    std::string msg = ui->msgLE->text().trimmed().toStdString();
    if(msg.empty()) return;

    PDU* pdu = makePDU(msg.size() + 1);
    pdu->msgType = EnMsgType::CHAT_MSG;
    std::string curname = Client::getInstance().getName().toStdString();
    memcpy(pdu->data, curname.c_str(), curname.size());
    std::string tarname = this->m_friendName.toStdString();
    memcpy(pdu->data + 32, tarname.c_str(), tarname.size());
    memcpy(pdu->msg, msg.c_str(), msg.size());
    Client::getInstance().sendPDU(pdu);
    free(pdu);
}
