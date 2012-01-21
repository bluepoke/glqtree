#ifndef NEWDIALOG_H
#define NEWDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include "plant.h"
#include "scene.h"
#include "tabbedoptionsdialog.h"

class NewDialog : public QDialog
{
    Q_OBJECT
public:
    explicit NewDialog();

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
