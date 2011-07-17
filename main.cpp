/* ======================================================================
**  main.cpp
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2009 by Max Schlee
** ======================================================================
*/

#include <QtGui>
#include "EMP.h"

// ----------------------------------------------------------------------
int main(int argc, char** argv)
{
//    Q_INIT_RESOURCE(EMP);

    QApplication app(argc, argv);
    app.setApplicationName("EMP");
    app.setQuitOnLastWindowClosed(true);

    QString fileString = app.arguments().value(1);
    MediaPlayer  mediaPlayer(fileString);

    QString dirString = app.applicationDirPath();
    fileString = dirString + "/Style/EMP.qss";
    QFile file(fileString);
    file.open(QFile::ReadOnly);
    QString strCSS = QLatin1String(file.readAll());

    qApp->setStyleSheet(strCSS);

    mediaPlayer.show(); 
    return app.exec();
}

