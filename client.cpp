#include "client.h"
#include <QFile>
#include <QHostAddress>
#include <QTextStream>
#include "logger.h"
#include <protocol.h>

Client::Client(QWidget *parent)
    : QMainWindow(parent)
{

    setUi();
    loadConf(); // 读取配置文件
    connect(&m_tcpSocket, &QTcpSocket::connected, []{
        INFO << "连接服务器成功";
    });
    m_tcpSocket.connectToHost(QHostAddress(m_serverIp), m_serverPort);

}

void Client::setUi()
{
    this->resize(400,400);
    edit = new QLineEdit(this);
    edit->move(100, 100);

    btn = new QPushButton("发送", this);
    btn->move(200, 100);

    connect(btn, &QPushButton::clicked, [this]{
        QString msg = edit->text();
        std::string stdmsg = msg.toStdString();
//        INFO << "stdmsg.size();" << stdmsg.size();
        uint msgsize = stdmsg.size() + 1;
        PDU* pdu = makePDU(msgsize);
        pdu->msgType = EnMsgType::REGIST_MSG;
        memcpy(pdu->msg,stdmsg.c_str(), msgsize);
        m_tcpSocket.write(reinterpret_cast<char*>(pdu), pdu->totalLen);
        free(pdu);
    });
}

Client &Client::getInstance()
{
    // 线程安全单例模式
    static Client ins;
    return ins;
}

Client::~Client()
{
}

void Client::loadConf() {
    QFile confFile(":/client.conf");
    if(!confFile.open(QIODevice::ReadOnly)) {
        INFO << "打开配置文件失败!!";
    }
    QString sport;
    QTextStream stream(&confFile);
    stream >> m_serverIp >> sport;
    m_serverPort = sport.toUShort();
    INFO << "ip " << m_serverIp << " port " << m_serverPort;
    confFile.close();
}

