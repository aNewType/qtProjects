#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "chatclient.h"
#include <QListWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_loginButton_clicked();

    void on_sayButton_clicked();

    void on_logOutButton_clicked();

    void connectToServer();
    void messageReceived(const QString &sender, const QString &text);
    void jsonReceived(const QJsonObject &docObj);
    void userJoined(const QString &user);
    void userLeft(const QString &user);
    void userListReceived(const QStringList &list);
    void onUserMuted(bool muted);

    void on_userListWidget_itemClicked(QListWidgetItem *item);

    void on_userListWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_kickButton_clicked();

    void on_banButton_clicked();

    void on_unbanButton_clicked();

    void on_findButton_clicked();

private:
    Ui::MainWindow *ui;
    QString selectedUser;
    ChatClient *m_chatClient;

    void updateAdminControls();
    void setAdminButton(bool adminStatus);

signals:
    void adminStatus(bool onlineStatus);
};
#endif // MAINWINDOW_H
