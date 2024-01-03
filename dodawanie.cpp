#include "dodawanie.h"
#include "ui_dodawanie.h"
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

//test czy moist to liczba
#include <QString>


QString obraz_filename;
Dodawanie::Dodawanie(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dodawanie)
{
    ui->setupUi(this);
}

Dodawanie::~Dodawanie()
{
    delete ui;
}

void Dodawanie::on_pushButton_2_clicked()
{
    obraz_filename = QFileDialog::getOpenFileName(this, "Wybierz obraz", "", "Obraz (*.png *.jpg *.jpeg *.bmp)");
    QImage obraz(obraz_filename);
    ui->label_obraz->setPixmap(QPixmap::fromImage(obraz));
    ui->label_obraz->setScaledContents(true);

}


void Dodawanie::on_pushButton_dodaj_clicked()
{
    QString nazwa = ui->lineEdit_nazwa->text();
    QString moist = ui->spinBox_moist->text();
    QImage obraz(obraz_filename);

    if(nazwa.isEmpty() || moist.isEmpty() || obraz_filename.isEmpty()){
        QMessageBox::information(this,"Błąd","Wszystkie pola muszą być wypełnione",QMessageBox::Ok);
    }
    else{
        //otwarcie bazy danych
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("C:/Users/Michał/Desktop/qt kurz/NawadnianieRoslin/bazadanych.db");

        if(!db.open()){
            qDebug()<<"Baza danych nie otwarta";
        }
        else{

            qDebug()<<"Baza danych otwarta";
            QByteArray imageData;
            QBuffer buffer(&imageData);
            buffer.open(QIODevice::WriteOnly);
            obraz.save(&buffer,"PNG");
            //Dodawanie obrazu do bazy danych
            QSqlQuery query;
            query.prepare("INSERT INTO rosliny (nazwa, moist, image) VALUES (:nazwa, :moist, :imageData)");
            query.bindValue(":nazwa",nazwa);
            query.bindValue(":moist",moist);
            query.bindValue(":imageData",imageData, QSql::Binary);

            if(!query.exec()){
                qDebug()<<"Nie dodano rośliny "<<query.lastError().text();
            }
            else{
                qDebug()<<"Dodano roślinę";
            }
        }

        QMessageBox::information(this,"Sukces","Dodano roślinę",QMessageBox::Ok);

    }

}

