#include <QtGui>
#include <qtsingleapplication.h>
#include "emp.h"

void LoadLang (QString &lang)
{
  QString strLang("en");
  QString strLocalLang = QLocale::system().name().left(2);
  QDir langDir = qApp->applicationDirPath() + "/lang";
  foreach (QString file, langDir.entryList(QStringList("*.qm"), QDir::Files)) {
    if (strLocalLang == file.section('.', 0, 0).section('_', 1))
      strLang = strLocalLang;
  }

  QString AppFileName = qApp->applicationDirPath()+"/EMP.ini";
  QSettings *m_settings = new QSettings(AppFileName, QSettings::IniFormat);
  lang = m_settings->value("Settings/Lang", strLang).toString();
}

// ----------------------------------------------------------------------
int main(int argc, char** argv)
{
  QtSingleApplication app(argc, argv);
  if (app.isRunning()) {
    app.sendMessage(app.arguments().value(1));
    return 0;
  }
  app.setApplicationName("EMP");
  app.setQuitOnLastWindowClosed(true);

  //    QString dirString = app.applicationDirPath();
  //    QString fileString = dirString + "/Style/EMP.qss";
  QString fileString = ":/style/style";
  QFile file(fileString);
  file.open(QFile::ReadOnly);
  QString strCSS = QLatin1String(file.readAll());
  app.setStyleSheet(strCSS);

  QString lang;
  LoadLang(lang);
  QTranslator translator;
#if defined(Q_OS_WIN) || defined(Q_OS_OS2)
  translator.load(QCoreApplication::applicationDirPath() +
                  QString("/lang/emp_%1").arg(lang));
#else
  translator.load(QString("/usr/share/emp/lang/emp_%1").
                  arg(langFileName_));
#endif
  app.installTranslator(&translator);

  fileString = app.arguments().value(1);
  MediaPlayer  mediaPlayer(fileString);

  app.setActivationWindow(&mediaPlayer, true);
  QObject::connect(&app, SIGNAL(messageReceived(const QString&)),
                   &mediaPlayer, SLOT(receiveMessage(const QString&)));

  mediaPlayer.show();
  return app.exec();
}

