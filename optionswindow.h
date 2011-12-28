#ifndef OPTIONSWINDOW_H
#define OPTIONSWINDOW_H

#include <QTabWidget>

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
};

#endif // OPTIONSWINDOW_H
