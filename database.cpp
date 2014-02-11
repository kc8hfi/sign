
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSettings>
#include <QList>
#include <QString>
#include <QVariant>
#include <QDebug>
#include "database.h"

Database::Database()
{
     db = QSqlDatabase::addDatabase("QMYSQL");
     makeConnection();
}

Database::~Database()
{
     
}

void Database::makeConnection()
{
     QSettings settings;
     
     username = settings.value("user").toString();
     password = settings.value("pass").toString();
     host = settings.value("host").toString();
     dbase = settings.value("dbase").toString();
     location = settings.value("location").toString();
     
     db.setDatabaseName(dbase);
     db.setUserName(username);
     db.setPassword(password);
     db.setHostName(host);
     int counter = 0;
     bool isopen = false;
     while(!isopen && counter < 2)
     {
          isopen = db.open();
          counter++;          
     }

     if (!db.open())
     {
          qDebug()<<db.lastError();
     }
}

void Database::getItems(QList<QString> *list)
{
     //qDebug()<<"how big: "<<list->size();
     //empty the list
     list->clear();
     QString q = "select data_order from digital_sign_data ";
     q += "where data_location_id = :location ";
     q += "order by data_order";
     QSqlQuery query;
     
     if (!db.isOpen())
     {
          //qDebug()<<"no dbase connection";
          makeConnection();
     }
     else
     {
          query.prepare(q);
          query.bindValue(":location",(QVariant)location);
          query.exec();
          while(query.next())
          {
               //qDebug()<<query.value(0).toString();
               list->append(query.value(0).toString());
          }
     }
     
}
