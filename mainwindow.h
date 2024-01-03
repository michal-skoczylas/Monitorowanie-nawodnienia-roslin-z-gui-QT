#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dodawanie.h"
//serial port
#include <QSerialPort>
//sql
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:


    void on_pushButton_pomiar_clicked();

    void on_comboBox_currentIndexChanged(int index);



    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
    void update();

    void on_pushButton_update_clicked();

private:
    Ui::MainWindow *ui;
    Dodawanie *dodawanie;
    QSerialPort* arduino;
    QSqlDatabase db;
};
#endif // MAINWINDOW_H
