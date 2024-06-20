#include <QApplication>
#include "client.h"
#include "mainform.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Client::getInstance().show();
    return a.exec();
}
