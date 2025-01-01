#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QObject>
#include <QTcpServer>
#include "serverworker.h"

class ChatServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit ChatServer(QObject *parent = nullptr);

protected:
    void incomingConnection(qintptr socketDescriptor) override;
    QVector<ServerWorker *>m_clients;
    QVector<ServerWorker *>m_mutedUsers;

    void broadcast(const QJsonObject &message, ServerWorker *exclude);
    void handleAdminCommand(const QJsonObject &message);
    void kickUser(const QString &username);
    void banUser(const QString &username);
    void cancelbanUser(const QString &username);

signals:
    void logMessage(const QString &msg);
    void updateUserList(const QVector<ServerWorker *>m_clients);

public slots:
    void stopServer();
    void jsonReceived(ServerWorker *sender, const QJsonObject &docObj);
    void userDisconnected(ServerWorker *sender);

};

#endif // CHATSERVER_H
