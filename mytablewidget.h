#ifndef MYTABLEWIDGET_H
#define MYTABLEWIDGET_H

#include <QTableWidget>

class MyTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit MyTableWidget(QWidget *parent = 0, QString valueName = 0, bool probabilityColumn = 0);
};

#endif // MYTABLEWIDGET_H
