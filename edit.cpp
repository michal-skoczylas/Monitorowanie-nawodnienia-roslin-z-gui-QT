#include "edit.h"
#include "ui_edit.h"
//Obraz
#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <QMessageBox>
#include <QByteArray>
#include <QBuffer>
#include <QDebug>
//SQL
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include <QMessageBox>


QString sciezka_obrazu;
edit::edit(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::edit)
{
    ui->setupUi(this);
}

edit::~edit()
{
    delete ui;
}
//konstruktor z parametrem id
edit::edit(QString arg_id, QWidget *parent)
    : QDialog(parent), ui(new Ui::edit){
    ui->setupUi(this);
    id = arg_id;

}
void edit::on_pushButton_obraz_clicked()
{
   sciezka_obrazu = QFileDialog::getOpenFileName(this, "Wybierz obraz", "", "Obraz (*.png *.jpg *.jpeg *.bmp)");
    QImage obraz(sciezka_obrazu);
    ui->label_obraz->setPixmap(QPixmap::fromImage(obraz));
    ui->label_obraz->setScaledContents(true);
}


void edit::on_pushButton_clicked()
{

    //Aktualizacja obiektu z bazy danych
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/Michał/Desktop/qt kurz/NawadnianieRoslin/bazadanych.db");
    if(!db.open()){
        qDebug()<<"Baza danych nie otwarta";
    }
    else{
        qDebug()<<"Baza danych otwarta";
    }
    QString updateQuery = "UPDATE rosliny SET nazwa = :nazwa, moist = :moist, image = :obraz WHERE id = :id";
    QSqlQuery query;
    query.prepare(updateQuery);
    //id
    query.bindValue(":id",id);
    //nazwa
    query.bindValue(":nazwa",ui->lineEdit_nazwa->text());
    //moist
    query.bindValue(":moist",ui->spinBox_moist->text());
    //obraz jako BLOP
    QImage obraz(sciezka_obrazu);
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    obraz.save(&buffer,"PNG");
    query.bindValue(":obraz",byteArray);
    //wykonanie zapytania
    if(!query.exec()){
        qDebug()<<"Nie udalo sie zaktualizowac";
        qDebug()<<query.lastError().text();
    }
    else{
        qDebug()<<"Zaktualizowano";
        QMessageBox::information(this,"SUKCES","Udalo sie zaktualizowac",QMessageBox::Ok);
    }
    db.close();
    this->close();

}

