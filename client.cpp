#include "ui_client.h"
#include "logger.h"
#include "mainform.h"
#include "client.h"

#include <QFile>
#include <QHostAddress>
#include <QTextStream>
#include <protocol.h>
#include <cstring>
#include <QMessageBox>


Client::Client(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Client)
{
    ui->setupUi(this);
    loadConf(); // 读取配置文件

    connect(&m_tcpSocket, &QTcpSocket::connected, []{
        INFO << "连接服务器成功";
    });

    connect(&m_tcpSocket, &QTcpSocket::readyRead, this, &Client::onReadyRead);

    m_tcpSocket.connectToHost(QHostAddress(m_serverIp), m_serverPort);


}

void Client::onReadyRead()
{
    INFO << "可读消息长度 " << m_tcpSocket.bytesAvailable();

    QByteArray data = m_tcpSocket.readAll();

    m_buffer.append(data);

    while(m_buffer.size() >= static_cast<int>(sizeof(PDU))) {
        PDU* pdu = reinterpret_cast<PDU*>(m_buffer.data());
        int totalLen = pdu->totalLen;
        if(totalLen > m_buffer.size()) {
            break;
        }
        PDU* msgPDU = makePDU(pdu->msgLen);
        memcpy(msgPDU, pdu, pdu->totalLen);
        m_msgHandler.printPDU(msgPDU);
        m_msgHandler.handleMsg(msgPDU);
        free(msgPDU);
        m_buffer.remove(0, totalLen);
    }

//    PDU* pdu = readPDU();
//    m_msgHandler.handleMsg(pdu);
//    free(pdu);
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

PDU *Client::readPDU()
{
    INFO << "可读消息长度 " << m_tcpSocket.bytesAvailable();
    uint totalLen = 0;
    m_tcpSocket.read(reinterpret_cast<char*>(&totalLen), sizeof(totalLen)); // 先读取消息总长度
    uint MsgLen = totalLen - sizeof(PDU); // 计算出实际消息长度
    PDU *pdu = makePDU(MsgLen); // 创建 pdu 用于保存协议消息
    // 接受剩余协议内容

    m_tcpSocket.read(reinterpret_cast<char*>(pdu) + sizeof(totalLen), totalLen - sizeof(totalLen));

    INFO << "总长度: " << totalLen ;
    INFO << "消息类型: " << (int)pdu->msgType;
    INFO << "消息长度" << pdu->msgLen;
    INFO << "消息内容" << pdu->msg;
    return pdu;
}

void Client::sendPDU(PDU *pdu)
{

    INFO << "send : ";
    m_msgHandler.printPDU(pdu);
    m_tcpSocket.write(reinterpret_cast<char*>(pdu), pdu->totalLen);
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


void Client::on_registBtn_clicked()
{
    QString name = ui->nameLE->text();
    QString pwd = ui->pwdLE->text();

    if(name.isEmpty() || pwd.isEmpty()) {
         QMessageBox::information(this, "注册", "账号或密码不合法!");
         return;
    }
    std::string sname = name.trimmed().toStdString();
    std::string spwd = pwd.trimmed().toStdString();
    if(sname.size() >= 32 || spwd.size() >= 32) {
         QMessageBox::information(this, "注册", "账号或密码不合法!");
         return;
    }
    PDU* pdu = makePDU(0);
    pdu->msgType =EnMsgType::REGIST_MSG;

//    INFO << sname.size();
//    INFO << spwd.size();
    memcpy(pdu->data, sname.c_str(), sname.size()); // 用户名放到 pdu 中
    memcpy(pdu->data + 32, spwd.c_str(), spwd.size()); // 密码放到 pdu 中
//    m_tcpSocket.write(reinterpret_cast<char*>(pdu), pdu->totalLen);
    sendPDU(pdu);
    free(pdu);
}

void Client::on_loginBtn_clicked()
{
    QString name = ui->nameLE->text();
    QString pwd = ui->pwdLE->text();
    if(name.isEmpty() || pwd.isEmpty()) {
         QMessageBox::information(this, "登录", "账号或密码不合法!");
    }
    INFO << "name " << name << "pwd " << pwd;
    m_userName = name;
    PDU* pdu = makePDU(0);
    pdu->msgType =EnMsgType::LOGIN_MSG;
    memcpy(pdu->data, name.toStdString().c_str(), 32); // 用户名放到 pdu 中
    memcpy(pdu->data + 32, pwd.toStdString().c_str(), 32); // 密码放到 pdu 中
    sendPDU(pdu);
//    m_tcpSocket.write(reinterpret_cast<char*>(pdu), pdu->totalLen);
    free(pdu);
}
