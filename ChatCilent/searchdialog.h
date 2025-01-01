#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QTextEdit>

namespace Ui {
class searchDialog;
}

class searchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit searchDialog(QWidget *parent = nullptr, QTextEdit *textEdit = nullptr);
    ~searchDialog();

private slots:
    void on_btnSearchNext_clicked();

    void on_btnCancel_clicked();

private:
    Ui::searchDialog *ui;
    QTextEdit *pTextEdit;
};

#endif // SEARCHDIALOG_H
