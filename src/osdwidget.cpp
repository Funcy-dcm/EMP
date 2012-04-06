#include "osdwidget.h"

OSDWidget::OSDWidget(MediaPlayer *player, QWidget *parent) :
  QWidget(parent,  Qt::Tool | Qt::FramelessWindowHint),
  mediaPlayer_(player)
{
  setAttribute(Qt::WA_TranslucentBackground);
  setFocusPolicy(Qt::NoFocus);

  generalLabel_ = new QLabel(this);
  generalLabel_->setObjectName("osdLabel");
  generalLabel_->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  generalLabel_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
  generalLabel_->setMinimumWidth(100);

  QAction *showLocalTime = new QAction(this);
  showLocalTime->setShortcut(QKeySequence( Qt::Key_T ));
  showLocalTime->setShortcutContext(Qt::ApplicationShortcut);
  connect(showLocalTime, SIGNAL(triggered()), this, SLOT(slotShowLocalTime()));
  mediaPlayer_->addAction(showLocalTime);

  QAction *showEndTime = new QAction(this);
  showEndTime->setShortcut(QKeySequence( Qt::Key_E ));
  showEndTime->setShortcutContext(Qt::ApplicationShortcut);
  connect(showEndTime, SIGNAL(triggered()), this, SLOT(slotShowEndTime()));
  mediaPlayer_->addAction(showEndTime);

  QVBoxLayout* pvbxLayout = new QVBoxLayout(this);
  pvbxLayout->setContentsMargins(0, 0, 0, 0);
  pvbxLayout->addWidget(generalLabel_);
  setLayout(pvbxLayout);

  generalLabel_->setText("EMP");
}

void OSDWidget::showWidget(QString str)
{
  hide();
  if (str == tr("Pause")) {
    generalLabel_->setPixmap(QPixmap(":/res/pause_48.png"));
  } else if (str == tr("Play")) {
    generalLabel_->setPixmap(QPixmap(":/res/play_48.png"));
  } else {
    generalLabel_->setText(str);
  }

  QPoint pos;
  show();
  pos.setX(mediaPlayer_->geometry().x() + mediaPlayer_->width() - width() - 25);
  if (mediaPlayer_->playListDoc->isVisible()) pos.setX(pos.x() - mediaPlayer_->playListDoc->width());
  pos.setY(mediaPlayer_->geometry().y() + 25);
  move(pos);

  if (str != tr("Pause"))
    timerShowWidget.start(5000, this);
}

void OSDWidget::timerEvent(QTimerEvent *pe)
{
  if (pe->timerId() == timerShowWidget.timerId()) {
    timerShowWidget.stop();
    hide();
  }
}

void OSDWidget::slotShowLocalTime()
{
  QString str = QDateTime::currentDateTime().toString("h:mm");
  showWidget(str);
}

void OSDWidget::slotShowEndTime()
{
  long len = mediaPlayer_->getTotalTime();
  long pos = mediaPlayer_->getCurrentTime();
  QString timeString = " ";
  if (pos || len) {
    long nPos = len - pos;
    int sec = nPos/1000;
    int min = sec/60;
    int hour = min/60;
    int msec = nPos;
    QTime nPlayTime(hour%60, min%60, sec%60, msec%1000);
    QString timeFormat = "m:ss";
    if (hour > 0)
      timeFormat = "h:mm:ss";
    if (len) timeString = nPlayTime.toString(timeFormat);
  }
  showWidget(timeString);
}
