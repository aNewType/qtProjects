#ifndef MASTERVIEW_H
#define MASTERVIEW_H

#include <QWidget>
#include "loginview.h"
#include "docterview.h"
#include "departmentview.h"
#include "paticenteditview.h"
#include "patientview.h"
#include "welcomeview.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MasterView; }
QT_END_NAMESPACE

class MasterView : public QWidget
{
    Q_OBJECT

public:
    MasterView(QWidget *parent = nullptr);
    ~MasterView();

public slots:
    void goLoginView();
    void goWelcomeView();
    void goDoctorView();
    void goDepartmentView();
    void goPatientEditView(int rowNo);
    void goPatientView();
    void goPreviousView();


private slots:
    void on_btnBack_clicked();

    void on_stackedWidget_currentChanged(int arg1);

    void on_btnLogOut_clicked();

private:
    void pushWidgetToStackView(QWidget *widget);

    Ui::MasterView *ui;

    WelcomeView *welcomeView;
    DocterView *docterView;
    PatientView *patientView;
    DepartmentView *departmentView;
    PaticentEditView *patientEditView;
    LoginView *loginView;

};
#endif // MASTERVIEW_H
