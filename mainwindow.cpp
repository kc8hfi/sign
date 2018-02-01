#include "mainwindow.h"
#include <QPlainTextEdit>
#include <QWebView>
#include <QLabel>
#include <QTimer>
#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include <QStandardPaths>
#include <QDebug>


MainWindow::MainWindow(QMainWindow *parent)
        :QMainWindow(parent)
{
     mw.setupUi(this);
     
     //QCoreApplication::setApplicationName("sign");
     QCoreApplication::setOrganizationName("sign");
     
     view = new QWebView(this);
     
     setCentralWidget(view);
     
     //set up a new timer 
     timer = new QTimer(this);
     connect(timer, SIGNAL(timeout()), this, SLOT(change()));
     
     //connect(view,SIGNAL(loadFinished(bool)),this, SLOT(checkPage()));     
     
     //set up the basic info for qsettings
     QCoreApplication::setOrganizationName("sign");
     QCoreApplication::setApplicationName("sign");
          
     timerDelay = 30000; //in milliseconds,  1000ms = 1s
     
     //set the index number
     index = 0;

//      pull all the stuff from the database
     checkSettings();
}

MainWindow::~MainWindow()
{
     
}

void MainWindow::change()
{
     //qWarning()<<"timer fired:"<<index;
     //qWarning()<<urls.size();
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
          QString page = "http://intraweb.wvutech.edu/digital_sign_pages/" + urls.at(index);
          view->load(QUrl(page));
     }
//      else
//      {
//           //load the default page since there's nothing in the array
//           //view->load(QUrl(defaultpage));
//      }
     
     index++;
}

void MainWindow::checkSettings()
{
     QSettings settings;
     configFile = settings.value("config").toString();
     
     QString putHere = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).join('/') + "/.config/" + QCoreApplication::organizationName() + "/"+configFile;
//      qWarning()<<putHere;
     QString command = "wget http://intraweb.wvutech.edu/digital_sign_pages/" + configFile + " -O " + putHere;
     //qWarning()<<command;
     
     //get the configFile
     QProcess::execute(command);
     
     QString configFileName = configFile;
     if(QFile::exists(putHere))
     {
          QFile file(putHere);
          QStringList fields;
          if (file.open(QIODevice::ReadOnly))
          {
               QTextStream in(&file);
               while (!in.atEnd())
               {
                    QString line = in.readLine();
                    fields = line.split(";");
               }
          }
          else
          {
               qWarning()<<"couldn't open the file";
          }
          
          QString s;
          //empty out the url list
          urls.clear();
          foreach(s,fields)
          {
               QStringList item = s.split(',');
               //load the webpage into the list of urls to cycle through
               defaultPage = settings.value("default").toString();
               if (item[0] != defaultPage)
               {
                    urls.append(item[0]);
                    //load the time that the url should be displayed
                    howlong.append(item[1]);
                    //QString url = "http://intraweb.wvutech.edu/digital_sign_pages/" + item[0];
                    //qWarning()<<url;
                    //view->load(QUrl(url));
               }
               else
               {
                    defaultPage = "http://intraweb.wvutech.edu/digital_sign_pages/" + item[0];
               }
          }
          //qWarning()<<"how many pages: "<<urls.size();
     }
     else
          qWarning()<<"list of items not there!";
     
     
     if(urls.size() == 0)
     {
          //show something, just in case there's a problem
          //qWarning()<<"nothing in the urls";
          defaultPage = "http://intraweb.wvutech.edu/digital_sign_pages/default.html";
          view->load(QUrl(defaultPage));
     }
     else
     {
          QString page = "http://intraweb.wvutech.edu/digital_sign_pages/" + urls.at(index);
          view->load(QUrl(page));
     }

     
     //start the timer
     timer->start(timerDelay);     //in milliseconds, 1000ms = 1s
}
