#include "paticenteditview.h"
#include "ui_paticenteditview.h"
#include "idatabase.h"
#include <QSqlTableModel>

PaticentEditView::PaticentEditView(QWidget *parent, int index) :
    QWidget(parent),
    ui(new Ui::PaticentEditView)
{
    ui->setupUi(this);

    dataMapper = new QDataWidgetMapper();
    QSqlTableModel *tabModel = IDataBase::getInstance().patientTableModel;
    dataMapper->setModel(IDataBase::getInstance().patientTableModel);
    dataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);

    dataMapper->addMapping(ui->dbEditID, tabModel->fieldIndex("ID"));
    dataMapper->addMapping(ui->dbEditName, tabModel->fieldIndex("NAME"));
    dataMapper->addMapping(ui->dbEditIDCard, tabModel->fieldIndex("ID_CARD"));
    dataMapper->addMapping(ui->dbSpinHeight, tabModel->fieldIndex("HEIGHT"));
    dataMapper->addMapping(ui->dbSpinWeight, tabModel->fieldIndex("WEIGHT"));
    dataMapper->addMapping(ui->dbEditMobile, tabModel->fieldIndex("MOBILEPHONE"));
    dataMapper->addMapping(ui->dbDateEditDOB, tabModel->fieldIndex("DOB"));
    dataMapper->addMapping(ui->dbComboxSex, tabModel->fieldIndex("SEX"));
    dataMapper->addMapping(ui->dbEditTimeStamp, tabModel->fieldIndex("CREATEDTIMESTAMP"));

    dataMapper->setCurrentIndex(index);
}

PaticentEditView::~PaticentEditView()
{
    delete ui;
}

void PaticentEditView::on_btnSave_clicked()
{
    IDataBase::getInstance().submitPatientEdit();
    emit goPreviousView();
}


void PaticentEditView::on_btnCancel_clicked()
{
    IDataBase::getInstance().revertPatientEdit();
    emit goPreviousView();
}

