#include "mainwindow.h"
#include <QPlainTextEdit>
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


#define verbose 10
#define errorOnly 0
#define fullInfo 0
#define error -1
#define warning -2
#define alert -3
#define dev -99


MainWindow::MainWindow(QMainWindow *parent)
        :QMainWindow(parent)
{
     mw.setupUi(this);

     //Default Debug Settings. Set in Config file to overwrite
     logLevel=verbose;      //Default Level is Verbose
     logToFile=true;        //Default Level is to log to file


     //Variables
     userHomeDir = QStandardPaths::locate(QStandardPaths::HomeLocation, QString(), QStandardPaths::LocateDirectory);    //Default path to user home
     serverUrl = "";                // Url to destination folder; Must end with /
     defaultPage = "";              // Default page to show in the event of a 404
     defaultPageDuration = 0;      // Length of time to show default page in the event of a 404
     timerDelay = 5000;             // in milliseconds,  1000ms = 1s
     index = 0;                     // set array index to be 0

     //Set application name
     QCoreApplication::setOrganizationName("sign");
     QCoreApplication::setApplicationName("sign");

     //Initialize the application and if successful start timer.
     if(initializeSettings())
     {
         //Create web-engine for displaying the page
         view = new QWebEngineView(this);

         //Errata solution: Make sure we always get the most current page
         view->page()->profile()->setCachePath(userHomeDir + ".config/sign/tmp/browserCache");
         view->page()->profile()->setHttpCacheType(QWebEngineProfile::NoCache);
         view->page()->profile()->setPersistentCookiesPolicy(QWebEngineProfile::NoPersistentCookies);

         //Set display to the view
         setCentralWidget(view);

         //Create Timer
         timer = new QTimer(this);
         connect(timer, SIGNAL(timeout()), this, SLOT(timerEvent()));
         connect(timer, SIGNAL(timeout()), this, SLOT(timerStart()));
         timer->start(timerDelay);

         //Load initial page
         loadPage();
     }
     else
     {
         exit(EXIT_FAILURE);
     }
}

MainWindow::~MainWindow()
{

}

bool MainWindow::initializeSettings()
{
    bool status=false;

    logger(alert, "\nAlert:\tProgram Starting\n");

    //Check configfile exists if not, note and move on.
    if (!(QFile(userHomeDir + ".config/sign/sign.conf").exists()))
    {
        logger(error,"ERROR:\tMissing config file. Does ~/.config/sign/sign.conf exist?");
        return false;
    }

    //Go get the config settings
    QSettings appSettings;
    serverUrl = appSettings.value("serverUrl").toString();
    defaultPage = appSettings.value("defaultPage").toString();
    defaultPageDuration = appSettings.value("defaultDuration").toInt();
    serverConf = appSettings.value("serverConf").toString();
    logLevel = appSettings.value("logLevel").toInt();
    logToFile = appSettings.value("logToFile").toBool();

    //Read the server Config
    if(readServerConfig())
    {
        status=true;
    }

    return status;
}

bool MainWindow::readServerConfig()
{
    //Download the server cfg file
    logger(fullInfo,"\nInfo:\tFetching Config File");
    logger(fullInfo,"Info:\twget " + serverUrl + serverConf + " -O " + userHomeDir+".config/sign/tmp/"+serverConf);

    QString command = "wget " + serverUrl + serverConf + " -O " + userHomeDir+".config/sign/tmp/"+serverConf;

    //Create process to execute our command
    QProcess::execute(command);

    logger(fullInfo,"Info:\tFinished fetching Config File");

    //Create file object
    QFile serverConfigFile(userHomeDir+".config/sign/tmp/"+serverConf);

    //Temp Variables
    QString line;
    QList<QString> tempDataSet;
    QList<QString> tempSplit;

    //If File does not exist leave.
    if (!(serverConfigFile.exists()))
    {
        logger(error,"Error:\tFile not found -- " + userHomeDir + ".config/sign/tmp/" + serverConf);
        return false;
    }

    //It exists so open it
    serverConfigFile.open(QIODevice::ReadOnly);

    //Note: Server config is expected to be one line and one line only so we do not need to loop
    line=serverConfigFile.readLine().trimmed();

    //Close file
    serverConfigFile.close();

    logger(fullInfo,"Info:\tServer config provided: " + line);

    //Iterate through the pairs, splitting them and placing them in their respective arrays;
    if(!line.isEmpty() && !line.isNull())
    {
        //Split on the comma to get the data pairs
        tempDataSet=line.split(";");

        for(int i=0;i<tempDataSet.count();i++)
        {
            tempSplit=tempDataSet.at(i).split(",");
            documents.append(tempSplit.at(0));
            duration.append(tempSplit.at(1).toInt());
        }

        //If were reaaaaly curious, then print contents of array when verbose is highest
        if(verbose==10)
        {
            QString serverPages;
            QString serverTimes;
            for(int j=0;j<documents.count();j++)
            {
                serverPages= serverPages + "[" + QString::number(j)  + "] => " + documents.at(j) + ", ";
                serverTimes= serverTimes + "[" + QString::number(j) + "] => " + QString::number(duration.at(j)) + ", ";
            }

            logger(dev,"Dev:\tContents of documents array:\t\t" + serverPages);
            logger(dev,"Dev:\tContents of duration array:\t\t" + serverTimes);
        }
    }
    else
    {
        logger(false,"Alert:\tServer config was empty!.");
    }

    logger(false,"Info:\tCompleted parsing of server config file.");


    return true;
}

void MainWindow::logger(int errLevel, QString logText)
{
    QString formattedEntry="";

    //If it starts with \n then adjust for formatting else Format the log text with date code
    if(logText.startsWith("\n"))
    {
        logText=logText.trimmed();
        formattedEntry = "\n" + QTime::currentTime().toString() + "\t" + logText;
    }
    else
    {
        formattedEntry = QTime::currentTime().toString() + "\t" + logText;
    }

    //Print as much as possible
    if(verbose==10)
    {
        if(errLevel==error)
        {
            qDebug().noquote()<< "\e[31m" + formattedEntry +  "\e[97m";
        }
        else if(errLevel==alert)
        {
            qDebug().noquote()<< "\e[91m" + formattedEntry +  "\e[97m";
        }
        else if(errLevel==warning)
        {
            qDebug().noquote()<< "\e[93m" + formattedEntry +  "\e[97m";
        }
        else if(errLevel==dev)
        {
            qDebug().noquote()<< "\e[96m" + formattedEntry +  "\e[97m";
        }
        else
        {
            qDebug().noquote()<< "\e[97m" + formattedEntry + "\e[97m";
        }
    }
    else
    {
        //Only Print error text
        if(errLevel==error)
        {
            qDebug().noquote()<< "\e[31m" + formattedEntry + "\e[97m";
        }
    }

    //If set, log it to a file
    if(logToFile)
    {
        writeFile(formattedEntry);
    }
}

void MainWindow::writeFile(QString outputText)
{
    //Open the log file
    QFile outFile(userHomeDir+".config/sign/log/sign.log");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);

    //If not open then we have a problem
    if(!outFile.isOpen())
    {
        qDebug().noquote()<< "\033[91m" + QTime::currentTime().toString() + "\tError: Unable to open: " + userHomeDir + ".config/sign/log/sign.log \e[97m";
        qDebug().noquote()<< "\033[91m" + QTime::currentTime().toString() + "\tApplication will now exit! \e[97m \n";
        exit(EXIT_FAILURE);
    }
    else
    {
        //Create text writer
        QTextStream writeToFile(&outFile);

        //Add to file with timestamp
        writeToFile << outputText.append("\n");

        //Close File
        outFile.close();
    }
}

void MainWindow::loadPage()
{
    //If the arrays are empty then we just show the default page else, load the page at the current idex
    //and display the page for the duration in the duration array at the current index
    if(documents.count()==0)
    {

        view->load(QUrl(serverUrl + defaultPage));
        timer->setInterval(defaultPageDuration*1000);
        logger(warning,"Warning:\tNo documents from server. Loading default page!");
    }
    else
    {

        view->load(QUrl(serverUrl + documents.at(index)));
        timer->setInterval(duration.at(index)*1000);
        logger(fullInfo,"Info:\tDisplaying: " + documents.at(index) + " for " + QString::number(duration.at(index)) + " seconds. Current value of index: " + QString::number(index));
        index++;
    }
}

void MainWindow::timerStart()
{
    //Dont do anything here
    //Was going to call load page to load the default since at this point the
    //document array is empty. When using timer->setInterval() it would appear
    //that the timer is destroyed and re-created with its previous state.
    //As such a running timer that the interval is updated on will trigger this
    //event and cauase a double page load to occur. Effectively displaying
    //every other page instead of the desired page.
}

void MainWindow::timerEvent()
{
    //Check to see if we've reached the end of our arrays. If so, clean up and get new data
    if(index>=documents.count())
    {
        index=0;
        documents.clear();
        duration.clear();

        readServerConfig();
    }

    loadPage();

}//end change
