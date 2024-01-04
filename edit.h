#ifndef EDIT_H
#define EDIT_H

#include <QDialog>



//sql
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>


namespace Ui {
class edit;
}

class edit : public QDialog
{
    Q_OBJECT

public:
    explicit edit(QWidget *parent = nullptr);
    edit(QString arg_id,QWidget *parent = nullptr);
    ~edit();
    QString id;

private slots:
    void on_pushButton_obraz_clicked();

    void on_pushButton_clicked();

private:
    Ui::edit *ui;
    QSqlDatabase db;
};

#endif // EDIT_H
