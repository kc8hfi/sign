#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_formbase.h"

class QWebEngineView;
class QWebEngineProfile;

class MainWindow : public QMainWindow, private Ui_Form
{
     Q_OBJECT
     public:
          MainWindow(QMainWindow *parent=0);
          ~MainWindow();
          
     public slots:
          void timerEvent();
          void timerStart();

     private:
          //CustomFunctions
          void checkSettings();
          bool readServerConfig();
          bool initializeSettings();
          void logger(int,QString);
          void writeFile(QString);
          void loadPage();

          Ui_Form mw;
          
          //Objects
          QWebEngineProfile *profile;
          QWebEngineView *view;
          QTimer *timer;

          
          //Arrays
          QList<QString>documents;
          QList<int>duration;
          
          //Vars
          QString userHomeDir;
          QString serverConf;
          QString serverUrl;
          QString defaultPage;
          int timerDelay;
          int index;
          int logLevel;
          int defaultPageDuration;
          bool logToFile;
};

#endif  //MAINWINDOW_H
