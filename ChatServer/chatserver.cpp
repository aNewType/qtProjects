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
    m_cilents.append(worker);
    emit logMessage("新的用户连接上了");
}

void ChatServer::broadcast(const QJsonObject &message, ServerWorker *exclude)
{
    for(ServerWorker *worker : m_cilents)
    {
        worker->sendJson(message);
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
        for(ServerWorker *worker : m_cilents)
        {
            if(worker == sender)
                userList.append(worker->userName() + "*");
            else
                userList.append(worker->userName());
        }
        userListMessage["userList"] = userList;
        sender->sendJson(userListMessage);

    }
}

void ChatServer::userDisconnected(ServerWorker *sender)
{
    m_cilents.removeAll(sender);
    const QString userName = sender->userName();
    if(!userName.isEmpty())
    {
        QJsonObject disconnectedMessage;
        disconnectedMessage["type"] = "userDisconected";
        disconnectedMessage["userName"] = userName;
        broadcast(disconnectedMessage, nullptr);
        emit logMessage(userName + "disconnected");
    }
    sender->deleteLater();
}
