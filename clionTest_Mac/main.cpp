#include <iostream>
#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QtSql>

int main(int argc, char **argv)
{
    std::cout << "Hello, World!" << std::endl;

    QApplication app(argc, argv);
    QWidget w;
    w.setWindowTitle("title");

    QLabel *label = new QLabel("Enter name");
    QLineEdit *lineEdit = new QLineEdit("Type here...");
    QHBoxLayout *layout = new QHBoxLayout();
    w.setLayout((QLayout*) layout);     //Typecast to superclass
    layout->addWidget(label);
    layout->addWidget(lineEdit);

    w.show();

    return app.exec();
}