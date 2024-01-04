#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "edit.h"
//serial port
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
//SQL
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
//messagebox
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //serialport setup
    arduino = new QSerialPort(this);
    arduino->setPortName("COM7");
    arduino->setBaudRate(QSerialPort::Baud9600);
    if(arduino->open(QIODevice::ReadWrite)){
        qDebug()<<"SERIAL PORT OPENED";
    }
    else
    {
        qDebug()<<"SERIAL PORT NOT OPENED";
    }
    connect(arduino,&QSerialPort::readyRead,this,[&](){
        QByteArray responseData = arduino->readAll();
        qDebug()<<"Rceived data: "<<responseData;
        ui->label_wynik->setText(responseData);
    });

    //Otwarcie bazy danych
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/Michał/Desktop/qt kurz/NawadnianieRoslin/bazadanych.db");
    if(!db.open()){
        qDebug()<<"Baza danych nie otwarta";
    }
    else{
        qDebug()<<"Baza danych otwarta";
    }
    //Wyslanie zapytania i wypelnienie comboboxa nazwami
    QSqlQuery query;
    query.exec("SELECT nazwa FROM rosliny");
    ui->comboBox->clear();
    while(query.next()){
       QString item = query.value(0).toString();
       ui->comboBox->addItem(item);
    }
    query.finish();


}

MainWindow::~MainWindow()
{
    delete ui;
}


// wyslanie polecenia pomiaru, odczytanie danych
void MainWindow::on_pushButton_pomiar_clicked()
{
    QByteArray requestData = "A";
    arduino->write(requestData);
    qDebug()<<"Data sent: "<<requestData;

    //porównanie z wilgotnoscia z bazy danych

    int moistLevel = ui->label_wynik->text().toInt();
    int goal_moist_level = ui->label_goalMoistLevel->text().toInt();
    if(moistLevel<goal_moist_level)
    {
        QMessageBox::information(this,"Za mało wody","Poziom wiglotności gleby dla wskazanej rośliny jest za niski",QMessageBox::Ok);
    }
    if(moistLevel>goal_moist_level)
    {
        QMessageBox::information(this,"Za dużo wody","Poziom wiglotności gleby dla wskazanej rośliny jest za wysoki",QMessageBox::Ok);
    }
    if(moistLevel==goal_moist_level)
    {
        QMessageBox::information(this,"Odpowiedni poziom wody","Poziom wiglotności gleby dla wskazanej rośliny jest odpowiedni",QMessageBox::Ok);
    }

}



void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    QString selectedText = ui->comboBox->currentText();
    ui->label_display_plant->setText(selectedText);

   //wyswietlanie goal moist  wybranej rzeczy w labelu
    QString queryStr = QString("SELECT moist FROM rosliny WHERE nazwa = '%1'").arg(selectedText);
    QSqlQuery query(queryStr,db);
    if(query.exec() && query.next()){
        int goalMoist = query.value(0).toInt();
        ui->label_goalMoistLevel->setText(QString::number(goalMoist));
    }
    else{
        qDebug()<<"Error executing query: "<<query.lastError().text();
    }

    //wyswietlanie obrazu

    QString queryIMG = QString("SELECT image FROM rosliny WHERE nazwa = '%1'").arg(selectedText);
    QSqlQuery queryIMG2(queryIMG,db);
    if(queryIMG2.exec() && queryIMG2.next())
    {
        QByteArray imageData = queryIMG2.value(0).toByteArray();

        QPixmap pixmap;
        pixmap.loadFromData(imageData);
        ui->label_obraz->setPixmap(pixmap.scaled(ui->label_obraz->size(),Qt::KeepAspectRatio));
        ui->label_obraz->setScaledContents(true);
    }
    else
    {
        qDebug()<<"Error executing query: "<<queryIMG2.lastError().text();
    }

}

void MainWindow::update(){

    //serialport setup
    arduino = new QSerialPort(this);
    arduino->setPortName("COM7");
    arduino->setBaudRate(QSerialPort::Baud9600);
    if(arduino->open(QIODevice::ReadWrite)){
        qDebug()<<"SERIAL PORT OPENED";
    }
    else
    {
        qDebug()<<"SERIAL PORT NOT OPENED";
    }
    connect(arduino,&QSerialPort::readyRead,this,[&](){
        QByteArray responseData = arduino->readAll();
        qDebug()<<"Rceived data: "<<responseData;
        ui->label_wynik->setText(responseData);
    });

    //Otwarcie bazy danych
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/Michał/Desktop/qt kurz/NawadnianieRoslin/bazadanych.db");
    if(!db.open()){
        qDebug()<<"Baza danych nie otwarta";
    }
    else{
        qDebug()<<"Baza danych otwarta";
    }
    //Wyslanie zapytania i wypelnienie comboboxa nazwami
    QSqlQuery query;
    query.exec("SELECT nazwa FROM rosliny");
    ui->comboBox->clear();
    while(query.next()){
        QString item = query.value(0).toString();
        ui->comboBox->addItem(item);
    }
    query.finish();


}


void MainWindow::on_pushButton_clicked()
{
    this->close();
}


void MainWindow::on_pushButton_2_clicked()
{
    dodawanie = new Dodawanie(this);
    dodawanie->show();


}


void MainWindow::on_pushButton_update_clicked()
{
    update();
}


void MainWindow::on_pushButton_edit_clicked()
{

    //w konstrukorze edit jest podawane id do ktorego ma sie odnosic edycja
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/Michał/Desktop/qt kurz/NawadnianieRoslin/bazadanych.db");
    if(!db.open()){
        qDebug()<<"Baza danych nie otwarta";
    }
    else{
        qDebug()<<"Baza danych otwarta";
    }
    QString queryID = QString("SELECT id FROM rosliny WHERE nazwa = '%1'").arg(ui->comboBox->currentText());
    QSqlQuery queryID2(queryID,db);
    if(queryID2.exec() && queryID2.next())
    {
        int idValue = queryID2.value(0).toInt();
        qDebug()<<"ID: "<<idValue;
        edycja = new edit(QString::number(idValue),this);
        edycja->show();
    }
    else
    {
        qDebug()<<"Error executing query: "<<queryID2.lastError().text();
    }


}

