#include "onlineuserform.h"
#include "ui_onlineuserform.h"

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
