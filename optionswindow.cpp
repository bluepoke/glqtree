#include "optionswindow.h"
#include "ui_optionswindow.h"
#include <QMessageBox>
#include <QDebug>
#include "optionswindowlayout.h"

OptionsWindow::OptionsWindow(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::OptionsWindow)
{
    ui->setupUi(this);
    list << "Range" << "Probability" << "Amount" << "";
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setRowCount(3);
    ui->tableWidget->setHorizontalHeaderLabels(list);
    ui->tableWidget->setSortingEnabled(false);

    lbl = new QLabel;
    lbl->setNum(0);
    doubleSpin = new QDoubleSpinBox;
    doubleSpin->setRange(0.0, 1.0);
    doubleSpin->setSingleStep(0.01);
    doubleSpin->setValue(0.5);
    singleSpin = new QSpinBox;
    singleSpin->setRange(0, 20);
    singleSpin->setSingleStep(1);
    singleSpin->setValue(2);
    ui->tableWidget->setCellWidget(0, 0, lbl);
    ui->tableWidget->setCellWidget(0, 1, doubleSpin);
    ui->tableWidget->setCellWidget(0, 2, singleSpin);
    lbl = new QLabel;
    ui->tableWidget->setCellWidget(0, 3, lbl);

    lbl = new QLabel;
    lbl->setNum(1);
    doubleSpin = new QDoubleSpinBox;
    doubleSpin->setRange(0.0, 1.0);
    doubleSpin->setSingleStep(0.01);
    doubleSpin->setValue(0.7);
    singleSpin = new QSpinBox;
    singleSpin->setRange(0, 20);
    singleSpin->setSingleStep(1);
    singleSpin->setValue(5);
    ui->tableWidget->setCellWidget(1, 0, lbl);
    ui->tableWidget->setCellWidget(1, 1, doubleSpin);
    ui->tableWidget->setCellWidget(1, 2, singleSpin);
    lbl = new QLabel;
    ui->tableWidget->setCellWidget(1, 3, lbl);

    btnAdd = new QPushButton("Add", this);
    connect(btnAdd, SIGNAL(clicked()), this, SLOT(addRow()));
    doubleSpin = new QDoubleSpinBox;
    doubleSpin->setRange(0.0, 1.0);
    doubleSpin->setSingleStep(0.01);
    doubleSpin->setValue(0.0);
    ui->tableWidget->setCellWidget(2, 0, doubleSpin);
    doubleSpin = new QDoubleSpinBox;
    doubleSpin->setRange(0.0, 1.0);
    doubleSpin->setSingleStep(0.01);
    doubleSpin->setValue(0.0);
    ui->tableWidget->setCellWidget(2, 1, doubleSpin);
    singleSpin = new QSpinBox;
    singleSpin->setRange(0, 20);
    singleSpin->setSingleStep(1);
    singleSpin->setValue(0);
    ui->tableWidget->setCellWidget(2, 2, singleSpin);
    ui->tableWidget->setCellWidget(2, 3, btnAdd);

    OptionsWindowLayout *tab2Layout = new OptionsWindowLayout(ui->tab2);
}

OptionsWindow::~OptionsWindow()
{
    delete ui;
}

void OptionsWindow::addRow() {

    int rowCount = ui->tableWidget->rowCount();
    int columnCount = ui->tableWidget->columnCount();
    for (int row = 0; row < rowCount; row++) {
        for (int col = 0; col < columnCount; col++) {
            if (sender() == ui->tableWidget->cellWidget(row, col)) {

                double dRange = ((QDoubleSpinBox*)ui->tableWidget->cellWidget(row, 0))->value();
                //qDebug() << "range: " << dRange;

                for (int next = 0; next < rowCount - 1; next++) {
                    double dNextRange = ((QLabel*)ui->tableWidget->cellWidget(next, 0))->text().toDouble();
                    //qDebug() << "next range " << dNextRange;
                    if (dNextRange < dRange) {
                        continue;
                    }
                    else if (dNextRange == dRange) {
                        QMessageBox* msgBox = new QMessageBox();
                        msgBox->setWindowTitle("Error");
                        msgBox->setText("There is already a row for this range!");
                        msgBox->exec();
                        return;
                    }
                    else {
                        ((QDoubleSpinBox*)ui->tableWidget->cellWidget(row, 0))->setValue(0.0);
                        double dProbability = ((QDoubleSpinBox*)ui->tableWidget->cellWidget(row, 1))->value();
                        ((QDoubleSpinBox*)ui->tableWidget->cellWidget(row, 1))->setValue(0.0);
                        int iAmount = ((QSpinBox*)ui->tableWidget->cellWidget(row, 2))->value();
                        ((QSpinBox*)ui->tableWidget->cellWidget(row, 2))->setValue(0);

                        lbl = new QLabel;
                        lbl->setNum(dRange);
                        doubleSpin = new QDoubleSpinBox;
                        doubleSpin->setRange(0.0, 1.0);
                        doubleSpin->setSingleStep(0.01);
                        doubleSpin->setValue(dProbability);
                        singleSpin = new QSpinBox;
                        singleSpin->setRange(0, 20);
                        singleSpin->setSingleStep(1);
                        singleSpin->setValue(iAmount);
                        btnDel = new QPushButton("Delete", this);
                        connect(btnDel, SIGNAL(clicked()), this, SLOT(delRow()));

                        ui->tableWidget->insertRow(next);
                        ui->tableWidget->setCellWidget(next, 0, lbl);
                        ui->tableWidget->setCellWidget(next, 1, doubleSpin);
                        ui->tableWidget->setCellWidget(next, 2, singleSpin);
                        ui->tableWidget->setCellWidget(next, 3, btnDel);

                        return;
                    }
                }
            }
        }
    }
}

void OptionsWindow::delRow() {
    int rowCount = ui->tableWidget->rowCount();
    int columnCount = ui->tableWidget->columnCount();
    for (int row = 0; row < rowCount; row++) {
        for (int col = 0; col < columnCount; col++) {
            if (sender() == ui->tableWidget->cellWidget(row, col)) {
                ui->tableWidget->removeRow(row);
                return;
            }
        }
    }
}
