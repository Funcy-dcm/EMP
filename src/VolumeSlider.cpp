/****************************************************************************
* VLC-Qt - Qt and libvlc connector library
* Copyright (C) 2011 Tadej Novak <tadej@tano.si>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include <vlc/vlc.h>
#include "VolumeSlider.h"

VlcVolumeSlider::VlcVolumeSlider(libvlc_media_player_t *player, QWidget *parent)
  : currentPlayer_(player),
    QWidget(parent)
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
  _currentVolume = volume;
  slider_->setValue(_currentVolume);
  QString vol = tr("Volume") + " [" + QString::number(_currentVolume) + "%]";
  slider_->setToolTip(vol);
  libvlc_audio_set_volume(currentPlayer_, _currentVolume);
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

  if(volume != _currentVolume) {
    libvlc_audio_set_volume(currentPlayer_, _currentVolume);
    if(libvlc_errmsg()) {
      qDebug() << "libvlc" << "Error:" << libvlc_errmsg();
      libvlc_clearerr();
    }
  }
  //    }
}

int VlcVolumeSlider::volume() const
{
  return _currentVolume;
}

void VlcVolumeSlider::volumeControl(const bool &up)
{
  if(up) {
    if(_currentVolume != 100) {
      setVolume(_currentVolume+1);
    }
  } else {
    if(_currentVolume != 0) {
      setVolume(_currentVolume-1);
    }
  }
}
