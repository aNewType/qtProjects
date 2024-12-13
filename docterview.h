#ifndef DOCTERVIEW_H
#define DOCTERVIEW_H

#include <QWidget>

namespace Ui {
class DocterView;
}

class DocterView : public QWidget
{
    Q_OBJECT

public:
    explicit DocterView(QWidget *parent = nullptr);
    ~DocterView();

private:
    Ui::DocterView *ui;
};

#endif // DOCTERVIEW_H
