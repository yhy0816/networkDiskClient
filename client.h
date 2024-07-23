#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QLineEdit>
#include <QPushButton>
#include "msghandler.h"
#include "protocol.h"

namespace Ui {
class Client;
}


class Client : public QMainWindow
{
    Q_OBJECT

public:
    static Client& getInstance();
    ~Client();
    QTcpSocket& getSocket() {return m_tcpSocket;}
    PDU* readPDU();
    QString getName() const {return m_userName;}
    void sendPDU(PDU* pdu);
private:
    //为了实现单例模式
    Client(QWidget *parent = nullptr);
    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;
    void loadConf();

    MsgHandler m_msgHandler;
    QString m_serverIp;
    quint16 m_serverPort;
    QTcpSocket m_tcpSocket;

    QLineEdit* edit;
    QPushButton* btn;
    Ui::Client *ui;
    QString m_userName;
    QByteArray m_buffer;

public slots:
    void onReadyRead();
private slots:
    void on_registBtn_clicked();
    void on_loginBtn_clicked();
};
#endif // MAINWINDOW_H
