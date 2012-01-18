#include "optionsdialogtablayout.h"

OptionsDialogTabLayout::OptionsDialogTabLayout(QWidget *parent) :
    QGridLayout(0)
{
    dialog = (TabbedOptionsDialog*)parent;
}

ValuesTable::ValuesTable(QWidget *parent) :
    QTableWidget(parent)
{
}

void OptionsDialogTabLayout::initValue(int row, QString valueName, int maxAge,
                                       bool probabilityColumn, bool valueColumn, QList<Tupel3> *values) {

    // the table is where all changable display of values is to be done
    ValuesTable *table = new ValuesTable;

    // graphical visualization of the table's contents and a name for finding it later
    GraphWidget *graph = new GraphWidget;

    // make the widgets know each other
    graph->table = table;
    table->graph = graph;
    // publish the columns
    table->probabilityColumn = probabilityColumn;
    table->valueColumn = valueColumn;
    graph->probabilityColumn = probabilityColumn;
    graph->valueColumn = valueColumn;

    // connect updates on the graph to signals we emit here
    connect(this, SIGNAL(valuesChanged()), graph, SLOT(update()));
    // connect any changes to the dialog close button
    connect(this, SIGNAL(valuesChanged()), dialog, SLOT(valuesChanged()));

    // adding graph and table
    addWidget(graph, row, 0);
    addWidget(table, row, 1);

    // list for header names
    QStringList list;
    list << "Age";

    // decide on the column count and headers
    if (probabilityColumn && valueColumn) {
        table->setColumnCount(4);
        list << "Probability" << valueName << "";

    }
    else if (!probabilityColumn && !valueColumn) {
        table->setColumnCount(2);
        list << "";
    }
    else {
        table->setColumnCount(3);
        list << valueName << "";
    }

    table->setRowCount(1);
    // headers and sorting
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
    if (valueColumn) {
        singleSpin = new QSpinBox;
        singleSpin->setMinimum(0);
        singleSpin->setSingleStep(1);
        singleSpin->setValue(0);
        table->setCellWidget(0, column++, singleSpin);
    }

    // a button to add new lines
    addBtn = new QPushButton("Add", table);
    connect(addBtn, SIGNAL(clicked()), this, SLOT(addRow()));
    table->setCellWidget(0, column++, addBtn);

    // prepend all data to the table, one tupel after the other
    for (int i=values->count() - 1; i >= 0; i--) {
        Tupel3 tupel = values->at(i);

        // prepare a new row
        lbl = new QLabel;
        lbl->setNum(tupel.age);

        if (probabilityColumn) {
            doubleSpin = new QDoubleSpinBox;
            doubleSpin->setRange(0.0, 1.0);
            doubleSpin->setSingleStep(0.01);
            doubleSpin->setValue(tupel.probability);
            // connect any changes to the dialog close button and the graph
            connect(doubleSpin, SIGNAL(valueChanged(double)), graph, SLOT(update()));
            connect(doubleSpin, SIGNAL(valueChanged(double)), dialog, SLOT(valuesChanged()));
        }

        if (valueColumn) {
            singleSpin = new QSpinBox;
            singleSpin->setMinimum(0);
            singleSpin->setSingleStep(1);
            singleSpin->setValue(tupel.value);
            // connect any changes to the dialog close button and the graph
            connect(singleSpin, SIGNAL(valueChanged(int)), graph, SLOT(update()));
            connect(singleSpin, SIGNAL(valueChanged(int)), dialog, SLOT(valuesChanged()));
        }

        // serves to add items to different columns
        column = 0;

        // add the row
        table->insertRow(0);

        // add the different items
        table->setCellWidget(0, column++, lbl);

        if (probabilityColumn) {
            table->setCellWidget(0, column++, doubleSpin);
        }

        if (valueColumn) {
            table->setCellWidget(0, column++, singleSpin);
        }

        if (tupel.age == 0 || tupel.age == maxAge) {
            lbl = new QLabel;
            table->setCellWidget(0, column++, lbl);
        }
        else {
            delBtn = new QPushButton("Delete", table);
            connect(delBtn, SIGNAL(clicked()), this, SLOT(delRow()));
            table->setCellWidget(0, column++, delBtn);
        }
    }
}

void OptionsDialogTabLayout::addRow() {
    // decide who is adding which where
    QPushButton *btnAdd = qobject_cast<QPushButton *>(QObject::sender());
    // Apparently, the parent of the button is not the table but a scrollarea surrounding the button
    ValuesTable *table = (ValuesTable*)(btnAdd->parentWidget()->parentWidget());
    GraphWidget *graph = table->graph;


    int rowCount = table->rowCount();
    int columnCount = table->columnCount();

    // find the correct row where the add button was pressed
    for (int row = 0; row < rowCount; row++) {
        for (int col = 0; col < columnCount; col++) {
            if (sender() == table->cellWidget(row, col)) {

                // remember the values to be added
                int iAge = ((QSpinBox*)table->cellWidget(row, 0))->value();
                double dProbability = 0.0;
                int iValue = 0;

                if (table->probabilityColumn && table->valueColumn) {
                    dProbability = ((QDoubleSpinBox*)table->cellWidget(row, 1))->value();
                    iValue = ((QSpinBox*)table->cellWidget(row, 2))->value();
                }
                else if (!table->probabilityColumn && table->valueColumn) {
                    iValue = ((QSpinBox*)table->cellWidget(row, 1))->value();
                }
                else if (table->probabilityColumn && !table->valueColumn) {
                    dProbability = ((QDoubleSpinBox*)table->cellWidget(row, 1))->value();
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
                        if (table->probabilityColumn && table->valueColumn) {
                            ((QDoubleSpinBox*)table->cellWidget(row, 1))->setValue(0.0);
                            ((QSpinBox*)table->cellWidget(row, 2))->setValue(0);
                        }
                        else if (!table->probabilityColumn && table->valueColumn) {
                            ((QSpinBox*)table->cellWidget(row, 1))->setValue(0);
                        }
                        else if (table->probabilityColumn && !table->valueColumn) {
                            ((QDoubleSpinBox*)table->cellWidget(row, 1))->setValue(0.0);
                        }

                        // prepare a new row
                        lbl = new QLabel;
                        lbl->setNum(iAge);
                        if (table->probabilityColumn) {
                            doubleSpin = new QDoubleSpinBox;
                            doubleSpin->setRange(0.0, 1.0);
                            doubleSpin->setSingleStep(0.01);
                            doubleSpin->setValue(dProbability);
                            // connect any changes to the dialog close button and the graph
                            connect(doubleSpin, SIGNAL(valueChanged(double)), graph, SLOT(update()));
                            connect(doubleSpin, SIGNAL(valueChanged(double)), dialog, SLOT(valuesChanged()));
                        }
                        if (table->valueColumn) {
                            singleSpin = new QSpinBox;
                            singleSpin->setMinimum(0);
                            singleSpin->setSingleStep(1);
                            singleSpin->setValue(iValue);
                            // connect any changes to the dialog close button and the graph
                            connect(singleSpin, SIGNAL(valueChanged(int)), graph, SLOT(update()));
                            connect(singleSpin, SIGNAL(valueChanged(int)), dialog, SLOT(valuesChanged()));
                        }

                        delBtn = new QPushButton("Delete", table);
                        connect(delBtn, SIGNAL(clicked()), this, SLOT(delRow()));

                        // add the row
                        int column = 0;
                        table->insertRow(next);
                        table->setCellWidget(next, column++, lbl);
                        if (table->probabilityColumn) {
                            table->setCellWidget(next, column++, doubleSpin);
                        }
                        if (table->valueColumn) {
                            table->setCellWidget(next, column++, singleSpin);
                        }

                        table->setCellWidget(next, column++, delBtn);
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
