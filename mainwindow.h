#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_formbase.h"
#include "database.h"

class QWebView;

class MainWindow : public QMainWindow, private Ui_Form
{
     Q_OBJECT
     public:
          MainWindow(QMainWindow *parent=0);
          ~MainWindow();
          void checkSettings();
          
     public slots:
          void change();
          void checkPage();
     private:
          Ui_Form mw;
          QWebView *view;
          QWebView *testview;
          QTimer *timer;
          int timerDelay;
          
          QList<QString>urls;
          int index;
          
          QString location;
          QString pages;   
          QString defaultpage;
          
          Database *dbase;
};

#endif  //MAINWINDOW_H