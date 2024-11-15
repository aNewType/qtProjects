#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QObject>
#include <QPlainTextEdit>

class codeEditor: public QPlainTextEdit
{
    Q_OBJECT
public:
    codeEditor(QWidget *parent = nullptr);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);
public slots:
    void hideLineNumberArea(bool flag);

private:
    QWidget *lineNumberArea;
};

class LineNumberArea: public QWidget
{
public:
    LineNumberArea(codeEditor *editor): QWidget(editor), codeEditor(editor)
    {}

    QSize sizeHint() const override
    {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    codeEditor *codeEditor;
};

#endif // CODEEDITOR_H
