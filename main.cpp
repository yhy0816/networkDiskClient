#include <QApplication>
#include "client.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    LoginForm lf;
//    lf.show();
    Client::getInstance().show();
    return a.exec();
}
