#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStack>
#include <QKeyEvent>
#include <QMap>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString operand;    // 操作数
    QString opCode;     // 操作符
    QStack<double> operands;   // 操作数堆栈
    QStack<QString> opCodes;  // 操作符堆栈
    QMap<int, QPushButton*> btnNumbers;  // 键盘数字
    QMap<int, QPushButton*> btnBinaryOps;   // 双位操作符

    QString caculation(bool *ok = NULL);  // 计算

private slots:
    void btnNumClicked();
    void addBinaryOperator();
    void addUnaryOperator();
    void calculateString();
    bool isNumberOrPoint(QChar charr);
    QString toPostFix(QString str);
    int precedence(QChar c);

    void on_pointBtn_clicked();

    void on_deleteBtn_clicked();

    void on_clearBtn_clicked();

    void on_equalBtn_clicked();

    virtual void keyPressEvent(QKeyEvent *event);

    void on_clearEntryBtn_clicked();

    void on_plusMiunsBtn_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
