#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDebug> // For debugging output
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

 // Hide all initially unnecessary UI elements
    ui->studentIdLineEdit->setVisible(false);
    ui->pipilikaIdLineEdit->setVisible(false);
    ui->confirmButton->setVisible(false);
    ui->EnterBooknameButton->setVisible(false);
    ui->selfnumberanshelfbutton->setVisible(false);
    ui->ADDButton->setVisible(false);
    ui->studentIdLineEdit_2->setVisible(false);
    ui->pipilikaIdLineEdit_2->setVisible(false);
    ui->EnterBooknameButton_2->setVisible(false);
    ui->selfnumberanshelfbutton_2->setVisible(false);

