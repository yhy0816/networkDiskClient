#ifndef FRIENDFORM_H
#define FRIENDFORM_H

#include <QWidget>

namespace Ui {
class FriendForm;
}

class FriendForm : public QWidget
{
    Q_OBJECT

public:
    explicit FriendForm(QWidget *parent = nullptr);
    ~FriendForm();

private slots:
    void on_findFriendPB_clicked();

private:
    Ui::FriendForm *ui;
};

#endif // FRIENDFORM_H
