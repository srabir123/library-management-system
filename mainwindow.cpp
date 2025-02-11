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

// Load book data from the file
void MainWindow::loadBooks()
{
    QFile file("books.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "No books file found. Starting fresh.";
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(", Shelf: ");
        if (parts.size() == 2) {
            QString bookName = parts[0].remove("Book: ").trimmed();
            QString shelfDetails = parts[1].trimmed();
            bookShelfMap.insert(bookName, shelfDetails);
        }
    }
    file.close();

    qDebug() << "Loaded books from file:" << bookShelfMap;
}
