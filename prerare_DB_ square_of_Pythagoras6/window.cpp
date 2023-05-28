#include <QtWidgets>
#include <QtSql>

#include "window.h"

#define DEBUG

//! [Set up widgets]
Window::Window(QWidget *parent)
    : QWidget(parent)
{
    setupModel();

    nameLabel = new QLabel(tr("Na&me:"));
    nameEdit = new QLineEdit();
    famileLabel = new QLabel(tr("&Famile:"));
    famileEdit = new QLineEdit();
    typeLabel = new QLabel(tr("&Type:"));
    typeComboBox = new QComboBox();
    nextButton = new QPushButton(tr("&Next"));
    previousButton = new QPushButton(tr("&Previous"));

    nameLabel->setBuddy(nameEdit);
    famileLabel->setBuddy(famileEdit);
    typeLabel->setBuddy(typeComboBox);
//! [Set up widgets]

//! [Set up the mapper]
    QSqlTableModel *relModel = model->relationModel(typeIndex);
    typeComboBox->setModel(relModel);
    typeComboBox->setModelColumn(relModel->fieldIndex("description"));

    mapper = new QDataWidgetMapper(this);
    mapper->setModel(model);
    mapper->setItemDelegate(new QSqlRelationalDelegate(this));
    mapper->addMapping(nameEdit, model->fieldIndex("name"));
    mapper->addMapping(famileEdit, model->fieldIndex("famile"));
    mapper->addMapping(typeComboBox, typeIndex);
//! [Set up the mapper]

//! [Set up connections and layouts]
    connect(previousButton, &QPushButton::clicked,
            mapper, &QDataWidgetMapper::toPrevious);
    connect(nextButton, &QPushButton::clicked,
            mapper, &QDataWidgetMapper::toNext);
    connect(mapper, &QDataWidgetMapper::currentIndexChanged,
            this, &Window::updateButtons);

    QGridLayout *layout = new QGridLayout();
    layout->addWidget(nameLabel, 0, 0, 1, 1);
    layout->addWidget(nameEdit, 0, 1, 1, 1);
    layout->addWidget(previousButton, 0, 2, 1, 1);
    layout->addWidget(famileLabel, 1, 0, 1, 1);
    layout->addWidget(famileEdit, 1, 1, 2, 1);
    layout->addWidget(nextButton, 1, 2, 1, 1);
    layout->addWidget(typeLabel, 3, 0, 1, 1);
    layout->addWidget(typeComboBox, 3, 1, 1, 1);
    setLayout(layout);

    setWindowTitle(tr("SQL Widget Mapper"));
    mapper->toFirst();
}
//! [Set up connections and layouts]

//! [Set up the main table]
void Window::setupModel()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    if (!db.open()) {
        QMessageBox::critical(0, tr("Cannot open database"),
            tr("Unable to establish a database connection.\n"
               "This example needs SQLite support. Please read "
               "the Qt SQL driver documentation for information how "
               "to build it."), QMessageBox::Cancel);
        return;
    }

    QSqlQuery query;
    query.exec("create table person (id int primary key, "
               "name varchar(20), famile varchar(200), typeid int)");
//    query.exec("insert into person values(1, 'Alice', "
//               "'<qt>123 Main Street<br/>Market Town</qt>', 101)");

    QFile file(":/file_for_DB/man");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QFile fileppp(":/file_for_DB/popularnist-prizvis_in_ua");
    fileppp.open(QIODevice::ReadOnly | QIODevice::Text);

    if(file.exists())
    {
                    #ifdef DEBUG
                        qDebug() << "file exist";
                    #endif
        int i= 1;
        while (!file.atEnd()) {
            QString line = file.readLine();
            //if (!fileppp.atEnd())
                QString lineppp = fileppp.readLine();
            if (line != "\n")
            {
                if (i%5) query.exec("insert into person values(" + QString::number(i) + ", '" + line + "','" + lineppp + "', 101)");
                if (i%2) query.exec("insert into person values(" + QString::number(i) + ", '" + line + "','" + lineppp + "', 102)");
                if (i%3) query.exec("insert into person values(" + QString::number(i) + ", '" + line + "','" + lineppp + "', 103)");
                i++;
            }
        }
    }
    file.close();
    fileppp.close();

    query.exec("create table familetype (id int, description varchar(20))");
    query.exec("insert into familetype values(101, 'Home')");
    query.exec("insert into familetype values(102, 'Work')");
    query.exec("insert into familetype values(103, 'Other')");

    model = new QSqlRelationalTableModel(this);
    model->setTable("person");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    typeIndex = model->fieldIndex("typeid");

    model->setRelation(typeIndex,
           QSqlRelation("familetype", "id", "description"));
    model->select();
}
//! [Set up the famile type table]

//! [Slot for updating the buttons]
void Window::updateButtons(int row)
{
    previousButton->setEnabled(row > 0);
    nextButton->setEnabled(row < model->rowCount() - 1);
}
//! [Slot for updating the buttons]
