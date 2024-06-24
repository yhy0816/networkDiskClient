#include "mainform.h"
#include "ui_mainform.h"

MainForm::MainForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainForm)
{
    ui->setupUi(this);
}

MainForm::~MainForm()
{
    delete ui;
}

MainForm &MainForm::getInstance()
{
    static MainForm mf;
    return mf;
}

FriendForm *MainForm::getFriendForm()
{
    return static_cast<FriendForm *>(ui->stackedWidget->widget(1));
}

void MainForm::on_friendPB_clicked()
{
    ui->filePB->setStyleSheet("QPushButton{border:none;padding:20px;}");
    ui->friendPB->setStyleSheet("QPushButton{border:none;background-color:rgb(255, 255, 255);padding:20px;}");

    ui->stackedWidget->setCurrentIndex(1);
}

void MainForm::on_filePB_clicked()
{
    ui->filePB->setStyleSheet("QPushButton{border:none;background-color:rgb(255, 255, 255);padding:20px;}");
    ui->friendPB->setStyleSheet("QPushButton{border:none;padding:20px;}");

    ui->stackedWidget->setCurrentIndex(0);
}
