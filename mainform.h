#ifndef MAINFORM_H
#define MAINFORM_H

#include <QWidget>
#include "friendform.h"
namespace Ui {
class MainForm;
}

class MainForm : public QWidget
{
    Q_OBJECT

public:

    ~MainForm();
    static MainForm& getInstance();
    FriendForm* getFriendForm();

private slots:

    void on_friendPB_clicked();
    void on_filePB_clicked();

private:
    explicit MainForm(QWidget *parent = nullptr);
    Ui::MainForm *ui;
};

#endif // MAINFORM_H
