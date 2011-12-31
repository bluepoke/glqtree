#include "optionswindowlayout.h"
#include <QDebug>

OptionsWindowLayout::OptionsWindowLayout(QWidget *parent) :
    QGridLayout(parent)
{
    // this is where a call to load some default xml values is made
    initValues();
}

void OptionsWindowLayout::initValues() {
    // this is where handling the xml should be done and calls to the different
    // display initialzations for each option should be done as exampled by the branching call

    // if you find a better way to hand over the values, for example as a single container object,
    // by all means do it, there must be something better than arrays

    // for now I declare that the data to be handed over has to:
    // 1. there can not be any ambigious range values
    // 2. there has to be a probability value (even if we do not use those through the boolean operator)
    // 3. range values have to be in sorted order
    // 4. there must be one tupel for range = 0.0 and one for range = 1.0

    // const int valuesCount = 3;
    vector<double> ranges;
    vector<double> probabilities;
    vector<int> values;

    ranges.push_back(0.0);
    ranges.push_back(0.5);
    ranges.push_back(1.0);
    probabilities.push_back(0.2);
    probabilities.push_back(0.1);
    probabilities.push_back(0.5);
    values.push_back(5);
    values.push_back(10);
    values.push_back(12);

    initBranching(QString("Branching"), true, ranges, probabilities, values);
}

void OptionsWindowLayout::initBranching(QString valueName, bool probabilityColumn,
                                        vector<double> ranges,
                                        vector<double> probabilities,
                                        vector<int> values) {

    // the table is where all changable display of values is to be done
    QTableWidget *table = new QTableWidget;

    // the button is a placeholder item for now and will be replaced with a
    // graphical visualization of the table contents
    QPushButton *aBtn = new QPushButton;
    aBtn->setText("Push");

    // adding display placeholder and table
    addWidget(aBtn, 0, 0);
    addWidget(table, 0, 1);

    // decide on the column count
    probabilityColumn ? table->setColumnCount(4) : table->setColumnCount(3);
    table->setRowCount(1);
    // add headers
    QStringList list;
    list << "Range";
    if (probabilityColumn) list << "Probability";
    list << valueName << "";
    table->setHorizontalHeaderLabels(list);

    table->setSortingEnabled(false);
    int column = 0;

    // a button to add new lines
    addBtn = new QPushButton;
    addBtn->setText("Add");
    connect(addBtn, SIGNAL(clicked()), this, SLOT(addRow()));

    // a spinner for the range to be added
    doubleSpin = new QDoubleSpinBox;
    doubleSpin->setRange(0.0, 1.0);
    doubleSpin->setSingleStep(0.01);
    doubleSpin->setValue(0.0);
    table->setCellWidget(0, column++, doubleSpin);

    // a spinner for the probability to be added (if there is one needed)
    if (probabilityColumn) {
        doubleSpin = new QDoubleSpinBox;
        doubleSpin->setRange(0.0, 1.0);
        doubleSpin->setSingleStep(0.01);
        doubleSpin->setValue(0.0);
        table->setCellWidget(0, column++, doubleSpin);
    }
    // a spinner for the value to be added
    singleSpin = new QSpinBox;
    singleSpin->setRange(0, 20);
    singleSpin->setSingleStep(1);
    singleSpin->setValue(0);
    table->setCellWidget(0, column++, singleSpin);
    table->setCellWidget(0, column++, addBtn);

    // add the initial data to the table
    while (!ranges.empty()) {
        // prepare a new row
        lbl = new QLabel;
        lbl->setNum(ranges.back());
        if (probabilityColumn) {
            doubleSpin = new QDoubleSpinBox;
            doubleSpin->setRange(0.0, 1.0);
            doubleSpin->setSingleStep(0.01);
            doubleSpin->setValue(probabilities.back());
        }
        singleSpin = new QSpinBox;
        singleSpin->setRange(0, 20);
        singleSpin->setSingleStep(1);
        singleSpin->setValue(values.back());

        column = 0;
        // add the row
        table->insertRow(0);
        table->setCellWidget(0, column++, lbl);
        if (probabilityColumn) {
            table->setCellWidget(0, column++, doubleSpin);
        }
        table->setCellWidget(0, column++, singleSpin);

        if (ranges.back() == 0.0 || ranges.back() == 1.0) {
            lbl = new QLabel;
            table->setCellWidget(0, column++, lbl);
        }
        else {
            delBtn = new QPushButton;
            delBtn->setText("Delete");
            connect(delBtn, SIGNAL(clicked()), this, SLOT(delRow()));
            table->setCellWidget(0, column++, delBtn);
        }

        // remove the last elements
        ranges.pop_back();
        probabilities.pop_back();
        values.pop_back();
    }
}

void OptionsWindowLayout::addRow() {
    int rowCount = table->rowCount();
    int columnCount = table->columnCount();

    // find the correct row where the add button was pressed
    for (int row = 0; row < rowCount; row++) {
        for (int col = 0; col < columnCount; col++) {
            if (sender() == table->cellWidget(row, col)) {

                // remember the values to be added
                double dRange = ((QDoubleSpinBox*)table->cellWidget(row, 0))->value();
                double dProbability = 0.0;
                int iAmount = 0;
                if (columnCount == 4) {
                    dProbability = ((QDoubleSpinBox*)table->cellWidget(row, 1))->value();
                    iAmount = ((QSpinBox*)table->cellWidget(row, 2))->value();
                }
                else {
                    iAmount = ((QSpinBox*)table->cellWidget(row, 1))->value();
                }

                // find the correct row where the values need to be added to
                for (int next = 0; next < rowCount - 1; next++) {
                    // if the rows range value is lower, step over it
                    double dNextRange = ((QLabel*)table->cellWidget(next, 0))->text().toDouble();
                    if (dNextRange < dRange) {
                        continue;
                    }
                    // if it equals, warn that nothing can be added
                    else if (dNextRange == dRange) {
                        QMessageBox* msgBox = new QMessageBox();
                        msgBox->setWindowTitle("Error");
                        msgBox->setText("There is already a row for this range!");
                        msgBox->exec();
                        return;
                    }
                    // otherwise add the values
                    else {
                        // clear the fields
                        ((QDoubleSpinBox*)table->cellWidget(row, 0))->setValue(0.0);
                        if (columnCount == 4) {
                            ((QDoubleSpinBox*)table->cellWidget(row, 1))->setValue(0.0);
                            ((QSpinBox*)table->cellWidget(row, 2))->setValue(0);
                        }
                        else {
                            ((QSpinBox*)table->cellWidget(row, 1))->setValue(0);
                        }

                        // prepare a new row
                        lbl = new QLabel;
                        lbl->setNum(dRange);
                        if (columnCount == 4) {
                            doubleSpin = new QDoubleSpinBox;
                            doubleSpin->setRange(0.0, 1.0);
                            doubleSpin->setSingleStep(0.01);
                            doubleSpin->setValue(dProbability);
                        }
                        singleSpin = new QSpinBox;
                        singleSpin->setRange(0, 20);
                        singleSpin->setSingleStep(1);
                        singleSpin->setValue(iAmount);
                        delBtn = new QPushButton;
                        delBtn->setText("Delete");
                        connect(delBtn, SIGNAL(clicked()), this, SLOT(delRow()));

                        // add the row
                        table->insertRow(next);
                        table->setCellWidget(next, 0, lbl);
                        if (columnCount == 4) {
                            table->setCellWidget(next, 1, doubleSpin);
                            table->setCellWidget(next, 2, singleSpin);
                            table->setCellWidget(next, 3, delBtn);
                        }
                        else {
                            table->setCellWidget(next, 1, singleSpin);
                            table->setCellWidget(next, 2, delBtn);
                        }
                        return;
                    }
                }
            }
        }
    }
}

void OptionsWindowLayout::delRow() {

}
