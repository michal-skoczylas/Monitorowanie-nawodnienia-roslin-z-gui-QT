#ifndef DODAWANIE_H
#define DODAWANIE_H

#include <QDialog>


//sql
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>


namespace Ui {
class Dodawanie;
}

class Dodawanie : public QDialog
{
    Q_OBJECT

public:
    explicit Dodawanie(QWidget *parent = nullptr);
    ~Dodawanie();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_dodaj_clicked();

private:
    Ui::Dodawanie *ui;
     QSqlDatabase db;

};

#endif // DODAWANIE_H
