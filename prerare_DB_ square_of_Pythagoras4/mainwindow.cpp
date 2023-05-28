#include "mainwindow.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QtSql>
#include <QTextEdit>
#include <QGridLayout>

#include <QtWidgets>

//#define DEBUG

MainWindow::MainWindow(QWidget *parent)
{
    ///////// DB for SQLite open FIRST! /////////
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("sqlite.db");
        // SQLite not worked at in resorouce file !!!!!
        // https://forum.qt.io/topic/53992/sqlite-database-file-within-qrc/4
        // Technically it can be embedded, BUT you can't make the QtSql module read it even less write it.
        // What you can do however is put a copy of that "embedded" database in a suitable folder and then use it from your application.
        // To get a suitable folder, use QStandardPaths.
        // or worked:  db.setDatabaseName(":memory:");
    ////////////////////////////////////////////

    QGridLayout *bottomLayout = new QGridLayout;
    insertTextObjectButton = new QPushButton(tr("Insert Image"));
    textEdit = new QTextEdit;
    bottomLayout->addWidget(textEdit,0,0);
    bottomLayout->addWidget(insertTextObjectButton,1,0);
    setLayout( bottomLayout);

    setWindowTitle("Text Object Example");


    if (db.open()) {
            #ifdef DEBUG
                    qDebug() << "OK" << db.isValid() << db.lastError().text();
            #endif//

        QSqlQuery query(db);
        query.exec("CREATE TABLE db_ua_man ("
                   " man_id INTEGER PRIMARY KEY,"
                   " man_id_famil TEXT NOT NULL UNIQUE REFERENCES man_id );");

        QFile file(":/file_for_DB/man");
        file.open(QIODevice::ReadOnly | QIODevice::Text);

            if(file.exists())
            {
            #ifdef DEBUG
                        qDebug() << "file exist";
            #endif//
            //
                int i= 1;
                while (!file.atEnd()) {
                    QString line = file.readLine();
                    if (line != "\n")
                        {
                        QString str_query = "INSERT INTO db_ua_man (man_id, man_id_famil) "
                                            "VALUES (" +  QString::number(i) + ", '" + line + "');";
                        query.exec(str_query);  // qDebug() << str_query;
                        i++;
                        }
                }
            }
            file.close();
            #ifdef DEBUG
                    qDebug()  <<  query.exec("SELECT man_id  FROM db_ua_man") << " | " << query.exec("SELECT man_id_famil  FROM db_ua_man") ;
            #endif

    query.exec("SELECT * FROM db_ua_man");
    while (query.next()) {
        QString man_id = query.value(0).toString();
        QString man_id_famil = query.value(1).toString();
        textEdit->insertPlainText(man_id + " " + man_id_famil);
            #ifdef DEBUG
                    qDebug()  << query.value(0).toString() << query.value(1).toString();
            #endif
    }

    }
}

MainWindow::~MainWindow()
{
}
