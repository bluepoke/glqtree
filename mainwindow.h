#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "tabbedoptionsdialog.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    TabbedOptionsDialog *tabs;
    QPushButton *btnQuit;
    QPushButton *btnOptions;

public slots:
    void showOptionsDialog();
};

#endif // MAINWINDOW_H
