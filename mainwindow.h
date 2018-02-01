#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_formbase.h"

//class QWebView;
class QWebEngineView;
class QWebEngineProfile;

class MainWindow : public QMainWindow, private Ui_Form
{
     Q_OBJECT
     public:
          MainWindow(QMainWindow *parent=0);
          ~MainWindow();
          void checkSettings();
          
     public slots:
          void change();
          //void checkPage();
     private:
          Ui_Form mw;
          //QWebView *view;
          
          QWebEngineProfile *profile;
          QWebEngineView *view;
          
          //QWebView *testview;
          QTimer *timer;
          int timerDelay;
          
          QList<QString>urls;
          QList<QString>howlong;
          int index;
          
          //QString location;
          //QString pages;   
          QString defaultPage;
          QString genericPage;
          
          //QUrl getPage(QString);
          

};

#endif  //MAINWINDOW_H
