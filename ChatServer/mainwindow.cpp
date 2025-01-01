#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_ChatServer = new ChatServer(this);
    connect(m_ChatServer, &ChatServer::logMessage, this, &MainWindow::logMessage);
    connect(m_ChatServer, &ChatServer::updateUserList, this, &MainWindow::updateUserList);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startStopButton_clicked()
{
    if (m_ChatServer->isListening())
    {
        m_ChatServer->stopServer();
        ui->startStopButton->setText("启动服务器");
        logMessage("服务器已停止");
    }
    else
    {
        if (!m_ChatServer->listen(QHostAddress::Any, 1967))
        {
            QMessageBox::critical(this, "错误", "无法启动服务器");
            return;
        }

        logMessage("服务器已经启动");
        ui->startStopButton->setText("停止服务器");
    }


}

void MainWindow::logMessage(const QString &msg)
{
    ui->logEdit->appendPlainText(msg);
}

void MainWindow::updateUserList(const QVector<ServerWorker *> m_clients)
{
    QStringList list;
    foreach (ServerWorker * worker, m_clients) {
        list << worker->userName();
    }
    ui->usersListWidget->clear();
    ui->usersListWidget->addItems(list);
}


