#ifndef PATICENTEDITVIEW_H
#define PATICENTEDITVIEW_H

#include <QWidget>
#include  <QDataWidgetMapper>

namespace Ui {
class PaticentEditView;
}

class PaticentEditView : public QWidget
{
    Q_OBJECT

public:
    explicit PaticentEditView(QWidget *parent = nullptr, int index = 0);
    ~PaticentEditView();

private slots:
    void on_btnSave_clicked();

    void on_btnCancel_clicked();

private:
    Ui::PaticentEditView *ui;
    QDataWidgetMapper *dataMapper;  //数据映射

signals:
    void goPreviousView();

};

#endif // PATICENTEDITVIEW_H
