#include "mainwindow.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QtSql>
#include <QTextEdit>
#include <QGridLayout>

#include <QtWidgets>

//#include "mymodel.h"

MainWindow::MainWindow(QWidget *parent)
{
    QGridLayout *bottomLayout = new QGridLayout;
    insertTextObjectButton = new QPushButton(tr("Insert Image"));
    textEdit = new QTextEdit;
    bottomLayout->addWidget(insertTextObjectButton,1,0);
    bottomLayout->addWidget(textEdit,0,0);
    setLayout( bottomLayout);

    setWindowTitle("Text Object Example");


    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    //db.setDatabaseName("/Users/srtsrthwqethqerthsrthwryjwrtsdflly/Documents/1sqlite333.sqlite");
    db.setDatabaseName("sqlite.sqlite");

if (db.open()) { qDebug() << "OK" << db.isValid() << db.lastError().text(); }

QFile file("/Users/<user_folder>/Documents/man");
QSqlQuery query(db);
query.exec("CREATE TABLE db_ua_man ( man_id integer PRIMARY KEY,    man_id_famil text REFERENCES man_id  );");

if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

if(file.exists())
{
    qDebug() << "exist";
    int i= 0;
    while (!file.atEnd()) {
        QString line = file.readLine();
        QString str_query = "INSERT INTO db_ua_man (man_id, man_id_famil) VALUES (" +  QString::number(i) + ", '" + line + "');";
        query.exec(str_query);
        qDebug() << str_query;
        i++;
    }
}
file.close();

qDebug()  <<  query.exec("SELECT man_id  FROM db_ua_man") << " | " << query.exec("SELECT man_id_famil  FROM db_ua_man") ;

    query.exec("SELECT *  FROM db_ua_man");
    while (query.next()) {
        QString country = query.value(0).toString();
        qDebug()  << query.value(0).toString() << query.value(1).toString();;
        textEdit->insertPlainText(country + " ");
        QString man_id_famil = query.value(1).toString();
        textEdit->insertPlainText(man_id_famil + "\n");
    }
}

MainWindow::~MainWindow()
{
}
