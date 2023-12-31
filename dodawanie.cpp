#include "dodawanie.h"
#include "ui_dodawanie.h"

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
