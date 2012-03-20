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

#include <QtGui/QHBoxLayout>
#include <QStyle>
#include <QDebug>
#include <QApplication>
#include <QEvent>
#include <QMouseEvent>
#include <vlc/vlc.h>
#include "VolumeSlider.h"

VlcVolumeSlider::VlcVolumeSlider(QWidget *parent)
  : QWidget(parent)
{
  _slider = new QSlider(this);
  _slider->setOrientation(Qt::Horizontal);
  _slider->setFixedWidth(100);
  _slider->setMaximum(100);
  _slider->setCursor(Qt::PointingHandCursor);
  _slider->setFocusPolicy(Qt::NoFocus);
  setVolume(50);

  _muteButton = new QPushButton(this);
  _muteButton->setObjectName("volumeButton");
  _muteButton->setToolTip(tr("Mute"));
  volumeIcon = style()->standardIcon(QStyle::SP_MediaVolume);
  mutedIcon = style()->standardIcon(QStyle::SP_MediaVolumeMuted);
  _muteButton->setIcon(volumeIcon);
  _muteButton->setCursor(Qt::PointingHandCursor);
  _muteButton->setFocusPolicy(Qt::NoFocus);

  QHBoxLayout *layout = new QHBoxLayout;
  layout->setMargin(0);
  layout->addWidget(_muteButton);
  layout->addWidget(_slider);
  setLayout(layout);

  connect(_muteButton, SIGNAL(clicked()), this, SLOT(mute()));
  connect(_slider, SIGNAL(valueChanged(int)), this, SLOT(setVolume(int)));

  qApp->installEventFilter(this);
}

VlcVolumeSlider::~VlcVolumeSlider()
{
  delete _slider;
  delete _muteButton;
}

/*virtual*/ bool VlcVolumeSlider::eventFilter(QObject* pobj, QEvent* pe)
{
  if (pe->type() == QEvent::MouseButtonPress) {
    if (pobj == _slider) {
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
  if(_curPlayer) {
    mute = libvlc_audio_get_mute(_curPlayer);
    if(libvlc_errmsg()) {
      qDebug() << "libvlc" << "Error:" << libvlc_errmsg();
      libvlc_clearerr();
    }
  }

  if(mute) {
    _slider->setDisabled(false);
    _muteButton->setIcon(volumeIcon);
  } else {
    _slider->setDisabled(true);
    _muteButton->setIcon(mutedIcon);
  }

  libvlc_audio_toggle_mute(_curPlayer);
}

void VlcVolumeSlider::setVolume(const int &volume)
{
  if (_curPlayer == NULL) return;
  _currentVolume = volume;
  _slider->setValue(_currentVolume);
  QString vol = tr("Volume") + " [" + QString::number(_currentVolume) + "%]";
  _slider->setToolTip(vol);
  libvlc_audio_set_volume(_curPlayer, _currentVolume);
}

void VlcVolumeSlider::updateVolume()
{
  //    if(_curPlayer) {
  int volume = -1;
  volume = libvlc_audio_get_volume(_curPlayer);
  if(libvlc_errmsg()) {
    qDebug() << "libvlc" << "Error:" << libvlc_errmsg();
    libvlc_clearerr();
  }

  if(volume != _currentVolume) {
    libvlc_audio_set_volume(_curPlayer, _currentVolume);
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
