#ifndef CHATFORM_H
#define CHATFORM_H

#include <QWidget>
#include <QTextEdit>

namespace Ui {
class ChatForm;
}

class ChatForm : public QWidget
{
    Q_OBJECT

public:
    explicit ChatForm(QWidget *parent = nullptr);
    ~ChatForm();
    void setFriendName(const QString& name) {
        m_friendName = name;
        this->setWindowTitle(QString("与 %1 的聊天").arg(m_friendName));
    }
    void setMsg(const QString &userName, const QString& msg);
private slots:

    void on_sendPB_clicked();

private:
    Ui::ChatForm *ui;
    QString m_friendName;
};

#endif // CHATFORM_H
