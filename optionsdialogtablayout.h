#ifndef OPTIONSDIALOGTABLAYOUT_H
#define OPTIONSDIALOGTABLAYOUT_H

#include <QGridLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QMessageBox>
#include "graphwidget.h"
#include "tabbedoptionsdialog.h"

using namespace std;

class ValuesTable : public QTableWidget
{
    Q_OBJECT

public:
    explicit ValuesTable(QWidget *parent = 0);
    GraphWidget *graph;
    bool probabilityColumn;
    bool valueColumn;
};

class OptionsDialogTabLayout : public QGridLayout
{
    Q_OBJECT

public:
    explicit OptionsDialogTabLayout(QWidget *parent = 0);
    void initValues();

private:
    TabbedOptionsDialog *dialog;
    QTableWidget *table;
    GraphWidget *graph;
    QPushButton *addBtn;
    QPushButton *delBtn;
    QDoubleSpinBox *doubleSpin;
    QSpinBox *singleSpin;
    QLabel *lbl;
    void initValue(int row = 0, QString valueName = 0, int maxAge = 0,
                   bool probabilityColumn = 0, bool valueColumn = 0,
                       vector<int> ages = vector<int> (0),
                       vector<double> probabilities = vector<double> (0),
                       vector<int> values = vector<int> (0));

signals:
    void valuesChanged();

public slots:
    void addRow();
    void delRow();
};

#endif // OPTIONSDIALOGTABLAYOUT_H
