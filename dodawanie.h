#ifndef DODAWANIE_H
#define DODAWANIE_H

#include <QDialog>

namespace Ui {
class Dodawanie;
}

class Dodawanie : public QDialog
{
    Q_OBJECT

public:
    explicit Dodawanie(QWidget *parent = nullptr);
    ~Dodawanie();

private:
    Ui::Dodawanie *ui;
};

#endif // DODAWANIE_H
