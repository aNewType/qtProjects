#ifndef IDATABASE_H
#define IDATABASE_H

#include <QObject>
#include <QtSql>
#include <QSqlDatabase>
#include <QDataWidgetMapper>

class IDataBase : public QObject
{
    Q_OBJECT
public:

    static IDataBase &getInstance()
    {
        static IDataBase instance;
        return instance;
    }

    QString userLogin(QString userName, QString password);

private:
    explicit IDataBase(QObject *parent = nullptr);
    IDataBase(IDataBase const &) = delete;
    void operator=(IDataBase const &) = delete;

    QSqlDatabase database;

    void ininDatabase();

signals:


public:
    bool initPatientModel();
    int addNewPatient();
    bool searchPatient(QString filter);
    void deleteCurrentPatient();
    bool submitPatientEdit();
    void revertPatientEdit();

    QSqlTableModel *patientTableModel; // 数据模型
    QItemSelectionModel *thePatientSelection;  // 选择模型
};

#endif // IDATABASE_H
