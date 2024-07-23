#ifndef FRIENDFORM_H
#define FRIENDFORM_H

#include <QWidget>
#include "onlineuserform.h"
#include "chatform.h"

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
    void updateFriends(const QStringList& list);
    void refreshFriendList();
    ChatForm* getChatForm() {return  &chatForm;}

private slots:
    void on_findFriendPB_clicked();

    void on_onlineFriendPb_clicked();

    void on_refreshPb_clicked();

    void on_deleteFriendPB_clicked();

    void on_chatPB_clicked();

    void on_friendListLW_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::FriendForm *ui;
    OnlineUserForm onlineUser;
    ChatForm chatForm;
};

#endif // FRIENDFORM_H
