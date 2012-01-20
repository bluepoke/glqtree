#ifndef OPTIONSFORMLAYOUT_H
#define OPTIONSFORMLAYOUT_H

#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>

class OptionsFormLayout : public QFormLayout
{
    Q_OBJECT
public:
    explicit OptionsFormLayout(QWidget *parent = 0);
private:
    QLineEdit *txtName;
    QSpinBox *spinAge;
    QSpinBox *spinSeed;
    QPushButton *btnRandomSeed;

signals:
    
public slots:
    void randomSeed();
    void changeSeed();
    void changeName();
    void changeMaxAge();
    
};

#endif // OPTIONSFORMLAYOUT_H
