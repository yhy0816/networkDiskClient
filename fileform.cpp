#include "fileform.h"
#include "ui_fileform.h"
#include "logger.h"
#include <QInputDialog>
#include <QLabel>
#include <QMessageBox>
#include "client.h"

FileForm::FileForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileForm)
{
    ui->setupUi(this);
    m_userName = Client::getInstance().getName();
    m_curPath = "/";
    sendGetFilesRequest();
}

FileForm::~FileForm()
{
    delete ui;
}

void FileForm::refreshList(const QList<FileInfo*>& fileList)
{
    for(int i = 0; i < m_fileWidgetList.size(); i++) {
        delete m_fileWidgetList[i];
    }
    m_fileWidgetList.clear();
    ui->fileListLW->clear();
    for(int i = 0; i < fileList.size(); i++) {
        QListWidgetItem* item = new QListWidgetItem();
        if(fileList[i]->fileType == 0) {
            QIcon icon = QIcon(":/images/dir.png");
            icon.setThemeName("dir");
            item->setIcon(icon);
        } else if(fileList[i]->fileType == 1) {
            QIcon icon = QIcon(":/images/dir.png");
            icon.setThemeName("file");
            item->setIcon(icon);
        }
        item->setText(fileList[i]->fileName);
        ui->fileListLW->addItem(item);
    }


}

void FileForm::sendGetFilesRequest()
{
    std::string curpath = m_curPath.toStdString();
    std::string uname = m_userName.toStdString();
    PDU* pdu = makePDU(curpath.size());
    pdu->msgType = EnMsgType::GET_FILES_MSG;
    memcpy(pdu->data, uname.c_str(), uname.size());
    memcpy(pdu->msg, curpath.c_str(), curpath.size());
    Client::getInstance().sendPDU(pdu);
    free(pdu);
}

void FileForm::on_makeDIrPB_clicked()
{
    QString dirName =  QInputDialog::getText(this, "新建文件夹", "输入文件夹名");
    INFO << dirName;
    if(dirName.isEmpty() || dirName.toStdString().size() >= 32){
        QMessageBox::information(this, "新建文件夹", "名称长度不合法");
        return;

    }
    std::string curpath = m_curPath.toStdString();
    PDU* pdu = makePDU(curpath.size() + 1);
    pdu->msgType = EnMsgType::MKDIR_MSG;
    std::string userPath = m_userName.toStdString();
    memcpy(pdu->data, userPath.c_str(), userPath.size());
    std::string newdirName = dirName.toStdString();
    memcpy(pdu->data + 32, newdirName.c_str(), newdirName.size());
    memcpy(pdu->msg, curpath.c_str(), curpath.size());
    Client::getInstance().sendPDU(pdu);
    free(pdu);

}

void FileForm::on_pushButton_clicked()
{

}



void FileForm::on_refreshFilePB_clicked()
{
    sendGetFilesRequest();
}

void FileForm::on_delDirPB_clicked()
{
    auto item = ui->fileListLW->currentItem();
    if(item == nullptr) return;
    if(item->icon().themeName() != "dir") {
        QMessageBox::information(this,  "删除", "请选择文件夹");
    }
    QString choiceDir = item->text();
    INFO << "删除: " << QString(m_curPath + choiceDir);
    std::string choicePath = QString(m_curPath + choiceDir).toStdString();
    int ret = QMessageBox::question(this, "删除", "确定要删除文件夹吗, 删除不可恢复");
    if(ret == QMessageBox::No) {
        return;
    }

    PDU* pdu = makePDU(choicePath.size());
    pdu->msgType = EnMsgType::DEL_DIR_MSG;
    std::string curname = m_userName.toStdString();
    memcpy(pdu->data, curname.c_str(), curname.size());
    memcpy(pdu->msg, choicePath.c_str(), choicePath.size());
    Client::getInstance().sendPDU(pdu);
    free(pdu);
}

void FileForm::on_delFilePB_clicked()
{
    auto item = ui->fileListLW->currentItem();
    if(item == nullptr) return;
    if(item->icon().themeName() != "file") {
        QMessageBox::information(this,  "删除", "请选择文件");
    }
    QString choiceDir = item->text();
    INFO << "删除: " << QString(m_curPath + choiceDir);
    std::string choicePath = QString(m_curPath + choiceDir).toStdString();
    int ret = QMessageBox::question(this, "删除", "确定要删除文件吗, 删除不可恢复");
    if(ret == QMessageBox::No) {
        return;
    }

    PDU* pdu = makePDU(choicePath.size());
    pdu->msgType = EnMsgType::DEL_FILE_MSG;
    std::string curname = m_userName.toStdString();
    memcpy(pdu->data, curname.c_str(), curname.size());
    memcpy(pdu->msg, choicePath.c_str(), choicePath.size());
    Client::getInstance().sendPDU(pdu);
    free(pdu);
}

void FileForm::on_renamePB_clicked()
{
    auto item = ui->fileListLW->currentItem();
    if(item == nullptr) return;
    std::string choiceFile = item->text().toStdString();

    std::string newName =  QInputDialog::getText(this, "新建文件夹", "输入文件夹名").toStdString();

    if(newName.empty() || newName.size() >= 32) {
        QMessageBox::information(this,  "重命名", "名称长度不合法");
    }
    std::string curname = m_userName.toStdString();
    std::string curpath = m_curPath.toStdString()  + choiceFile;

    PDU* pdu = makePDU(curpath.size());
    pdu->msgType = EnMsgType::RENAME_FILE_MSG;
    memcpy(pdu->data, curname.c_str(), curname.size());
    memcpy(pdu->data + 32, newName.c_str(), newName.size());
    memcpy(pdu->msg, curpath.c_str(), curpath.size());
    Client::getInstance().sendPDU(pdu);
    free(pdu);
}

void FileForm::on_fileListLW_itemDoubleClicked(QListWidgetItem *item)
{

    if(item == nullptr) return;
    if(item->icon().themeName() != "dir") {
       return;
    }
    m_curPath += item->text() + "/";
    sendGetFilesRequest();
}

void FileForm::on_returnPB_clicked()
{
    INFO << "m_curPath" << m_curPath;
    if(m_curPath.isEmpty()) {
        return;
    }
    int idx = m_curPath.lastIndexOf("/");
    if(idx == 0) return;
    m_curPath.remove(idx, m_curPath.size() - idx);

    sendGetFilesRequest();

}

void FileForm::on_movePB_clicked()
{

    QListWidgetItem* item = ui->fileListLW->currentItem();


    if(ui->movePB->text() == "剪切") {

        if(item == nullptr) {
            QMessageBox::information(this,  "删除", "请选择文件");
            return;
        }
        m_moveFilePath = m_curPath;
        m_moveFileName = item->text();
        ui->movePB->setText("粘贴/取消");
        return;
    }

    QString newFilePathName;
    QString oldFilePathName = m_moveFilePath + "/" + m_moveFileName;
    if(item == nullptr) {
        newFilePathName = m_curPath + "/" + m_moveFileName;
    } else {
        newFilePathName = m_curPath + "/" + item->text() + m_moveFileName;
    }

    int ret = QMessageBox::question(this, "粘贴/取消", "是否粘贴文件");
    ui->movePB->setText("剪切");
    if(ret == QMessageBox::No) return;
    std::string newfilename = newFilePathName.toStdString();
    std::string oldfilename = oldFilePathName.toStdString();
    std::string curname = m_curPath.toStdString();
    PDU* pdu = makePDU(newfilename.size() + oldfilename.size() + 1);
    pdu->msgType = EnMsgType::MOVE_FILE_MSG;

    int oldSize = oldfilename.size();

    memcpy(pdu->data, curname.c_str(), curname.size());
    memcpy(pdu->data + 32, &oldSize, sizeof(oldSize));
    memcpy(pdu->msg, oldfilename.c_str(), oldfilename.size());
    memcpy(pdu->msg + oldfilename.size() + 1, newfilename.c_str(), newfilename.size());

    Client::getInstance().sendPDU(pdu);
    free(pdu);

}
