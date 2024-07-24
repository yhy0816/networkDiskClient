#ifndef FILEFORM_H
#define FILEFORM_H

#include <QListWidget>
#include <QWidget>
#include "protocol.h"
namespace Ui {
class FileForm;
}

class FileForm : public QWidget
{
    Q_OBJECT

public:
    explicit FileForm(QWidget *parent = nullptr);
    ~FileForm();
    void refreshList(const QList<FileInfo*>& fileList);
    void sendGetFilesRequest();
private slots:
    void on_makeDIrPB_clicked();

    void on_pushButton_clicked();

    void on_refreshFilePB_clicked();

    void on_delDirPB_clicked();

    void on_delFilePB_clicked();

    void on_renamePB_clicked();

    void on_fileListLW_doubleClicked(const QModelIndex &index);

    void on_fileListLW_itemDoubleClicked(QListWidgetItem *item);

    void on_returnPB_clicked();

    void on_movePB_clicked();

    void on_uploadFilePB_clicked();

private:
    QString m_curPath;
    QString m_userName;
    QString m_moveFilePath;
    QString m_moveFileName;
    QList<QListWidgetItem*> m_fileWidgetList;

    bool m_uploading;
    QString m_uploadingFileName;
    Ui::FileForm *ui;
};

#endif // FILEFORM_H
