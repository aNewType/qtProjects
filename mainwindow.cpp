#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 按下数字按钮后将数字添加进显示框
    btnNumbers = {{Qt::Key_0, ui->number0Btn},
                {Qt::Key_1, ui->number1Btn},
                {Qt::Key_2, ui->number2Btn},
                {Qt::Key_3, ui->number3Btn},
                {Qt::Key_4, ui->number4Btn},
                {Qt::Key_5, ui->number5Btn},
                {Qt::Key_6, ui->number6Btn},
                {Qt::Key_7, ui->number7Btn},
                {Qt::Key_8, ui->number8Btn},
                {Qt::Key_9, ui->number9Btn},};

    foreach(auto btn, btnNumbers)connect(btn, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
//    connect(ui->number0Btn, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
//    connect(ui->number1Btn, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
//    connect(ui->number2Btn, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
//    connect(ui->number3Btn, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
//    connect(ui->number4Btn, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
//    connect(ui->number5Btn, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
//    connect(ui->number6Btn, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
//    connect(ui->number7Btn, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
//    connect(ui->number8Btn, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
//    connect(ui->number9Btn, SIGNAL(clicked()), this, SLOT(btnNumClicked()));

    // 其他符号功能
    btnBinaryOps = {{Qt::Key_Plus, ui->addBtn},
                   {Qt::Key_Minus, ui->subtractionBtn},
                   {Qt::Key_multiply, ui->multiplyBtn},
                   {Qt::Key_division, ui->divideBtn},};
    foreach(auto btnB, btnBinaryOps)connect(btnB, SIGNAL(clicked()), this, SLOT(addBinaryOperator()));
    connect(ui->precentBtn, SIGNAL(clicked()), this, SLOT(addUnaryOperator()));         // 添加百分号
    connect(ui->fractionBtn, SIGNAL(clicked()), this, SLOT(addUnaryOperator()));        // 添加分号
    connect(ui->squareBtn, SIGNAL(clicked()), this, SLOT(addUnaryOperator()));          // 添加平方
    connect(ui->rootingBtn, SIGNAL(clicked()), this, SLOT(addUnaryOperator()));         //添加开根
//    connect(ui->addBtn, SIGNAL(clicked()), this, SLOT(addBinaryOperator()));            // 添加加号
//    connect(ui->subtractionBtn, SIGNAL(clicked()), this, SLOT(addBinaryOperator()));    // 添加减号
//    connect(ui->multiplyBtn, SIGNAL(clicked()), this, SLOT(addBinaryOperator()));       // 添加乘号
//    connect(ui->divideBtn, SIGNAL(clicked()), this, SLOT(addBinaryOperator()));         // 添加除号
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::caculation(bool *ok)
{
    double result = 0;

    if (operands.size() == 2 && opCodes.size() > 0)
    {
        // 取操作数
        double operand1 = operands.front();
        operands.pop_front();
        double operand2 = operands.front();
        operands.pop_front();

        // 取操作符
        QString op = opCodes.front();
        opCodes.pop_front();
        if(op == "+")
            result = operand1 + operand2;
        else if(op == "-")
            result = operand2 - operand1;
        else if(op == "×")
            result = operand1 * operand2;
        else if(op == "÷")
        {
            if(operand1 != 0)
                result = operand2 / operand1;
            else
                ui->statusbar->showMessage("除数不能为零！");
        }
        operands.push_back(result);

        ui->statusbar->showMessage(QString("caculation is in progess: operands is %1, opcode is %2").arg(operands.size()).arg(opCodes.size()));
    }
    else
        ui->statusbar->showMessage(QString("operands is %1, opcode is %2").arg(operands.size()).arg(opCodes.size()));

    return QString::number(result);
}

void MainWindow::btnNumClicked()
{
    QString digit = qobject_cast<QPushButton *>(sender())->text();
    if(operand == "0")
        operand = digit;
    else
        operand += digit;
    ui->displayString->setText(operand);
    ui->statusbar->showMessage("the button of number " + qobject_cast<QPushButton *>(sender())->text() + " has been cilcked!");
}

void MainWindow::on_pointBtn_clicked()
{
    if(!operand.contains("."))
    {
        operand += qobject_cast<QPushButton *>(sender())->text();
        ui->statusbar->showMessage("the button of point has been cilcked!");
    }
    else
        ui->statusbar->showMessage("A number has one point at most!");
    ui->displayString->setText(operand);
}

void MainWindow::on_deleteBtn_clicked()
{
    operand = operand.left(operand.length() - 1);
    ui->displayString->setText(operand);
    ui->statusbar->showMessage("the button of delete has been cilcked!");
}

void MainWindow::on_clearBtn_clicked()
{
    operand.clear();
    ui->displayString->setText(operand);
    ui->statusbar->showMessage("the button of clear has been cilcked!");
}

void MainWindow::on_clearEntryBtn_clicked()
{
    while(!operands.isEmpty())
        operands.pop_back();
    while(!opCodes.isEmpty())
        opCodes.pop_back();
    operand.clear();
    ui->displayString->setText(operand);
    ui->statusbar->showMessage("the button of clear empty has been cilcked!");
}

void MainWindow::addBinaryOperator()
{
    opCode = qobject_cast<QPushButton *>(sender())->text();
    qDebug() << opCode;
    if(operand != "")
    {
        operands.push_back(operand.toDouble());
        operand.clear();
        opCodes.push_back(opCode);
        ui->displayString->setText(operand);
        ui->statusbar->showMessage("the button of number " + qobject_cast<QPushButton *>(sender())->text() + " has been cilcked!");
    }

    QString result = caculation();
    ui->displayString->setText(result);
}

void MainWindow::addUnaryOperator()
{
    if(operand != "")
    {
        double result = operand.toDouble();
        operand = "";
        QString op = qobject_cast<QPushButton*>(sender())->text();

        if(op == "%")
            result /= 100.0;
        else if(op == "1/x")
            result = 1.0 / result;
        else if(op == "x²")
            result = result * result;
        else if(op == "√x")
            result = sqrt(result);

        operand = QString::number(result);
        ui->displayString->setText(operand);
    }
}

void MainWindow::on_equalBtn_clicked()
{
    if(operand != "")
    {
        operands.push_back(operand.toDouble());
        operand = "";
    }
    QString result = caculation();
    operand = result;
    ui->displayString->setText(result);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    foreach(auto btnKeys, btnNumbers.keys())
    {
        if(event->key() == btnKeys)
            btnNumbers[btnKeys]->animateClick(100);
    }
    if(event->key() == Qt::Key_Equal)
        ui->equalBtn->animateClick(100);
    else if(event->key() == Qt::Key_Delete)
        ui->deleteBtn->animateClick(100);
    else if(event->key() == Qt::Key_C)
        ui->clearBtn->animateClick(100);
    else if(event->key() == Qt::Key_E)
        ui->clearEntryBtn->animateClick(100);
}

void MainWindow::calculateString()
{
    QStack<double> numbers;
    double sum = 0;
    QString str = ui->displayString->text();
    str += ui->displayString->text();
    QString exp = toPostFix(str);

    for(int i = 0; i < exp.length(); i++)
    {
        if(exp[i] == " ")
            continue;
        else if(isNumberOrPoint(exp[i]))
        {
            QString num = "";
            num += str[i];
            for(int j = i + 1; j < str.length(); j++)
            {
                if(isNumberOrPoint(str[j]))
                    num += str[j];
                else
                {
                    i = j - 1;
                    break;
                }
            }
            numbers.push(num.toDouble());
        }
        else
        {
            double v2 = numbers.top();
            numbers.pop();
            double v1 = numbers.top();
            numbers.pop();

            if(str[i] == "+")
                numbers.push(v1 + v2);
            else if(str[i] == "-")
                numbers.push(v1 - v2);
            else if(str[i] == "×")
                numbers.push(v1 * v2);
            else
                numbers.push(v1 / v2);
        }
    }
    sum = numbers.top();
    numbers.pop();
    QString res = QString::number(sum, 'f', 6);
    ui->displayString->setText(res);
}

bool MainWindow::isNumberOrPoint(QChar charr)
{
    char numbers[11] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.'};
    for(int i = 0; i < 11; i++)
    {
        if(i == 10 && charr != numbers[i])
            return false;
        else if(charr == numbers[i])
            return true;
    }
    return true;
}

QString MainWindow::toPostFix(QString str)
{
    QStack<QChar> operators;
    QString postFix = "";

    for(int i = 0; i < str.length(); i++)
    {
        if(isNumberOrPoint(str[i]))
        {
            QString num = "";
            num += str[i];
            for(int j = i + 1; j < str.length(); j++)
            {
                if(isNumberOrPoint(str[j]))
                    num += str[j];
                else
                {
                    i = j - 1;
                    break;
                }
            }
            if(num[num.length() - 1] == ".")
                num += "0";
            postFix += num + " ";
        }
        else
        {
            while(!operators.isEmpty() && precedence(operators.top()) >= precedence(str[i]))
            {
                postFix += operators.top();
                postFix += " ";
                operators.pop();
            }
            operators.push(str[i]);
        }
    }
    while(!operators.isEmpty())
    {
        postFix += operators.top();
        operators.pop();
    }
    return postFix;
}

int MainWindow::precedence(QChar c)
{
    if(c == "×" || c == "÷")
        return 2;
    else if(c == "+" || c == "-")
        return 1;
    else
        return 0;
}
void MainWindow::on_plusMiunsBtn_clicked()
{
    if(operand == "" || operand == "0")
        operand = operand;
    else if(!operand.contains("-"))
        operand = "-" + operand;
    else
        operand = operand.right(operand.length() - 1);

    ui->displayString->setText(operand);
}
