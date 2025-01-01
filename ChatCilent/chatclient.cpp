#include "chatclient.h"

ChatClient::ChatClient(QObject *parent)
    : QObject{parent}
{
    m_clientSocket = new QTcpSocket(this);

    connect(m_clientSocket, &QTcpSocket::connected, this, &ChatClient::connected);
    connect(m_clientSocket, &QTcpSocket::readyRead, this, &ChatClient::onReadyRead);
}

void ChatClient::setAdminStatus(bool isAdmin)
{
    m_isAdmin = isAdmin;
}

bool ChatClient::isAdmin() const
{
    return m_isAdmin;
}

void ChatClient::onReadyRead()
{
    QByteArray jsonData;
    QDataStream socketStream(m_clientSocket);
    socketStream.setVersion(QDataStream::Qt_6_7);

    // start an infinite loop
    for(;;)
    {
        socketStream.startTransaction();
        socketStream >> jsonData;
        if (socketStream.commitTransaction())
        {
            // emit messageReceived(QString::fromUtf8(jsonData));
            // sendMessage("I received Message");
            QJsonParseError parseError;
            const QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);
            if(parseError.error == QJsonParseError::NoError)
            {
                if(jsonDoc.isObject())  // and is a JSON object
                {
                    // emit logMessage(QJsonDocument(jsonDoc).toJson(QJsonDocument::Compact));
                    emit jsonReceived(jsonDoc.object());  // parse the JSON
                }
            }
        }
        else
        {
            break;
        }
    }
}

void ChatClient::adminStatus(bool onlineStatus)
{
    if(onlineStatus == true)
        setAdminStatus(true);
    else
        setAdminStatus(false);
}

void ChatClient::sendMessage(const QString &text, const QString &type)
{
    if(m_clientSocket->state() != QAbstractSocket::ConnectedState)
    {
        qDebug() << "It seems not connect";
        return;
    }

    if(!text.isEmpty())
    {
        // create a QDataStream operating on the socket
        QDataStream serverStream(m_clientSocket);
        serverStream.setVersion(QDataStream::Qt_6_7);

        // create the JSON we want to send
        QJsonObject message;
        message["type"] = type;
        message["text"] = text;

        // send the JSON using QDataStream
        serverStream << QJsonDocument(message).toJson();
    }
}

void ChatClient::sendPrivateMessage(const QString &text, const QString &type, const QString &receiver)
{
    if(m_clientSocket->state() != QAbstractSocket::ConnectedState)
    {
        qDebug() << "It seems not connect";
        return;
    }

    if(!text.isEmpty())
    {
        // create a QDataStream operating on the socket
        QDataStream serverStream(m_clientSocket);
        serverStream.setVersion(QDataStream::Qt_6_7);

        // create the JSON we want to send
        QJsonObject message;
        message["type"] = type;
        message["text"] = text;
        message["receiver"] = receiver;

        // send the JSON using QDataStream
        serverStream << QJsonDocument(message).toJson();
    }
}

void ChatClient::sendAdminCommand(const QString &command, const QString &targetUser)
{
    if (m_clientSocket->state() != QAbstractSocket::ConnectedState) {
        qDebug() << "It seems not connect";
        return;
    }

    // Create the message in JSON format
    QJsonObject message;
    message["type"] = "adminCommand";  // 标记为管理员命令
    message["command"] = command;      // 传递命令类型
    message["targetUser"] = targetUser;  // 传递目标用户名

    // Send the JSON message
    QDataStream serverStream(m_clientSocket);
    serverStream.setVersion(QDataStream::Qt_6_7);
    serverStream << QJsonDocument(message).toJson();
}

void ChatClient::connectToServer(const QHostAddress &Address, quint16 port)
{
    m_clientSocket->connectToHost(Address, port);
}

void ChatClient::disconnectFromHost()
{
    m_clientSocket->disconnectFromHost();
}
