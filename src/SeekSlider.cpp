#include <QtCore/QTime>
#include <QtGui/QHBoxLayout>
#include <QApplication>
#include <QEvent>
#include <QMouseEvent>
#include <vlc/vlc.h>
#include "EMP.h"
#include "SeekSlider.h"

VlcSeekSlider::VlcSeekSlider(QWidget *parent)
  : QWidget(parent)
{
  seek_ = new QSlider(this);
  seek_->setOrientation(Qt::Horizontal);
  seek_->setMaximum(0);
  seek_->setPageStep(1000);
  seek_->setCursor(Qt::PointingHandCursor);
  seek_->setFocusPolicy(Qt::NoFocus);

  QHBoxLayout *layout = new QHBoxLayout;
  layout->setMargin(0);
  layout->addWidget(seek_);
  setLayout(layout);

  timeString = "";

  timer_ = new QTimer(this);

  connect(seek_, SIGNAL(sliderPressed()), this, SLOT(sliderPressed()));
  connect(seek_, SIGNAL(sliderMoved(int)), this, SLOT(changeTime(int)));
  connect(timer_, SIGNAL(timeout()), this, SLOT(updateTime()));

  timer_->start(250);

  qApp->installEventFilter(this);
}

VlcSeekSlider::~VlcSeekSlider()
{
  delete seek_;
  delete timer_;
}

/*virtual*/ bool VlcSeekSlider::eventFilter(QObject* pobj, QEvent* pe)
{
  if (pe->type() == QEvent::MouseButtonPress) {
    if (pobj == seek_) {
      if (((QMouseEvent*)pe)->button() == Qt::LeftButton) {
        QMouseEvent* pe1 = new QMouseEvent(QEvent::MouseButtonPress, ((QMouseEvent*)pe)->pos(),
                                           Qt::MidButton, Qt::MidButton, Qt::NoModifier);
        QApplication::sendEvent(pobj, pe1);
      }
    }
  }

  return false;
}

void VlcSeekSlider::updateTime()
{
  static int fullTime_t = 0;
  static int currentTime_t = 0;
  if(MediaPlayer::isActive()) {
    libvlc_state_t state;
    state = libvlc_media_player_get_state(currentPlayer_);
    int fullTime = libvlc_media_player_get_length(currentPlayer_);
    if (state != libvlc_Stopped) {
      int currentTime = libvlc_media_player_get_time(currentPlayer_);
      fullTime_t = fullTime;
      currentTime_t = currentTime;
    } else {
      currentTime_t = 0;
    }
    seek_->setMaximum(fullTime_t);
    if (!seek_->isSliderDown())
      seek_->setValue(currentTime_t);
    timeString = QTime(0,0,0,0).addMSecs(currentTime_t).toString("hh:mm:ss");
    timeString += " (-" +
        QTime(0,0,0,0).addMSecs(fullTime_t-currentTime_t).toString("hh:mm:ss") +
        ")";
    timeString += " / " + QTime(0,0,0,0).addMSecs(fullTime_t).toString("hh:mm:ss");

    if (currentTime_t < 1000) {
      seek_->setSliderDown(false);
    }

  } else {
    seek_->setMaximum(0);
    seek_->setValue(0);
  }
}

void VlcSeekSlider::sliderPressed()
{
  if(currentPlayer_ == NULL)
    return;
  libvlc_media_player_set_time(currentPlayer_, seek_->value());
}

void VlcSeekSlider::changeTime(int value)
{
  if(currentPlayer_ == NULL)
    return;
  libvlc_media_player_set_time(currentPlayer_, value);
}
