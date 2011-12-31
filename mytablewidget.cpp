#include "mytablewidget.h"

MyTableWidget::MyTableWidget(QWidget *parent, QString valueName, bool probabilityColumn) :
    QTableWidget(parent)
{
    probabilityColumn ? setColumnCount(4) : setColumnCount(3);
    setRowCount(1);
    QStringList list;
    list << "Range";
    if (probabilityColumn) list << "Probability";
    list << valueName << "";
    setHorizontalHeaderLabels(list);
    setSortingEnabled(false);
}

