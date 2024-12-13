#ifndef WELCOMEVIEW_H
#define WELCOMEVIEW_H

#include <QWidget>

namespace Ui {
class WelcomeView;
}

class WelcomeView : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomeView(QWidget *parent = nullptr);
    ~WelcomeView();

private slots:
    void on_btnDepartment_clicked();

    void on_btnDoctor_clicked();

    void on_btnPatient_clicked();


signals:
    void goDepartmentView();
    void goDoctorView();
    void goPatientView();

private:
    Ui::WelcomeView *ui;
};

#endif // WELCOMEVIEW_H
