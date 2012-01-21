#ifndef NEWDIALOG_H
#define NEWDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>

class NewDialog : public QDialog
{
    Q_OBJECT
public:
    explicit NewDialog(QWidget *parent = 0);

    QDialogButtonBox *buttonBox;
    QPushButton *btnCancel;
    QPushButton *btnNew;
    QFormLayout *mainPanelLayout;
    QLineEdit *txtName;
    QSpinBox *spinAge;
    QSpinBox *spinSeed;
    QPushButton *btnRandomSeed;

public slots:
    void cancelDialog();
    void newPlant();
    void randomSeed();
};

#endif // NEWDIALOG_H
