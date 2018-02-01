#include "mainwindow.h"
#include <QPlainTextEdit>
//#include <QWebView>
#include <QWebEngineView>
#include <QWebEngineProfile>
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
     
     genericPage = "";
     defaultPage = "";
     
     //view = new QWebView(this);
     view = new QWebEngineView(this);
     
     profile = new QWebEngineProfile(view);
     profile->clearHttpCache();
     
     
     setCentralWidget(view);
     
     //set up a new timer 
     timer = new QTimer(this);
     connect(timer, SIGNAL(timeout()), this, SLOT(change()));
     
     //connect(view,SIGNAL(loadFinished(bool)),this, SLOT(checkPage()));     
     
     //set up the basic info for qsettings
     QCoreApplication::setOrganizationName("sign");
     QCoreApplication::setApplicationName("sign");
          
     timerDelay = 5000; //in milliseconds,  1000ms = 1s
     
     //set the index number
     index = 0;

     //get the config file, pages and everything to start with
     checkSettings();
     
     //start the timer
     timer->start(timerDelay);
}

MainWindow::~MainWindow()
{
     
}

void MainWindow::checkSettings()
{
     //stop the timer first
     timer->stop();
     //lets go get the configuration file
     QSettings settings;
     defaultPage = settings.value("default").toString();
     QString configFileName = settings.value("config").toString();
     //qWarning()<<"config file: " << configFileName;
     QString putHere = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).join('/') + "/.config/" + QCoreApplication::organizationName() + "/"+configFileName;
     genericPage = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).join('/') + "/.config/" + QCoreApplication::organizationName() + "/"+defaultPage;
     //qWarning()<<putHere;
     QString command = "wget https://intraweb.wvutech.edu/digital_sign_pages/" + configFileName + " -O " + putHere;

     //execute the wget command to pull down the configuration file
     QProcess::execute(command);
     
     //check and see if the file actually exists first
     if (QFile::exists(putHere))
     {
          //open the configuration file
          QFile file(putHere);
          QStringList fields;
          if(file.open(QIODevice::ReadOnly))
          {
               QTextStream incoming(&file);
               while(!incoming.atEnd())
               {
                    QString line = incoming.readLine();
                    fields = line.split(";");
               }//end while
          }//end file opening
          else
          {
               //couldn't open the file, better log it!!
               qWarning()<<"could not open file: "<<putHere;
               view->setUrl(QUrl(genericPage));
          }
          //reset the index to start back at 0
          index = 0;
          //empty out the list of files
          urls.clear();
          
          //clear the cache 
          profile->clearHttpCache();
          
          QString s;
          foreach(s,fields)
          {
               QStringList item = s.split(',');
               urls.append(item[0]);
               howlong.append(item[1]);
          }//end for loop
          
          //now, we need to display the first page
          if(urls.size() !=0)
          {
               //QUrl u = getPage(urls.at(index));
               QUrl u = QString("https://intraweb.wvutech.edu/digital_sign_pages/" + urls.at(index));
               view->setUrl(u);
          }
          else
          {
               //there was nothing in the urls, so display a generic page
               view->setUrl(QUrl(genericPage));
          }
     }//end file was there
     else
     {
          qWarning()<<"file "<< putHere <<"was not there!";
          view->setUrl(QUrl(genericPage));
     }
     qWarning()<<"checkSettings() showing: "<<view->url();
     view->show();
     //restart the timer
     timer->start(timerDelay);
}


void MainWindow::change()
{
     qWarning()<<"timer went off, current index:"<<index;
     //clear the cache
     profile->clearHttpCache();
     if (index >= urls.size()) //need to restart everything!
     {
          index = 0;
          timer->stop();
          //get the settings again
          checkSettings();
          //restart the timer
          timer->start(timerDelay);
     }
     
     if (urls.size() != 0)    //we got stuff to show
     {
          QString p = "https://intraweb.wvutech.edu/digital_sign_pages/" + urls.at(index);
          view->setUrl(QUrl(p));
          index++;
     }
     else //might want to show the default page since there's nothing to show
     {
          view->setUrl(QUrl(genericPage));
     }
     qWarning()<<"showing: "<<view->url();
     view->show();
}//end change
