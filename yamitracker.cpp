#include "yamitracker.h"
#include "ui_yamitracker.h"

Yamitracker::Yamitracker(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Yamitracker)
{
    ui->setupUi(this);
}

Yamitracker::~Yamitracker()
{
    delete ui;
}

