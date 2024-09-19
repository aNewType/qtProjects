#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::iniUi()
{
    fLabCurFile = new QLabel;
    fLabCurFile->setMinimumWidth(150);
    fLabCurFile->setText("当前文件: ");
    ui->statusbar->addWidget(fLabCurFile);

    progressBar = new QProgressBar;
    progressBar->setMinimum(5);
    progressBar->setMaximum(50);
    progressBar->setValue(ui->textEdit->font().pointSize());
    ui->statusbar->addWidget(progressBar);

    spinFontSize = new QSpinBox;
    spinFontSize->setMinimum(5);
    spinFontSize->setMaximum(50);
    ui->mainToolBar->addWidget(new QLabel("字体大小"));
    ui->mainToolBar->addWidget(spinFontSize);

    combotFont = new QFontComboBox;
    ui->mainToolBar->addWidget(new QLabel("字体"));
    ui->mainToolBar->addWidget(combotFont);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    iniUi();
    setCentralWidget(ui->textEdit);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actFontBold_triggered(bool checked)
{
    QTextCharFormat fmt;
//    fmt = ui->textEdit->currentCharFormat();
    if(checked)
        fmt.setFontWeight(QFont::Bold);
    else
        fmt.setFontWeight(QFont::Normal);
    ui->textEdit->mergeCurrentCharFormat(fmt);
}


void MainWindow::on_actFontItalic_triggered(bool checked)
{
    QTextCharFormat fmt;
    if(checked)
        fmt.setFontItalic(true);
    else
        fmt.setFontItalic(false);
    ui->textEdit->mergeCurrentCharFormat(fmt);
}


void MainWindow::on_actFontUnder_triggered(bool checked)
{
    QTextCharFormat fmt;
    if(checked)
        fmt.setFontUnderline(true);
    else
        fmt.setFontUnderline(false);
    ui->textEdit->mergeCurrentCharFormat(fmt);
}


void MainWindow::on_textEdit_copyAvailable(bool b)
{
    ui->actCut->setEnabled(b);
    ui->actCopy->setEnabled(b);
    ui->actPaste->setEnabled(ui->textEdit->canPaste());
}


void MainWindow::on_textEdit_selectionChanged()
{
    QTextCharFormat fmt;
    fmt = ui->textEdit->currentCharFormat();
    ui->actFontItalic->setChecked(fmt.fontItalic());
    ui->actFontBold->setChecked(fmt.font().bold());
    ui->actFontUnder->setChecked(fmt.fontUnderline());
}


void MainWindow::on_actShowAuthor_triggered()
{
    QMessageBox::about(this,"ciallo～(∠・ω< )⌒★","姓名：\n学号：");
}

