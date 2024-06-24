#ifndef ONLINEUSERFORM_H
#define ONLINEUSERFORM_H

#include <QWidget>

namespace Ui {
class OnlineUserForm;
}

class OnlineUserForm : public QWidget
{
    Q_OBJECT

public:
    explicit OnlineUserForm(QWidget *parent = nullptr);
    ~OnlineUserForm();
    void updateOnlineUsers(QStringList &onlineUsers);
private:
    Ui::OnlineUserForm *ui;
};

#endif // ONLINEUSERFORM_H
