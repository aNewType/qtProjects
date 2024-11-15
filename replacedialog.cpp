#include "replacedialog.h"
#include "ui_replacedialog.h"

#include <QMessageBox>

ReplaceDialog::ReplaceDialog(QWidget *parent, QPlainTextEdit * textEdit) :
    QDialog(parent),
    ui(new Ui::ReplaceDialog)
{
    ui->setupUi(this);
    pTextEdit = textEdit;
    ui->btnDown->setChecked(true);
}

ReplaceDialog::~ReplaceDialog()
{
    delete ui;
}

void ReplaceDialog::on_btnSearchNext_clicked()
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
                             ui->btnCaseSensitive->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);
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
                                 ui->btnCaseSensitive->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);
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


void ReplaceDialog::on_btnReplace_clicked()
{
    QString target = ui->searchText->text();
    QString toPlace = ui->replaceText->text();
    QString text = pTextEdit->toPlainText();

    if((target == "") || (pTextEdit == nullptr) || (toPlace == ""))
        return ;

    QString selText = pTextEdit->textCursor().selectedText();

    if(selText == target)
        pTextEdit->insertPlainText(toPlace);
    on_btnSearchNext_clicked();
}


void ReplaceDialog::on_btnReplaceAll_clicked()
{
    QString target = ui->searchText->text();
    QString toPlace = ui->replaceText->text();
    QString text = pTextEdit->toPlainText();

    if((target == "") || (pTextEdit == nullptr) || (toPlace == ""))
        return ;

    QString allOfText = pTextEdit->toPlainText();

    allOfText.replace(target, toPlace, ui->btnCaseSensitive->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);
    pTextEdit->clear();
    pTextEdit->insertPlainText(allOfText);
}


void ReplaceDialog::on_pushButton_4_clicked()
{
    accept();
}

