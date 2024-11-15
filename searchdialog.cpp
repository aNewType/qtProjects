#include "searchdialog.h"
#include "ui_searchdialog.h"
#include <QMessageBox>

SearchDialog::SearchDialog(QWidget *parent, QPlainTextEdit * textEdit) :
    QDialog(parent),
    ui(new Ui::SearchDialog)
{
    ui->setupUi(this);
    pTextEdit = textEdit;
    ui->btnDown->setChecked(true);
}

SearchDialog::~SearchDialog()
{
    delete ui;
}

void SearchDialog::on_btnSearchNext_clicked()
{
    QString target = ui->searchText->text();

    if(target == "" || pTextEdit == nullptr)
        return ;

    QString text = pTextEdit->toPlainText();
    QTextCursor c = pTextEdit->textCursor();
    int index = -1;

    if(ui->btnDown->isChecked())
    {
        ui->btnUp->setChecked(false);
        index = text.indexOf(target, c.position(),
                             ui->cbCaseSensetive->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);
        if(index >= 0)
        {
            c.setPosition(index);
            c.setPosition(index + target.length(), QTextCursor::KeepAnchor);
            pTextEdit->setTextCursor(c);
        }
    }
    else if(ui->btnUp->isChecked())
    {
        ui->btnDown->setChecked(false);
        index = text.lastIndexOf(target, c.position() - text.length() - 1,
                                 ui->cbCaseSensetive->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);
        if(index >= 0)
        {
            c.setPosition(index + target.length());
            c.setPosition(index, QTextCursor::KeepAnchor);
            pTextEdit->setTextCursor(c);
        }
    }

    if(index < 0)
    {
        QMessageBox msg(this);
        msg.setWindowTitle("查找事项注意");
        msg.setText(QString("找不到") + target);
        msg.setWindowFlag(Qt::Drawer);
        msg.setIcon(QMessageBox::Information);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
    }

}


void SearchDialog::on_btnCancel_clicked()
{
    accept();
}
