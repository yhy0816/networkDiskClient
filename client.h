#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QLineEdit>
#include <QPushButton>

class Client : public QMainWindow
{
    Q_OBJECT

public:
    static Client& getInstance();
    ~Client();
private:
    //为了实现单例模式
    Client(QWidget *parent = nullptr);
    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;

    void setUi() ;

    void loadConf();
    QString m_serverIp;
    quint16 m_serverPort;
    QTcpSocket m_tcpSocket;

    QLineEdit* edit;
    QPushButton* btn;
};
#endif // MAINWINDOW_H
