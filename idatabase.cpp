#include "idatabase.h"
#include <QDebug>
#include <QUuid>

void IDataBase::ininDatabase()
{
    database = QSqlDatabase::addDatabase("QSQLITE");    // 添加SQL LITE数据库驱动
    QString aFile = "D:/QtProjects/Lab3/sqlLab4.db";
    database.setDatabaseName(aFile);   // 设置数据库名称

    if(!database.open())
    {
        qDebug() << "failed to open database.";
    }
    else
    {
        qDebug() << "open database is ok.";
    }

}

bool IDataBase::initPatientModel()
{
    patientTableModel = new QSqlTableModel(this, database);
    patientTableModel->setTable("patient");
    patientTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit); // 数据保存方式，OnManualSubmit, OnRowChange
    patientTableModel->setSort(patientTableModel->fieldIndex("name"), Qt::AscendingOrder);  // 排序
    if(!(patientTableModel->select()))   // 查询数据
        return false;

    thePatientSelection = new QItemSelectionModel(patientTableModel);
    return true;
}

int IDataBase::addNewPatient()
{
    patientTableModel->insertRow(patientTableModel->rowCount(), QModelIndex()); // 在末尾添加一条记录

    QModelIndex curIndex = patientTableModel->index(patientTableModel->rowCount() - 1, 1); // 创建最后一行的ModelIndex

    int curRecNo = curIndex.row();
    QSqlRecord curRec = patientTableModel->record(curRecNo);    // 获取当前记录
    curRec.setValue("CREATEDTIMESTAMP", QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    curRec.setValue("ID", QUuid::createUuid().toString(QUuid::WithoutBraces));

    patientTableModel->setRecord(curRecNo, curRec);

    return curIndex.row();
}

bool IDataBase::searchPatient(QString filter)
{
    patientTableModel->setFilter(filter);
    return patientTableModel->select();
}

void IDataBase::deleteCurrentPatient()
{
    QModelIndex curIndex = thePatientSelection->currentIndex(); // 获取当前选择单元格的模型索引
    patientTableModel->removeRow(curIndex.row());
    patientTableModel->submitAll();
    patientTableModel->select();
}

bool IDataBase::submitPatientEdit()
{
    return patientTableModel->submitAll();
}

void IDataBase::revertPatientEdit()
{
    return patientTableModel->revertAll();
}

QString IDataBase::userLogin(QString userName, QString password)
{
    QSqlQuery query;
    query.prepare("select USERNAME, PASSWORD from user where USERNAME = :USER");
    query.bindValue(":USER", userName);
    query.exec();
    if(query.first() && query.value("USERNAME").isValid())
    {
        QString passwd = query.value("PASSWORD").toString();
        if(passwd == password)
        {
            qDebug() << "input correct user's name or password.";
            return "loginOK";
        }
        else
        {
            qDebug() << "input the error user's name or password.";
            return "wrongPassword";
        }
    }
    else
    {
        qDebug() << "no such user.";
        return "loginFailed";
    }

}

IDataBase::IDataBase(QObject *parent) : QObject(parent)
{
    ininDatabase();

}
