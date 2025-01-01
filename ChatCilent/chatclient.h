#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QJsonObject>
#include <QJsonDocument>

class ChatClient : public QObject
{
    Q_OBJECT
public:
    explicit ChatClient(QObject *parent = nullptr);
    bool isAdmin() const;


signals:
    void connected();
    void messageReceived(const QString &text);
    void jsonReceived(const QJsonObject &docObj);
    void userMuted(bool muted);

private:
    QTcpSocket *m_clientSocket;
    bool m_isAdmin;
    void setAdminStatus(bool isAdmin);

public slots:
    void onReadyRead();
    void adminStatus(bool onlineStatus);
    void sendMessage(const QString &text, const QString &type = "message");
    void sendPrivateMessage(const QString &text, const QString &type = "privateMessage", const QString &receiver = "");
    void sendAdminCommand(const QString &command, const QString &targetUser);
    void connectToServer(const QHostAddress &Address, quint16 port);
    void disconnectFromHost();
};

#endif // CHATCLIENT_H
