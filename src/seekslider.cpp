#include "seekslider.h"
#include "emp.h"

VlcSeekSlider::VlcSeekSlider(libvlc_media_player_t *player, QWidget *parent)
  : QWidget(parent),
    currentPlayer_(player)
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

/*virtual*/ bool VlcSeekSlider::eventFilter(QObject* pobj, QEvent* event)
{
  if (event->type() == QEvent::MouseButtonPress ||
      event->type() == QEvent::MouseMove ||
      event->type() == QEvent::MouseButtonRelease) {
    if (pobj == seek_) {
      QPoint point = ((QMouseEvent*)event)->pos();
      int value = seek_->maximum()/seek_->width()*point.x();
      if (value > 0) {
        QString timeString = QTime(0,0,0,0).addMSecs(value).toString("hh:mm:ss");
        seek_->setToolTip(timeString);

        point.setX(point.x()-25);
        point.setY(-50);
        QHelpEvent* event1 = new QHelpEvent(QEvent::ToolTip, point, mapToGlobal(point));
        QApplication::sendEvent(pobj, event1);
      }
    }
  }
  if (event->type() == QEvent::MouseButtonPress) {
    if (pobj == seek_) {
      if (((QMouseEvent*)event)->button() == Qt::LeftButton) {
        QMouseEvent* event1 = new QMouseEvent(QEvent::MouseButtonPress, ((QMouseEvent*)event)->pos(),
                                              Qt::MidButton, Qt::MidButton, Qt::NoModifier);
        QApplication::sendEvent(pobj, event1);
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
