#ifndef OPTIONSWINDOW_H
#define OPTIONSWINDOW_H

#include <QTabWidget>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QPushButton>

namespace Ui {
    class OptionsWindow;
}

class OptionsWindow : public QTabWidget
{
    Q_OBJECT

public:
    explicit OptionsWindow(QWidget *parent = 0);
    ~OptionsWindow();

private:
    Ui::OptionsWindow *ui;
    QStringList list;
    QDoubleSpinBox *doubleSpin;
    QSpinBox *singleSpin;
    QLabel *lbl;
    QPushButton *btnAdd;
    QPushButton *btnDel;

public slots:
    void addRow();
    void delRow();

};

#endif // OPTIONSWINDOW_H
