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

    switch(pdu->msgType) {
            case EnMsgType::REGIST_RESPOND : {
                bool ret;
                memcpy(&ret, pdu->data, sizeof(ret));
                QString info = ret ? "注册成功" : "注册失败";

                QMessageBox::information(this, "注册", info);

                break;
            }
            case EnMsgType::LOGIN_RESPOND : {
                bool ret;
                memcpy(&ret, pdu->data, sizeof(ret));
                QString info = ret ? "登录成功" : "账号或密码错误";
                QMessageBox::information(this, "注册", info);
                if(ret) {

                    MainForm::getInstance().show();
                    this->hide();
                }
                break;
            }
            case EnMsgType::FIND_FRIEND_RESPONE : {
                int ret = 0;
                memcpy(&ret, pdu->data, sizeof(ret));
                QString info;
                if(ret == -1) {
                    info = "用户不存在";
                } else if(ret == 0) {
                    info = "用户在线";
                } else {
                    info = "用户不线";
                }

                QMessageBox::information(this, "查找用户", info);

                break;
            }
            default :{
                INFO << "未知消息";
            }


        }

    free(pdu);
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


void Client::on_registBtn_clicked()
{
    QString name = ui->nameLE->text();
    QString pwd = ui->pwdLE->text();
    if(name.isEmpty() || pwd.isEmpty()) {
         QMessageBox::information(this, "注册", "账号或密码不合法!");
    }
    PDU* pdu = makePDU(0);
    pdu->msgType =EnMsgType::REGIST_MSG;
    memcpy(pdu->data, name.trimmed().toStdString().c_str(), 32); // 用户名放到 pdu 中
    memcpy(pdu->data + 32, name.trimmed().toStdString().c_str(), 32); // 密码放到 pdu 中
    m_tcpSocket.write(reinterpret_cast<char*>(pdu), pdu->totalLen);
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
    m_tcpSocket.write(reinterpret_cast<char*>(pdu), pdu->totalLen);
}
