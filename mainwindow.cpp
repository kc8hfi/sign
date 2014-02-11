#include "mainwindow.h"
#include <QPlainTextEdit>
#include <QWebView>
#include <QLabel>
#include <QTimer>
#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QDebug>

MainWindow::MainWindow(QMainWindow *parent)
        :QMainWindow(parent)
{
     mw.setupUi(this);
     
     view = new QWebView(this);
     
     setCentralWidget(view);
     
     //set up a new timer 
     timer = new QTimer(this);
     connect(timer, SIGNAL(timeout()), this, SLOT(change()));
     
     connect(view,SIGNAL(loadFinished(bool)),this, SLOT(checkPage()));     
     
     //set up the basic info for qsettings
     QCoreApplication::setOrganizationName("sign");
     QCoreApplication::setApplicationName("sign");
          
     timerDelay = 5000; //in milliseconds,  1000ms = 1s
     QSettings settings;
     timerDelay = settings.value("timer").toInt();
     
     //set the index number
     index = 0;
     
     //set up the initial stuff for where the pages live     
     pages = settings.value("pages").toString();
     location = settings.value("location").toString();
     
     //set up the default page
     defaultpage = pages + location + "default.html";     
     
     dbase = new Database();
     
     //pull all the stuff from the database
     checkSettings();
     
     //put this back later
     //showFullScreen();
}
MainWindow::~MainWindow()
{
     
}

void MainWindow::change()
{
     //qDebug()<<"timer fired:"<<index;
     if (index >= urls.size())
     {
          index = 0;
          //stop the timer
          timer->stop();
          //go get all the records again, just in case something changed
          checkSettings();
          //restart the timer
          timer->start(timerDelay);
     }
     if (urls.size() != 0)
     {
          //build a fancy string to put in here
          QString s = pages + location + urls.at(index) + ".html";
          //view->setHtml(s);
          view->load(QUrl(s));
     }
     else
     {
          //load the default page since there's nothing in the array
          view->load(QUrl(defaultpage));
     }
     
     index++;
}

void MainWindow::checkSettings()
{
     //fill up the list first
     dbase->getItems(&urls);

     //show something to start with?
     QSettings settings;
     pages = settings.value("pages").toString();
     location = settings.value("location").toString();
     defaultpage = pages + location + "default.html";
     view->load(QUrl(defaultpage));
     
     timer->start(timerDelay);        //in milliseconds,  1000ms = 1s
}

void MainWindow::checkPage()
{
     if (view->findText("Not Found"))     
     {
          //load the default page instead
          view->load(QUrl(defaultpage));
     }
}
