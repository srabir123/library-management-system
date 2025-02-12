
#include <QMainWindow>
#include <QSet>
#include <QMap>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void button1(); // Book access check
    void onRegisterButtonClicked(); // Show registration fields
    void onConfirmButtonClicked(); // Register roll numbers
    void on_AddBookButton_clicked(); // Show add book fields
    void on_ADDButton_clicked(); // Add book to library
    void on_CHECKAVAILABILYbutton_clicked(); // Handles the CHECK AVAILABILITY button click
    void checkBookAvailability(const QString &bookName); // Checks book availability in the library
    void on_searchbutton_clicked(); // Handles the search button click (NEW)


private:
    Ui::MainWindow *ui;
    QSet<int> registeredRolls; // Stores registered roll numbers
    QMap<QString, QString> bookShelfMap; // Stores book name and shelf details

    void loadRegisteredRolls(); // Load registered rolls from file
    void saveRegisteredRolls(); // Save registered rolls to file
    void loadBooks(); // Load books from file
};

#endif // MAINWINDOW_H

