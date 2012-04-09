#include "volumeslider.h"

VlcVolumeSlider::VlcVolumeSlider(libvlc_media_player_t *player, QWidget *parent)
  : QWidget(parent),
    currentPlayer_(player)
{
  slider_ = new QSlider(this);
  slider_->setOrientation(Qt::Horizontal);
  slider_->setFixedWidth(100);
  slider_->setMaximum(100);
  slider_->setCursor(Qt::PointingHandCursor);
  slider_->setFocusPolicy(Qt::NoFocus);
  setVolume(100);

  muteButton_ = new QPushButton(this);
  muteButton_->setObjectName("volumeButton");
  muteButton_->setToolTip(tr("Mute"));
  volumeIcon = style()->standardIcon(QStyle::SP_MediaVolume);
  mutedIcon = style()->standardIcon(QStyle::SP_MediaVolumeMuted);
  muteButton_->setIcon(volumeIcon);
  muteButton_->setCursor(Qt::PointingHandCursor);
  muteButton_->setFocusPolicy(Qt::NoFocus);

  QHBoxLayout *layout = new QHBoxLayout;
  layout->setMargin(0);
  layout->addWidget(muteButton_);
  layout->addWidget(slider_);
  setLayout(layout);

  connect(muteButton_, SIGNAL(clicked()), this, SLOT(mute()));
  connect(slider_, SIGNAL(valueChanged(int)), this, SLOT(setVolume(int)));

  qApp->installEventFilter(this);
}

VlcVolumeSlider::~VlcVolumeSlider()
{
  delete slider_;
  delete muteButton_;
}

/*virtual*/ bool VlcVolumeSlider::eventFilter(QObject* pobj, QEvent* pe)
{
  if (pe->type() == QEvent::MouseButtonPress) {
    if (pobj == slider_) {
      if (((QMouseEvent*)pe)->button() == Qt::LeftButton) {
        QMouseEvent* pe1 = new QMouseEvent(QEvent::MouseButtonPress, ((QMouseEvent*)pe)->pos(),
                                           Qt::MidButton, Qt::MidButton, Qt::NoModifier);
        QApplication::sendEvent(pobj, pe1);
      }
    }
  }

  return false;
}

void VlcVolumeSlider::mute()
{
  bool mute = false;
  if(currentPlayer_) {
    mute = libvlc_audio_get_mute(currentPlayer_);
    if(libvlc_errmsg()) {
      qDebug() << "libvlc" << "Error:" << libvlc_errmsg();
      libvlc_clearerr();
    }
  }

  if(mute) {
    slider_->setDisabled(false);
    muteButton_->setIcon(volumeIcon);
  } else {
    slider_->setDisabled(true);
    muteButton_->setIcon(mutedIcon);
  }

  libvlc_audio_toggle_mute(currentPlayer_);
}

void VlcVolumeSlider::setVolume(const int &volume)
{
  if (currentPlayer_ == NULL) return;
  currentVolume_ = volume;
  slider_->setValue(currentVolume_);
  QString vol = tr("Volume") + " [" + QString::number(currentVolume_) + "%]";
  slider_->setToolTip(vol);
  libvlc_audio_set_volume(currentPlayer_, currentVolume_);
}

void VlcVolumeSlider::updateVolume()
{
  //    if(_curPlayer) {
  int volume = -1;
  volume = libvlc_audio_get_volume(currentPlayer_);
  if(libvlc_errmsg()) {
    qDebug() << "libvlc" << "Error:" << libvlc_errmsg();
    libvlc_clearerr();
  }

  if(volume != currentVolume_) {
    libvlc_audio_set_volume(currentPlayer_, currentVolume_);
    if(libvlc_errmsg()) {
      qDebug() << "libvlc" << "Error:" << libvlc_errmsg();
      libvlc_clearerr();
    }
  }
  //    }
}

int VlcVolumeSlider::volume() const
{
  return currentVolume_;
}

void VlcVolumeSlider::volumeControl(const bool &up)
{
  if(up) {
    if(currentVolume_ != 100) {
      setVolume(currentVolume_+1);
    }
  } else {
    if(currentVolume_ != 0) {
      setVolume(currentVolume_-1);
    }
  }
}
