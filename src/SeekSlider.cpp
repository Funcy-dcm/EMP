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

#include <QtCore/QTime>
#include <QtGui/QHBoxLayout>
#include <QApplication>
#include <QEvent>
#include <QMouseEvent>
#include <vlc/vlc.h>
#include "SeekSlider.h"

VlcSeekSlider::VlcSeekSlider(QWidget *parent)
    : QWidget(parent),
      _autoHide(false)
{
    _seek = new QSlider(this);
    _seek->setOrientation(Qt::Horizontal);
    _seek->setMaximum(0);
    _seek->setPageStep(1000);
    _seek->setCursor(Qt::PointingHandCursor);
    _seek->setFocusPolicy(Qt::NoFocus);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->addWidget(_seek);
    setLayout(layout);

    _timer = new QTimer(this);

    connect(_seek, SIGNAL(sliderPressed()), this, SLOT(sliderPressed()));
    connect(_seek, SIGNAL(sliderMoved(int)), this, SLOT(changeTime(int)));
    connect(_timer, SIGNAL(timeout()), this, SLOT(updateTime()));

    _timer->start(250);

    qApp->installEventFilter(this);
}

VlcSeekSlider::~VlcSeekSlider()
{
    delete _seek;
    delete _timer;
}

/*virtual*/ bool VlcSeekSlider::eventFilter(QObject* pobj, QEvent* pe)
{
    if (pe->type() == QEvent::MouseButtonPress) {
        if (pobj == _seek) {
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
    bool active = false;

    if(_curPlayer == NULL)
        active = false;
    else {
        libvlc_state_t state;
        state = libvlc_media_player_get_state(_curPlayer);

        if(state == libvlc_NothingSpecial || state == libvlc_Ended || state == libvlc_Error)
            active = false;
        else
            active = true;
    }

    if(active) {
        int fullTime = libvlc_media_player_get_length(_curPlayer);
        int currentTime = libvlc_media_player_get_time(_curPlayer);

        _seek->setMaximum(fullTime);
        if (!_seek->isSliderDown())
            _seek->setValue(currentTime);

        if(_autoHide && fullTime == 0) {
            setVisible(false);
        } else {
            setVisible(true);
        }
    } else {
        _seek->setMaximum(0);
        _seek->setValue(0);

        if(_autoHide) {
            setVisible(false);
        }
    }
}

void VlcSeekSlider::sliderPressed()
{
    if(_curPlayer == NULL)
        return;
    libvlc_media_player_set_time(_curPlayer, _seek->value());
}

void VlcSeekSlider::changeTime(int value)
{
    if(_curPlayer == NULL)
        return;
    libvlc_media_player_set_time(_curPlayer, value);
}
