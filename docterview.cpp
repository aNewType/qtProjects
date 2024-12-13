#include "docterview.h"
#include "ui_docterview.h"

DocterView::DocterView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DocterView)
{
    ui->setupUi(this);
}

DocterView::~DocterView()
{
    delete ui;
}
