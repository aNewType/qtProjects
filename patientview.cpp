#include "patientview.h"
#include "ui_patientview.h"
#include "idatabase.h"

PatientView::PatientView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientView)
{
    ui->setupUi(this);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setAlternatingRowColors(true);
    IDataBase &iDatabase = IDataBase::getInstance();
    if(iDatabase.initPatientModel())
    {
        ui->tableView->setModel(iDatabase.patientTableModel);
        ui->tableView->setSelectionModel(iDatabase.thePatientSelection);
    }
}

PatientView::~PatientView()
{
    delete ui;
}

void PatientView::on_btnAdd_clicked()
{
    int curRow = IDataBase::getInstance().addNewPatient();
    emit goPatientEditView(curRow);
}


void PatientView::on_btnSearch_clicked()
{
    QString filter = QString("name like '%%1%'").arg(ui->txtSearch->text());
    IDataBase::getInstance().searchPatient(filter);
}


void PatientView::on_btnDelete_clicked()
{
    IDataBase::getInstance().deleteCurrentPatient();
}


void PatientView::on_btnEdit_clicked()
{
    QModelIndex curIndex = IDataBase::getInstance().thePatientSelection->currentIndex();    // 获取当前选择的单元格的模型索引

    emit goPatientEditView(curIndex.row());
}

