#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "searchdialog.h"
#include <QHostAddress>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->loginPage);
    m_chatClient = new ChatClient;

    selectedUser = "";
    ui->titleLabel->setText("欢迎使用聊天室");
    connect(m_chatClient, &ChatClient::connected, this, &MainWindow::connectToServer);
    // connect(m_chatClient, &ChatClient::messageReceived, this, &MainWindow::messageReceived);
    connect(m_chatClient, &ChatClient::jsonReceived, this, &MainWindow::jsonReceived);
    connect(this, &MainWindow::adminStatus, m_chatClient, &ChatClient::adminStatus);
    connect(m_chatClient, &ChatClient::userMuted, this, &MainWindow::onUserMuted);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loginButton_clicked()
{
    m_chatClient->connectToServer(QHostAddress(ui->serverEdit->text()), 1967);
    ui->titleLabel->setText(ui->userNameEdit->text() + "的聊天室");
    if(ui->userNameEdit->text() == "admin")
    {
        setAdminButton(true);
        emit adminStatus(true);
    }
    else
    {
        setAdminButton(false);
        emit adminStatus(false);
    }
}


void MainWindow::on_sayButton_clicked()
{
    if(!ui->sayLineEdit->text().isEmpty())
    {
        if(selectedUser.isEmpty())
        {
            m_chatClient->sendMessage(ui->sayLineEdit->text());
        }
        else
        {
            m_chatClient->sendPrivateMessage(ui->sayLineEdit->text(), "privateMessage", selectedUser);
            messageReceived("你[私发给]" + selectedUser, ui->sayLineEdit->text());
        }

        ui->sayLineEdit->clear();
    }
}


void MainWindow::on_logOutButton_clicked()
{
    m_chatClient->disconnectFromHost();
    ui->stackedWidget->setCurrentWidget(ui->loginPage);

    for(auto aItem: ui->userListWidget->findItems(ui->userNameEdit->text(), Qt::MatchExactly))
    {
        qDebug("remove");
        ui->userListWidget->removeItemWidget(aItem);
        delete aItem;
    }

    ui->titleLabel->setText("欢迎使用聊天室");
}

void MainWindow::connectToServer()
{
    ui->stackedWidget->setCurrentWidget(ui->chatPage);
    m_chatClient->sendMessage(ui->userNameEdit->text(), "login");
}

void MainWindow::messageReceived(const QString &sender, const QString &text)
{
    ui->roomTextEdit->append(QString("%1 : %2").arg(sender).arg(text));
}

void MainWindow::jsonReceived(const QJsonObject &docObj)
{
    const QJsonValue typeVal = docObj.value("type");
    if(typeVal.isNull() || !typeVal.isString())
        return;
    if(typeVal.toString().compare("message", Qt::CaseInsensitive) == 0)
    {
        const QJsonValue textVal = docObj.value("text");
        const QJsonValue senderVal = docObj.value("sender");

        if(textVal.isNull() || !textVal.isString())
            return;
        if(senderVal.isNull() || !senderVal.isString())
            return;

        messageReceived(senderVal.toString(), textVal.toString());

    }
    else if(typeVal.toString().compare("privateMessage", Qt::CaseInsensitive) == 0)
    {
        const QJsonValue textVal = docObj.value("text");
        const QJsonValue senderVal = docObj.value("sender");
        const QJsonValue receiverVal = docObj.value("receiver");

        if(!receiverVal.isNull() && receiverVal.toString() != ui->userNameEdit->text())
            return;
        if(textVal.isNull() || !textVal.isString())
            return;
        if(senderVal.isNull() || !senderVal.isString())
            return;

        if(receiverVal.toString() == ui->userNameEdit->text())
            messageReceived(senderVal.toString() + "[私发]", textVal.toString());

        if(senderVal.toString() == ui->userNameEdit->text())
            messageReceived(senderVal.toString(), textVal.toString());

    }
    else if(typeVal.toString().compare("newUser", Qt::CaseInsensitive) == 0)
    {
        const QJsonValue userNameVal = docObj.value("userName");
        if(userNameVal.isNull() || !userNameVal.isString())
            return;

        userJoined(userNameVal.toString());
    }
    else if(typeVal.toString().compare("userDisconnected", Qt::CaseInsensitive) == 0)
    {
        const QJsonValue userNameVal = docObj.value("userName");
        if(userNameVal.isNull() || !userNameVal.isString())
            return;

        userLeft(userNameVal.toString());
    }
    else if(typeVal.toString().compare("userList", Qt::CaseInsensitive) == 0)
    {
        const QJsonValue userListVal = docObj.value("userList");
        if(userListVal.isNull() || !userListVal.isArray())
            return;

        // 将 QJsonArray 转换为 QVariantList 然后转换为 QStringList
        // QJsonArray userListArray = userListVal.toArray();
        // QStringList userList;
        // for (const QJsonValue &value : userListArray) {
        //     if (value.isString()) {
        //         userList << value.toString();
        //     }
        // }
        // qDebug() << userList;
        // userListReceived(userList);
        qDebug() << userListVal.toVariant().toStringList();
        userListReceived(userListVal.toVariant().toStringList());
    }
    else if(typeVal.toString().compare("ban", Qt::CaseInsensitive) == 0)
    {
        QString text = docObj.value("text").toString();
        qDebug() << "You are unbanned:" << text;

        // 禁用发送消息功能
        emit onUserMuted(false);
    }
    else if(typeVal.toString().compare("unban", Qt::CaseInsensitive) == 0)
    {
        QString text = docObj.value("text").toString();
        qDebug() << "You are banned:" << text;

        // 禁用发送消息功能
        emit onUserMuted(true);
    }
}

void MainWindow::userJoined(const QString &user)
{
    ui->userListWidget->addItem(user);
}

void MainWindow::userLeft(const QString &user)
{
    for(auto aItem: ui->userListWidget->findItems(user, Qt::MatchExactly))
    {
        qDebug("remove");
        ui->userListWidget->removeItemWidget(aItem);
        delete aItem;
    }
}

void MainWindow::userListReceived(const QStringList &list)
{
    ui->userListWidget->clear();
    ui->userListWidget->addItems(list);
}

void MainWindow::onUserMuted(bool muted)
{
    if (muted) {
        ui->sayLineEdit->setEnabled(false);  // 禁用输入框
        ui->sayButton->setEnabled(false);   // 禁用发送按钮
    } else {
        ui->sayLineEdit->setEnabled(true);
        ui->sayButton->setEnabled(true);
    }
}


void MainWindow::on_userListWidget_itemClicked(QListWidgetItem *item)
{
    if (!item)
        return;

    // 记录当前选中的用户
    selectedUser = item->text();
}


void MainWindow::on_userListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    if (!item)
        return;

    // 取消选中当前项
    item->setSelected(false);
    selectedUser = "";
}


void MainWindow::on_kickButton_clicked()
{
    if (selectedUser.isEmpty()) {
        qDebug() << "No user selected!";
        return;
    }

    // 发送踢除命令给服务器
    m_chatClient->sendAdminCommand("kick", selectedUser);
}


void MainWindow::on_banButton_clicked()
{
    if (selectedUser.isEmpty()) {
        qDebug() << "No user selected!";
        return;
    }

    // 发送禁言命令给服务器
    m_chatClient->sendAdminCommand("ban", selectedUser);
}

void MainWindow::updateAdminControls()
{
    if (m_chatClient->isAdmin()) {
        ui->kickButton->setEnabled(true);
        ui->banButton->setEnabled(true);
    } else {
        ui->kickButton->setEnabled(false);
        ui->banButton->setEnabled(false);
    }
}

void MainWindow::setAdminButton(bool adminStatus)
{
    ui->banButton->setEnabled(adminStatus);
    ui->banButton->setHidden(adminStatus);
    ui->kickButton->setEnabled(adminStatus);
    ui->kickButton->setHidden(adminStatus);
    ui->unbanButton->setEnabled(adminStatus);
    ui->unbanButton->setHidden(adminStatus);
    if(adminStatus)
    {
        ui->banButton->show();
        ui->kickButton->show();
        ui->unbanButton->show();
    }
}


void MainWindow::on_unbanButton_clicked()
{
    if (selectedUser.isEmpty()) {
        qDebug() << "No user selected!";
        return;
    }

    // 发送解除禁言命令给服务器
    m_chatClient->sendAdminCommand("unban", selectedUser);
}


void MainWindow::on_findButton_clicked()
{
    qDebug() << "clicked!";
    searchDialog sdl(this, ui->roomTextEdit);
    sdl.exec();
}

