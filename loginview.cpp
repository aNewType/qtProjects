#include "loginview.h"
#include "ui_loginview.h"
#include "idatabase.h"

LoginView::LoginView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginView)
{
    ui->setupUi(this);
}

LoginView::~LoginView()
{
    delete ui;
}

void LoginView::on_btnSignIn_clicked()
{
    QString status = IDataBase::getInstance().userLogin(ui->inputUserName->text(),
                                                        ui->inputUserPassword->text());

    if(status == "loginOK")
        emit loginSuccessed();
}

