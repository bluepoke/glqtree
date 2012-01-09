#ifndef TABBEDOPTIONSDIALOG_H
#define TABBEDOPTIONSDIALOG_H

#include <QDialog>
#include <QTabWidget>
#include <QDialogButtonBox>

class QTabWidget;
class QDialogButtonBox;

class Tab1 : public QWidget
{
    Q_OBJECT
public:
    Tab1(QWidget *parent = 0, QLayout *mainLayout = 0);
};

class Tab2 : public QWidget
{
    Q_OBJECT
public:
    Tab2(QWidget *parent = 0, QLayout *mainLayout = 0);
};

class TabbedOptionsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TabbedOptionsDialog(QWidget *parent = 0);

private:
    bool updated;
    QTabWidget *tabWidget;
    QDialogButtonBox *buttonBox;
    QPushButton *btnClose;
    QPushButton *btnOpen;
    QPushButton *btnSave;

public slots:
    void valuesChanged();
    void closeDialog();
    void openFromXML();
    void saveToXML();
};
#endif // TABBEDOPTIONSDIALOG_H
