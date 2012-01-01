#ifndef OPTIONSWINDOWLAYOUT_H
#define OPTIONSWINDOWLAYOUT_H

#include <QGridLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QMessageBox>

using namespace std;

class OptionsWindowLayout : public QGridLayout
{
    Q_OBJECT

public:
    explicit OptionsWindowLayout(QWidget *parent = 0);
    void initValues();

private:
    // static const int valuesCount;
    QTableWidget *table;
    QPushButton *addBtn;
    QPushButton *delBtn;
    QDoubleSpinBox *doubleSpin;
    QSpinBox *singleSpin;
    QLabel *lbl;
    void initValue(int row = 0, QString valueName = 0, bool probabilityColumn = 0,
                       vector<double> ranges = vector<double> (0),
                       vector<double> probabilities = vector<double> (0),
                       vector<int> values = vector<int> (0));

public slots:
    void addRow();
    void delRow();
};

#endif // OPTIONSWINDOWLAYOUT_H
