#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "searchdialog.h"
#include "replacedialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QColorDialog>
#include <QFontDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    textChange = false;
    on_actionNewFile_triggered();

    statusLabel.setMaximumWidth(180);
    statusLabel.setText("length:" + QString::number(0) + "      lines:     " + QString::number(1));
    ui->statusbar->addPermanentWidget(&statusLabel);

    statusCursorLabel.setMaximumWidth(180);
    statusCursorLabel.setText("Ln:" + QString::number(0) + "      Col: " + QString::number(1));
    ui->statusbar->addPermanentWidget(&statusCursorLabel);

    QLabel *author = new QLabel(ui->statusbar);
    author->setText(tr("ciallo"));
    ui->statusbar->addPermanentWidget(author);

    ui->actionCopy->setEnabled(false);
    ui->actionCut->setEnabled(false);
    ui->actionUndo->setEnabled(false);
    ui->actionRedo->setEnabled(false);
    ui->actionPaste->setEnabled(false);

    QPlainTextEdit::LineWrapMode mode = ui->textEdit->lineWrapMode();
    if(mode == QTextEdit::NoWrap)
    {
        ui->textEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        ui->actionAutoWordWrap->setChecked(false);
    }
    else
    {
        ui->textEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
        ui->actionAutoWordWrap->setChecked(true);
    }

    ui->actionShowBar->setChecked(true);
    ui->actionToolBar->setChecked(true);

    connect(ui->actionShowLine, SIGNAL(triggered(bool)), ui->textEdit, SLOT(hideLineNumberArea(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionAbout_triggered()
{
    AboutDialog adl;
    adl.exec();
}


void MainWindow::on_actionSearch_triggered()
{
    qDebug() << "clicked!";
    SearchDialog sdl(this, ui->textEdit);
    sdl.exec();
}


void MainWindow::on_actionReplace_triggered()
{
    ReplaceDialog rdl(this, ui->textEdit);
    rdl.exec();
}


void MainWindow::on_actionNewFile_triggered()
{
    if(!userEditConfirmed())
        return;

    filePath = "";
    ui->textEdit->clear();
    this->setWindowTitle(tr("新建文本 - 编辑器"));

    textChange = false;
}

void MainWindow::on_actionOpenFile_triggered()
{
    if(!userEditConfirmed())
        return;

    QString filename = QFileDialog::getOpenFileName(this, "打开文件",".",tr("Text files(*.txt) ;; All(*.*)"));
    QFile file(filename);

    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, ". .", "打开文件失败!");
        return;
    }

    filePath = filename;

    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->insertPlainText(text);
    file.close();

    this->setWindowTitle(QFileInfo(filename).absoluteFilePath());

    textChange = false;
}


void MainWindow::on_actionSaveFile_triggered()
{
    if(filePath == "")
    {
        QString filename = QFileDialog::getSaveFileName(this, "保存文件", ".",
                                                        tr("Text files (*.txt) "));
        QFile file(filename);
        if(!file.open(QFile::WriteOnly | QFile::Text))
        {
                QMessageBox::warning(this, ". .", "打开保存文件失败!");
                return ;
        }

        file.close();
        filePath = filename;
    }

    QFile file(filePath);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, ". .", "打开保存文件失败!");
        return;
    }

    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out<<text;
    file.flush();
    file.close();

    this->setWindowTitle(QFileInfo(filePath).absoluteFilePath());

    textChange = false;
}


void MainWindow::on_actionSaveAs_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "另存为文件",".",tr("Text files(*.txt) ;; All(*.*)"));
    QFile file(filename);

    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, ". .", "另存为文件失败!");
        return;
    }

    filePath = filename;
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out<<text;
    file.flush();
    file.close();

    this->setWindowTitle(QFileInfo(filePath).absoluteFilePath());
}


void MainWindow::on_textEdit_textChanged()
{
    if(!textChange)
    {
        this->setWindowTitle("*" + this->windowTitle());
        textChange = true;
    }

    statusLabel.setText("length:" + QString::number(ui->textEdit->toPlainText().length()) +
                        "      lines:     " + QString::number(ui->textEdit->document()->lineCount()));
}

bool MainWindow::userEditConfirmed()
{
    if(textChange)
    {
        QString path = filePath != "" ? filePath : "无标题.txt";

        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Question);
        msg.setWindowTitle("...");
        msg.setWindowFlag(Qt::Drawer);
        msg.setText(QString("是否将更改保存到\n") + "\"" + path + "\"");
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        int r = msg.exec();
        switch ( r ) {
        case QMessageBox::Yes:
            on_actionSaveFile_triggered();
            break;
        case QMessageBox::No:
            textChange = false;
            break;
        case QMessageBox::Cancel:
            return false;
        }
    }

    return true;
}

void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}


void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
    ui->actionPaste->setEnabled(true);
}


void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
    ui->actionPaste->setEnabled(true);
}


void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}


void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}


void MainWindow::on_textEdit_copyAvailable(bool b)
{
    ui->actionCopy->setEnabled(b);
    ui->actionCut->setEnabled(b);
}


void MainWindow::on_textEdit_redoAvailable(bool b)
{
    ui->actionRedo->setEnabled(b);
}


void MainWindow::on_textEdit_undoAvailable(bool b)
{
    ui->actionUndo->setEnabled(b);
}


void MainWindow::on_actionFontColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "选择颜色");
    if(color.isValid())
    {
        ui->textEdit->setStyleSheet(QString("QPlainTextEdit {color: %1}").arg(color.name()));
    }
}


void MainWindow::on_actionFontBackColor_triggered()
{

}


void MainWindow::on_actionBackColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "选择颜色");
    if(color.isValid())
    {
        ui->textEdit->setStyleSheet(QString("QPlainTextEdit {background-color: %1}").arg(color.name()));
    }
}


void MainWindow::on_actionAutoWordWrap_triggered()
{
    QPlainTextEdit::LineWrapMode mode = ui->textEdit->lineWrapMode();

    if(mode == QTextEdit::NoWrap)
    {
        ui->textEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        ui->actionAutoWordWrap->setChecked(true);
    }
    else
    {
        ui->textEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
        ui->actionAutoWordWrap->setChecked(false);
    }
}


void MainWindow::on_actionFont_triggered()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, this);

    if(ok)
        ui->textEdit->setFont(font);
}


void MainWindow::on_actionShowBar_triggered()
{
    bool visible = ui->statusbar->isVisible();
    ui->statusbar->setVisible(!visible);
    ui->actionShowBar->setChecked(!visible);
}


void MainWindow::on_actionToolBar_triggered()
{
    bool visible = ui->toolBar->isVisible();
    ui->toolBar->setVisible(!visible);
    ui->actionToolBar->setChecked(!visible);
}


void MainWindow::on_actionSelectAll_triggered()
{
    ui->textEdit->selectAll();
}


void MainWindow::on_actionExit_triggered()
{
    if(userEditConfirmed())
        exit(0);
}


void MainWindow::on_textEdit_cursorPositionChanged()
{
    int col = 0;
    int ln = 0;
    int flg = -1;
    int pos = ui->textEdit->textCursor().position();
    QString text = ui->textEdit->toPlainText();

    for(int i = 0; i < pos; i++)
    {
        if(text[i] == '\n')
        {
            ln++;
            flg = i;
        }
    }

    flg++;
    col = pos - flg;
    statusCursorLabel.setText("Ln:" + QString::number(ln + 1) + "      Col: " + QString::number(col + 1));
}

