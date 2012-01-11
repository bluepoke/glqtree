#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPainter>
#include <QDoubleSpinBox>
#include <QLabel>
#include <algorithm>

using namespace std;

class GraphWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GraphWidget(QWidget *parent = 0);
    QTableWidget *table;

private:
    static const int MARGIN = 17;
    static const int HEIGHT = 100;
    static const int WIDTH = 100;
    static const int dataRowCount;

protected:
    void paintEvent(QPaintEvent *event);

};

#endif // GRAPHWIDGET_H
