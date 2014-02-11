#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>



class Database
{
     public:
          Database();
          ~Database();
          void getItems(QList<QString> *list);
          void makeConnection();
          
     private:
          QString username;
          QString password;
          QString host;
          QString dbase;
          QString location;
          
          QSqlDatabase db;
};

#endif  //DATABASE_H