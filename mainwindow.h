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
          void checkSettings();
          
     public slots:
          void change();

     private:
          Ui_Form mw;
          
          
          QWebEngineProfile *profile;
          QWebEngineView *view;
          
          QTimer *timer;
          int timerDelay;
          
          QList<QString>urls;
          QList<QString>howlong;
          int index;
          
          QString defaultPage;
          QString genericPage;
};

#endif  //MAINWINDOW_H
