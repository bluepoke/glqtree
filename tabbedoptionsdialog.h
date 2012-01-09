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
    Tab1(QWidget *parent = 0);
};

class Tab2 : public QWidget
{
    Q_OBJECT
public:
    Tab2(QWidget *parent = 0);
};

class TabbedOptionsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TabbedOptionsDialog(QWidget *parent = 0);

private:
    QTabWidget *tabWidget;
    QDialogButtonBox *buttonBox;
};
#endif // TABBEDOPTIONSDIALOG_H
