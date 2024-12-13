#include "welcomeview.h"
#include "ui_welcomeview.h"

WelcomeView::WelcomeView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WelcomeView)
{
    ui->setupUi(this);
}

WelcomeView::~WelcomeView()
{
    delete ui;
}

void WelcomeView::on_btnDepartment_clicked()
{
    emit goDepartmentView();
}


void WelcomeView::on_btnDoctor_clicked()
{
    emit goDoctorView();
}


void WelcomeView::on_btnPatient_clicked()
{
    emit goPatientView();
}

