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

void LoadLang (QString &lang){
    QString AppFileName = qApp->applicationDirPath()+"/EMP.ini";
    QSettings *m_settings = new QSettings(AppFileName, QSettings::IniFormat);
    QString strLocalLang = QLocale::system().name();

    m_settings->beginGroup("/Settings");
    lang = m_settings->value("/Lang", strLocalLang).toString();
    m_settings->endGroup();
}

// ----------------------------------------------------------------------
int main(int argc, char** argv)
{
    Q_INIT_RESOURCE(EMP);

    QApplication app(argc, argv);
    app.setApplicationName("EMP");
    app.setQuitOnLastWindowClosed(true);

    QString dirString = app.applicationDirPath();
    QString fileString = dirString + "/Style/EMP.qss";
    QFile file(fileString);
    file.open(QFile::ReadOnly);
    QString strCSS = QLatin1String(file.readAll());
    qApp->setStyleSheet(strCSS);

    QString lang;
    LoadLang(lang);
    QTranslator translator;
    translator.load(lang, qApp->applicationDirPath() + QString("/lang"));
    qApp->installTranslator(&translator);

    fileString = app.arguments().value(1);
    MediaPlayer  mediaPlayer(fileString);

    mediaPlayer.show(); 
    return app.exec();
}

