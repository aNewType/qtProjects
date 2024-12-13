#include "masterview.h"
#include "ui_masterview.h"
#include "idatabase.h"

MasterView::MasterView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MasterView)
{
    ui->setupUi(this);

    // this->setWindowFlag(Qt::FramelessWindowHint);

    goLoginView();

    IDataBase::getInstance();
}

MasterView::~MasterView()
{
    delete ui;
}

void MasterView::goLoginView()
{
    loginView = new LoginView(this);
    pushWidgetToStackView(loginView);

    connect(loginView, SIGNAL(loginSuccessed()), this, SLOT(goWelcomeView()));
}

void MasterView::goWelcomeView()
{
    welcomeView = new WelcomeView(this);
    pushWidgetToStackView(welcomeView);

    connect(welcomeView, SIGNAL(goDoctorView()), this, SLOT(goDoctorView()));
    connect(welcomeView, SIGNAL(goPatientView()), this, SLOT(goPatientView()));
    connect(welcomeView, SIGNAL(goDepartmentView()), this, SLOT(goDepartmentView()));
}

void MasterView::goDoctorView()
{
    docterView = new DocterView(this);
    pushWidgetToStackView(docterView);
}

void MasterView::goDepartmentView()
{
    departmentView = new DepartmentView(this);
    pushWidgetToStackView(departmentView);
}

void MasterView::goPatientEditView(int rowNo)
{
    patientEditView = new PaticentEditView(this, rowNo);
    pushWidgetToStackView(patientEditView);

    connect(patientEditView, SIGNAL(goPreviousView()), this, SLOT(goPreviousView()));

}
void MasterView::goPatientView()
{
    patientView = new PatientView(this);
    pushWidgetToStackView(patientView);

    connect(patientView, SIGNAL(goPatientEditView(int)), this, SLOT(goPatientEditView(int)));
}

void MasterView::goPreviousView()
{
    int count = ui->stackedWidget->count();

    if(count > 1)
    {
        ui->stackedWidget->setCurrentIndex(count - 2);
        ui->labelTab->setText(ui->stackedWidget->currentWidget()->windowTitle());

        QWidget *widget = ui->stackedWidget->widget(count - 1);
        ui->stackedWidget->removeWidget(widget);
        delete widget;
    }
}

void MasterView::pushWidgetToStackView(QWidget *widget)
{
    ui->stackedWidget->addWidget(widget);
    int count = ui->stackedWidget->count();
    ui->stackedWidget->setCurrentIndex(count - 1);  // 总是显示最新加入的view
    ui->labelTab->setText(widget ->windowTitle());
}


void MasterView::on_btnBack_clicked()
{
    goPreviousView();
}


void MasterView::on_stackedWidget_currentChanged(int arg1)
{
    int count = ui->stackedWidget->count();
    if(count > 1)
        ui->btnBack->setEnabled(true);
    else
        ui->btnBack->setEnabled(false);

    QString title = ui->stackedWidget->currentWidget()->windowTitle();
    if(title == "Welcome")
    {
        ui->btnBack->setEnabled(false);
        ui->btnLogOut->setEnabled(true);
        ui->btnBack->hide();
    }
    else if(title == "Logining")
    {
        ui->btnBack->setEnabled(false);
        ui->btnLogOut->setEnabled(false);
        ui->btnBack->hide();
        ui->btnLogOut->hide();
    }
    else
    {
        ui->btnLogOut->setEnabled(false);
    }
}


void MasterView::on_btnLogOut_clicked()
{
    goPreviousView();
}

