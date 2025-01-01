#include "chatserver.h"
#include "serverworker.h"
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>

ChatServer::ChatServer(QObject *parent):
    QTcpServer(parent)
{

}

void ChatServer::incomingConnection(qintptr socketDescriptor)
{
    ServerWorker *worker = new ServerWorker(this);
    if(!worker->setSocketDescriptor(socketDescriptor))
    {
        worker->deleteLater();
        return;
    }

    connect(worker, &ServerWorker::logMessage, this, &ChatServer::logMessage);
    connect(worker, &ServerWorker::jsonReceived, this, &ChatServer::jsonReceived);
    connect(worker, &ServerWorker::disconnectedFromClient, this, std::bind(&ChatServer::userDisconnected, this, worker));
    m_clients.append(worker);
    emit logMessage("新的用户连接上了");
}

void ChatServer::broadcast(const QJsonObject &message, ServerWorker *exclude)
{
    for(ServerWorker *worker : m_clients)
    {
        worker->sendJson(message);
    }
}

void ChatServer::handleAdminCommand(const QJsonObject &message)
{
    QString typeVal = message["type"].toString();
    QString command = message["command"].toString();
    QString targetUser = message["targetUser"].toString();

    if (typeVal != "adminCommand") {
        return;
    }

    if (command == "kick") {
        kickUser(targetUser);
    } else if (command == "ban") {
        banUser(targetUser);
    }
}

void ChatServer::kickUser(const QString &username)
{
    // 查找目标用户
    ServerWorker *targetWorker = nullptr;
    for (ServerWorker *worker : m_clients) {
        if (worker->userName() == username) {
            targetWorker = worker;
            break;
        }
    }

    if (targetWorker) {
        // 找到了目标用户，关闭连接并从列表中移除
        m_clients.removeAll(targetWorker);

        // 关闭与目标用户的连接
        ServerWorker *deleteUser = new ServerWorker;
        deleteUser->setUserName(username);
        userDisconnected(deleteUser);

        qDebug() << "User" << username << "has been kicked from the server.";

        // 可选：通知其他客户端目标用户已被踢出
        foreach (ServerWorker *sender, m_clients) {
            QJsonObject userListMessage;
            userListMessage["type"] = "userList";
            QJsonArray userList;
            for(ServerWorker *worker : m_clients)
            {
                if(worker == sender)
                    userList.append(worker->userName() + "*");
                else
                    userList.append(worker->userName());
            }
            userListMessage["userList"] = userList;
            qDebug() << userList;
            sender->sendJson(userListMessage);
            emit updateUserList(m_clients);
        }
    } else {
        qDebug() << "User not found: " << username;
    }
}

void ChatServer::banUser(const QString &username)
{
    ServerWorker *bannedUser = nullptr;
    // 查找对应用户名的 ServerWorker*
    for (ServerWorker *worker : m_clients) {
        if (worker->userName() == username) {
            bannedUser = worker;
            break;
        }
    }
    bannedUser->setUserName(username);
    if (!m_mutedUsers.contains(bannedUser)) {
        m_mutedUsers.append(bannedUser);  // 添加到禁言列表
        qDebug() << "User" << username << "has been muted.";

        // 通知目标用户他们已被禁言
        for (ServerWorker *worker : m_clients) {
            if (worker->userName() == username) {
                ServerWorker *sendbanned = worker;
                QJsonObject message;
                message["type"] = "ban";
                message["text"] = "You have been muted by the admin.";
                message["userName"] = bannedUser->userName();
                sendbanned->sendJson(message);
                break;
            }
        }
    } else {
        qDebug() << "User" << username << "is already muted.";
    }
}

void ChatServer::cancelbanUser(const QString &username)
{
    ServerWorker* targetWorker = nullptr;
    // 查找对应用户名的 ServerWorker*
    for (ServerWorker *worker : m_mutedUsers) {
        if (worker->userName() == username) {
            targetWorker = worker;
            break;
        }
    }

    // 如果找到了对应的 ServerWorker*，就从禁言列表中移除
    if (targetWorker) {
        m_mutedUsers.removeOne(targetWorker);  // 从禁言列表中移除该指针
        qDebug() << "User" << username << "has been unmuted.";

        // 通知目标用户他们已被禁言
        for (ServerWorker *worker : m_clients) {
            if (worker->userName() == username) {
                ServerWorker *sendbanned = worker;
                QJsonObject message;
                message["type"] = "unban";
                message["text"] = "You have been unmuted by the admin.";
                message["userName"] = targetWorker->userName();
                sendbanned->sendJson(message);
                break;
            }
        }
    } else {
        qDebug() << "User" << username << "is not muted.";
    }
}

void ChatServer::stopServer()
{
    close();
}

void ChatServer::jsonReceived(ServerWorker *sender, const QJsonObject &docObj)
{
    const QJsonValue typeVal = docObj.value("type");
    if(typeVal.isNull() || !typeVal.isString())
        return;
    if(typeVal.toString().compare("message", Qt::CaseInsensitive) == 0)
    {
        const QJsonValue textVal = docObj.value("text");
        if(textVal.isNull() || !textVal.isString())
            return;
        const QString text = textVal.toString().trimmed();
        if(text.isEmpty())
            return;
        QJsonObject message;
        message["type"] = "message";
        message["text"] = text;
        message["sender"] = sender->userName();

        broadcast(message, sender);
    }
    else if(typeVal.toString().compare("privateMessage", Qt::CaseInsensitive) == 0)
    {
        const QJsonValue textVal = docObj.value("text");
        const QString receiverVal = docObj.value("receiver").toString();
        if(textVal.isNull() || !textVal.isString())
            return;
        const QString text = textVal.toString().trimmed();
        if(text.isEmpty())
            return;
        QJsonObject message;
        message["type"] = "privateMessage";
        message["text"] = text;
        message["sender"] = sender->userName();
        message["receiver"] = receiverVal;

        broadcast(message, sender);
    }
    else if(typeVal.toString().compare("login", Qt::CaseInsensitive) == 0)
    {
        const QJsonValue userNameVal = docObj.value("text");
        if(userNameVal.isNull() || !userNameVal.isString())
            return;

        sender->setUserName(userNameVal.toString());
        QJsonObject connectMessage;
        connectMessage["type"] = "newUser";
        connectMessage["userName"] = userNameVal.toString();

        broadcast(connectMessage, sender);

        // send user list to new logined user
        QJsonObject userListMessage;
        userListMessage["type"] = "userList";
        QJsonArray userList;
        for(ServerWorker *worker : m_clients)
        {
            if(worker == sender)
                userList.append(worker->userName() + "*");
            else
                userList.append(worker->userName());
        }
        userListMessage["userList"] = userList;
        qDebug() << userList;
        sender->sendJson(userListMessage);
        emit updateUserList(m_clients);
    }
}

void ChatServer::userDisconnected(ServerWorker *sender)
{
    m_clients.removeAll(sender);
    const QString userName = sender->userName();
    if(!userName.isEmpty())
    {
        QJsonObject disconnectedMessage;
        disconnectedMessage["type"] = "userDisconnected";
        disconnectedMessage["userName"] = userName;
        broadcast(disconnectedMessage, nullptr);
        emit logMessage(userName + " disconnected");
    }
    sender->deleteLater();
    emit updateUserList(m_clients);
}
