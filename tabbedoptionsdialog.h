#ifndef TABBEDOPTIONSDIALOG_H
#define TABBEDOPTIONSDIALOG_H

#include <QDialog>
#include <QTabWidget>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QFileDialog>
#include "newdialog.h"
#include "scene.h"
#include "persistencemanager.h"

#include "optionsformlayout.h"

class Tab : public QWidget
{
    Q_OBJECT
public:
    Tab(QWidget *parent = 0, QLayout *mainLayout = 0);
};

class TabbedOptionsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TabbedOptionsDialog(QWidget *parent = 0);
    void reloadTabs();

private:
    QTabWidget *tabWidget;
    QDialogButtonBox *buttonBox;
    QPushButton *btnClose;
    QPushButton *btnOpen;
    QPushButton *btnSave;
    QPushButton *btnNew;
    void initTabs();

public:
    OptionsFormLayout *optionsFormLayout;

signals:
    void cameraChanged(int x, int y, int zoom);

public slots:
    void valuesChanged();
    void closeDialog();
    void openFromXML();
    void saveToXML();
    void newPlant();
};
#endif // TABBEDOPTIONSDIALOG_H
