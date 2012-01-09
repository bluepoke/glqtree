#include "optionsdialogtablayout.h"
#include <QDebug>
#include <graphwidget.h>

OptionsDialogTabLayout::OptionsDialogTabLayout(QWidget *parent) :
    QGridLayout(parent)
{
    dialog = (TabbedOptionsDialog*)parent;
}

void OptionsDialogTabLayout::initValues() {
    // this is where handling the xml should be done and calls to the different
    // display initialzations for each option should be done as exampled by the two following calls

    // if you find a better way to hand over the values, for example as a single container object,
    // by all means do it, there must be something better than arrays

    // for now I declare that the data to be handed over has to:
    // 1. there can not be any ambigious age values
    // 2. there has to be a probability value (even if we do not use those through the boolean operator)
    // 3. age values have to be in sorted order
    // 4. there must be one tupel for age = 0 and one for age = max_age

    // I assume there will be an age spinner somewhere to enter the max age
    // Though I could re-calculate all the values in the table when the max_age happens, I cannot
    // be sure as to what happens, shrinking the whole age range and floor to nearest int will
    // most likely result in duplicate age values which contradict the rules I set.

    // I might simply cut the top off in this case, and increase it when the max age changes upwards

    int maxAge = 50;
    vector<int> ages;
    vector<double> probabilities;
    vector<int> values;

    ages.push_back(0);
    ages.push_back(25);
    ages.push_back(50);
    probabilities.push_back(0.2);
    probabilities.push_back(0.1);
    probabilities.push_back(0.5);
    values.push_back(5);
    values.push_back(10);
    values.push_back(12);

    // position integer deciding where on the optionspanel's layout the widget is placed
    int row = 0;

    initValue(row++, QString("Branching"), maxAge, true, ages, probabilities, values);

    ages.clear();
    probabilities.clear();
    values.clear();
    ages.push_back(0);
    ages.push_back(20);
    ages.push_back(50);
    probabilities.push_back(0.5);
    probabilities.push_back(0.2);
    probabilities.push_back(0.7);
    values.push_back(2);
    values.push_back(1);
    values.push_back(19);

    initValue(row++, QString("Thickness"), maxAge, false, ages, probabilities, values);

}

void OptionsDialogTabLayout::initValue(int row, QString valueName, int maxAge, bool probabilityColumn,
                                        vector<int> ages,
                                        vector<double> probabilities,
                                        vector<int> values) {

    // the table is where all changable display of values is to be done
    QTableWidget *table = new QTableWidget;

    // graphical visualization of the table's contents and a name for finding it later
    GraphWidget *graph = new GraphWidget(table);
    qDebug() << graph;

    // connect updates on the graph to signals we emit here
    connect(this, SIGNAL(valuesChanged()), graph, SLOT(update()));
    // connect any changes to the dialog close button
    connect(this, SIGNAL(valuesChanged()), parentWidget(), SLOT(valuesChanged()));

    // adding graph and table
    addWidget(graph, row, 0);
    addWidget(table, row, 1);

    // decide on the column count
    probabilityColumn ? table->setColumnCount(4) : table->setColumnCount(3);
    table->setRowCount(1);
    // headers and sorting
    QStringList list;
    list << "Age";
    if (probabilityColumn) list << "Probability";
    list << valueName << "";
    table->setHorizontalHeaderLabels(list);
    table->setSortingEnabled(false);
    table->verticalHeader()->hide();

    // serves to add items to different columns
    int column = 0;

    // a spinner for the age to be added
    singleSpin = new QSpinBox;
    singleSpin->setRange(0, maxAge);
    singleSpin->setSingleStep(1);
    singleSpin->setValue(0);
    table->setCellWidget(0, column++, singleSpin);

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

    // a button to add new lines
    addBtn = new QPushButton("Add", table);
    connect(addBtn, SIGNAL(clicked()), this, SLOT(addRow()));
    table->setCellWidget(0, column++, addBtn);

    // add the initial data to the table
    while (!ages.empty()) {
        // prepare a new row
        lbl = new QLabel;
        lbl->setNum(ages.back());
        if (probabilityColumn) {
            doubleSpin = new QDoubleSpinBox;
            doubleSpin->setRange(0.0, 1.0);
            doubleSpin->setSingleStep(0.01);
            doubleSpin->setValue(probabilities.back());
            // connect any changes to the dialog close button and the graph
            connect(doubleSpin, SIGNAL(valueChanged(double)), graph, SLOT(update()));
            connect(doubleSpin, SIGNAL(valueChanged(double)), parentWidget(), SLOT(valuesChanged()));
        }
        singleSpin = new QSpinBox;
        singleSpin->setRange(0, 20);
        singleSpin->setSingleStep(1);
        singleSpin->setValue(values.back());
        // connect any changes to the dialog close button and the graph
        connect(singleSpin, SIGNAL(valueChanged(int)), graph, SLOT(update()));
        connect(singleSpin, SIGNAL(valueChanged(int)), parentWidget(), SLOT(valuesChanged()));

        // serves to add items to different columns
        column = 0;

        // add the row
        table->insertRow(0);

        // add the different items
        table->setCellWidget(0, column++, lbl);

        if (probabilityColumn) {
            table->setCellWidget(0, column++, doubleSpin);
        }
        table->setCellWidget(0, column++, singleSpin);

        if (ages.back() == 0 || ages.back() == maxAge) {
            lbl = new QLabel;
            table->setCellWidget(0, column++, lbl);
        }
        else {
            delBtn = new QPushButton("Delete", table);
            connect(delBtn, SIGNAL(clicked()), this, SLOT(delRow()));
            table->setCellWidget(0, column++, delBtn);
        }

        // remove the just added elements from the containers holding them
        ages.pop_back();
        probabilities.pop_back();
        values.pop_back();
    }
}

void OptionsDialogTabLayout::addRow() {
    // decide who is adding which where
    QPushButton *btnAdd = qobject_cast<QPushButton *>(QObject::sender());
    // Apparently, the parent of the button is not the table but a scrollarea surrounding the button
    QTableWidget *table = (QTableWidget*)(btnAdd->parentWidget()->parentWidget());
    GraphWidget *graph = this->graph;
    qDebug() << graph;

    int rowCount = table->rowCount();
    int columnCount = table->columnCount();

    // find the correct row where the add button was pressed
    for (int row = 0; row < rowCount; row++) {
        for (int col = 0; col < columnCount; col++) {
            if (sender() == table->cellWidget(row, col)) {

                // remember the values to be added
                double iAge = ((QSpinBox*)table->cellWidget(row, 0))->value();
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
                    // if the rows age value is lower, step over it
                    int iNextAge = ((QLabel*)table->cellWidget(next, 0))->text().toInt();
                    if (iNextAge < iAge) {
                        continue;
                    }
                    // if it equals, warn that nothing can be added
                    else if (iNextAge == iAge) {
                        QMessageBox(QMessageBox::Critical, "Duplicate entry",
                                    "There is already an entry for this age!", QMessageBox::Ok).exec();
                        return ;
                    }
                    // otherwise add the values
                    else {
                        // clear the fields
                        ((QSpinBox*)table->cellWidget(row, 0))->setValue(0);
                        if (columnCount == 4) {
                            ((QDoubleSpinBox*)table->cellWidget(row, 1))->setValue(0.0);
                            ((QSpinBox*)table->cellWidget(row, 2))->setValue(0);
                        }
                        else {
                            ((QSpinBox*)table->cellWidget(row, 1))->setValue(0);
                        }

                        // prepare a new row
                        lbl = new QLabel;
                        lbl->setNum(iAge);
                        if (columnCount == 4) {
                            doubleSpin = new QDoubleSpinBox;
                            doubleSpin->setRange(0.0, 1.0);
                            doubleSpin->setSingleStep(0.01);
                            doubleSpin->setValue(dProbability);
                            // connect any changes to the dialog close button and the graph
                            connect(doubleSpin, SIGNAL(valueChanged(double)), graph, SLOT(update()));
                            connect(doubleSpin, SIGNAL(valueChanged(double)), dialog, SLOT(valuesChanged()));
                        }
                        singleSpin = new QSpinBox;
                        singleSpin->setRange(0, 20);
                        singleSpin->setSingleStep(1);
                        singleSpin->setValue(iAmount);
                        // connect any changes to the dialog close button and the graph
                        connect(singleSpin, SIGNAL(valueChanged(int)), graph, SLOT(update()));
                        connect(singleSpin, SIGNAL(valueChanged(int)), dialog, SLOT(valuesChanged()));
                        delBtn = new QPushButton("Delete", table);
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
                        emit valuesChanged();
                        return;
                    }
                }
            }
        }
    }
}

void OptionsDialogTabLayout::delRow() {
    // decide who deletes what and where
    QPushButton *btnDel = qobject_cast<QPushButton *>(QObject::sender());
    // Apparently, the parent of the button is not the table but the scrollarea surrounding the button
    QTableWidget *table = (QTableWidget*)(btnDel->parentWidget()->parentWidget());

    int rowCount = table->rowCount();
    int columnCount = table->columnCount();
    // find the buttons row
    for (int row = 0; row < rowCount; row++) {
        for (int col = 0; col < columnCount; col++) {
            if (sender() == table->cellWidget(row, col)) {
                // remove the whole row
                table->removeRow(row);
                emit valuesChanged();
                return;
            }
        }
    }
}
