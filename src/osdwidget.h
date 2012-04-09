#ifndef OSDWIDGET_H
#define OSDWIDGET_H

#include <QtGui>
#include "emp.h"

class OSDWidget : public QWidget
{
  Q_OBJECT
public:
  OSDWidget(MediaPlayer *player, QWidget * parent = 0);
  QLabel *generalLabel_;
public slots:
  void slotShowLocalTime();
  void slotShowEndTime();
  void showWidget(QString);

protected:
  void timerEvent(QTimerEvent *);
private:
  MediaPlayer *mediaPlayer_;
  QBasicTimer timerShowWidget;

};

#endif // OSDWIDGET_H
