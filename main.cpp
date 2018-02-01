#include <QApplication>
#include "mainwindow.h"

int main (int argc, char *argv[])
{
     QApplication app(argc,argv);

     //hide the cursor, put this back later
     qApp->setOverrideCursor(QCursor(Qt::BlankCursor));
     
     MainWindow w;

     w.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

     w.show();
     return app.exec();
}
