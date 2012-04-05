#ifndef OSDWIDGET_H
#define OSDWIDGET_H

#include <QtGui>
#include "EMP.h"

class OSDWidget : public QWidget
{
    Q_OBJECT
public:
    OSDWidget(MediaPlayer *player, QWidget * parent = 0);
    QLabel *mLabel;
public slots:
    void slotShowLocalTime();
    void slotShowEndTime();
    void showWidget(QString);

protected:
    void timerEvent(QTimerEvent *);
private:
    MediaPlayer *m_player;
    QBasicTimer timerShowWidget;

};

#endif // OSDWIDGET_H
