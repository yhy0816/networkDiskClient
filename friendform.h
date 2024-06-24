#ifndef FRIENDFORM_H
#define FRIENDFORM_H

#include <QWidget>
#include "onlineuserform.h"
namespace Ui {
class FriendForm;
}

class FriendForm : public QWidget
{
    Q_OBJECT

public:
    explicit FriendForm(QWidget *parent = nullptr);
    ~FriendForm();
    OnlineUserForm& getOnlineUserForm();
private slots:
    void on_findFriendPB_clicked();

    void on_onlineFriendPb_clicked();

private:
    Ui::FriendForm *ui;
    OnlineUserForm onlineUser;
};

#endif // FRIENDFORM_H
