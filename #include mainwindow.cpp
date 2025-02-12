#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDebug>  // For debugging output
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
    ui->enternameofbook->setVisible(false); 
    ui->enternameofbook_2->setVisible(false); 
    ui->searchbutton->setVisible(false);


    
    loadRegisteredRolls();
    loadBooks(); 

    // Connect buttons to their slots
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::button1);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::close);
    connect(ui->registerButton, &QPushButton::clicked, this, &MainWindow::onRegisterButtonClicked);
    connect(ui->confirmButton, &QPushButton::clicked, this, &MainWindow::onConfirmButtonClicked);
    connect(ui->AddBookButton, &QPushButton::clicked, this, &MainWindow::on_AddBookButton_clicked);
    connect(ui->ADDButton, &QPushButton::clicked, this, &MainWindow::on_ADDButton_clicked);
    connect(ui->CHECKAVAILABILYbutton, &QPushButton::clicked, this, &MainWindow::on_CHECKAVAILABILYbutton_clicked);
    // Connect search button to checkBookAvailability (NEW)
    connect(ui->searchbutton, &QPushButton::clicked, this, &MainWindow::on_searchbutton_clicked);


}


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


MainWindow::~MainWindow()
{
    delete ui;
}

// Load registered rolls from a file
void MainWindow::loadRegisteredRolls()
{
    QFile file("registered_rolls.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        bool ok;
        int roll = line.toInt(&ok);
        if (ok) {
            registeredRolls.insert(roll);
        }
    }
    file.close();
}

// Save registered rolls to a file
void MainWindow::saveRegisteredRolls()
{
    QFile file("registered_rolls.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Failed to save registered roll numbers.");
        return;
    }

    QTextStream out(&file);
    for (int roll : registeredRolls) {
        out << roll << "\n";
    }
    file.close();
}


void MainWindow::button1()
{
    QString ubook = ui->lineEdit_2->text().trimmed();
    QString uRollNo = ui->lineEdit->text().trimmed();

    if (uRollNo.isEmpty() || ubook.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter both Roll Number and Book Name.");
        return;
    }

    bool ok;
    int uRollNo_int = uRollNo.toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Input Error", "Invalid Roll Number entered.");
        return;
    }

    if (!registeredRolls.contains(uRollNo_int)) {
        QMessageBox::information(this, "Error", "Roll Number is not registered. Please register first.");
        return;
    }

    // Check if the book exists in the library map
    if (bookShelfMap.contains(ubook)) {
        QString shelfDetails = bookShelfMap.value(ubook);
        QMessageBox::information(this, "Library Management",
                                 QString("The book '%1' is available in self %2.")
                                 .arg(ubook)
                                 .arg(shelfDetails));
    } else {
        QMessageBox::information(this, "Library Management",
                                 QString("The book '%1' is not available in the library.")
                                 .arg(ubook));
    }
}



void MainWindow::onRegisterButtonClicked()
{
    ui->studentIdLineEdit->setVisible(true);
    ui->pipilikaIdLineEdit->setVisible(true);
    ui->confirmButton->setVisible(true);

    // Show additional input fields
    ui->studentIdLineEdit_2->setVisible(true);
    ui->pipilikaIdLineEdit_2->setVisible(true);

    QMessageBox::information(this, "Registration", "Enter Student IDs and Pipilika IDs, then click Confirm.");
}

void MainWindow::onConfirmButtonClicked()
{
    QString studentId = ui->studentIdLineEdit->text().trimmed();
    QString pipilikaId = ui->pipilikaIdLineEdit->text().trimmed();

    qDebug() << "Entered Student ID: " << studentId;
    qDebug() << "Entered Pipilika ID: [" << pipilikaId << "]";

    if (studentId.isEmpty() || pipilikaId.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter both Student ID and Pipilika ID.");
        return;
    }

    // Define valid Pipilika ID
    const QString validPipilikaID = "61008";

    if (pipilikaId.trimmed() == validPipilikaID) {  
        bool ok;
        int roll = studentId.toInt(&ok);

        qDebug() << "Converted Roll Number: " << roll;

        if (!ok) {
            QMessageBox::warning(this, "Input Error", "Invalid Student ID. It must be a number.");
            return;
        }

        // Check if roll number is already registered
        if (!registeredRolls.contains(roll)) {
            registeredRolls.insert(roll);
            saveRegisteredRolls();

            QMessageBox::information(this, "Registration Successful",
                                     QString("Student ID %1 has been registered successfully.").arg(roll));
        } else {
            QMessageBox::information(this, "Registration", "This Student ID is already registered.");
        }

        ui->studentIdLineEdit->clear();
        ui->pipilikaIdLineEdit->clear();
        ui->studentIdLineEdit->setVisible(false);
        ui->pipilikaIdLineEdit->setVisible(false);
        ui->confirmButton->setVisible(false);

    } else {
        QMessageBox::warning(this, "Registration Failed", "Invalid Pipilika ID entered.");
    }
}



void MainWindow::on_AddBookButton_clicked()
{
    ui->EnterBooknameButton->setVisible(true);
    ui->selfnumberanshelfbutton->setVisible(true);
    ui->ADDButton->setVisible(true);

    // Show additional fields
    ui->EnterBooknameButton_2->setVisible(true);
    ui->selfnumberanshelfbutton_2->setVisible(true);

    QMessageBox::information(this, "Add Book", "Enter Book Names and Shelf Details, then click ADD.");
}

void MainWindow::on_ADDButton_clicked()
{
    QString bookName = ui->EnterBooknameButton_2->text().trimmed(); // Use the correct object name
    QString shelfDetails = ui->selfnumberanshelfbutton_2->text().trimmed(); // Use the correct object name

    // Debugging output
    qDebug() << "Book Name Field Text: " << ui->EnterBooknameButton_2->text();
    qDebug() << "Shelf Details Field Text: " << ui->selfnumberanshelfbutton_2->text();
    qDebug() << "Trimmed Book Name: " << bookName;
    qDebug() << "Trimmed Shelf Details: " << shelfDetails;

    // Input validation
    if (bookName.isEmpty() || shelfDetails.isEmpty() ||
        bookName == "Enter Book name" || shelfDetails == "Self Number and Shelf") {
        QMessageBox::warning(this, "Input Error", "Please enter valid Book Name and Shelf Details.");
        return;
    }

    // Update map and save to file
    bookShelfMap.insert(bookName, shelfDetails);

    QFile file("books.txt");
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Failed to save book details.");
        return;
    }

    QTextStream out(&file);
    out << "Book: " << bookName << ", Shelf: " << shelfDetails << "\n";
    file.close();

    QMessageBox::information(this, "Book Addition", QString("The book '%1' has been added to Self '%2' successfully.").arg(bookName, shelfDetails));


    // Clear fields
    ui->EnterBooknameButton_2->clear();
    ui->selfnumberanshelfbutton_2->clear();
}

void MainWindow::on_CHECKAVAILABILYbutton_clicked()
{
    ui->enternameofbook->setVisible(true);
    ui->enternameofbook_2->setVisible(true);
    ui->searchbutton->setVisible(true);

    QMessageBox::information(this, "Check Availability", "Enter the book name and click search to check availability.");
}


void MainWindow::checkBookAvailability(const QString &bookName)
{
    int copyCount = 0;
    QFile file("books.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.contains(bookName, Qt::CaseInsensitive)) {
                copyCount++;
            }
        }
        file.close();
    }

    if (copyCount > 0) {
        QMessageBox::information(this, "Book Availability", QString("The book '%1' is available with %2 copies.").arg(bookName).arg(copyCount));
    } else {
        QMessageBox::information(this, "Book Availability", QString("The book '%1' is not available in the library.").arg(bookName));
    }
}


void MainWindow::on_searchbutton_clicked()
{
    QString bookName = ui->enternameofbook_2->text().trimmed();
    if (bookName.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter the name of the book.");
        return;
    }
    checkBookAvailability(bookName);  // Calls the availability check function
}

