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

#ifndef _SEEKWIDGET_H_
#define _SEEKWIDGET_H_

#include <QtCore/QTimer>
#include <QtGui/QLabel>
#include <QtGui/QSlider>
#include <QtGui/QWidget>

struct libvlc_media_player_t;
extern libvlc_media_player_t *_curPlayer;

class VlcSeekSlider : public QWidget
{
    Q_OBJECT
public:
    VlcSeekSlider(QWidget *parent = 0);
    ~VlcSeekSlider();

public:
    bool autoHide() const { return _autoHide; }
    void setAutoHide(const bool &autoHide) { _autoHide = autoHide; }

protected:
    virtual bool eventFilter(QObject*, QEvent*);

private slots:
    void changeTime(int);
    void sliderPressed();
    void updateTime();

private:
    bool _autoHide;
    QSlider *_seek;
    QTimer *_timer;
};

#endif // _SEEKWIDGET_H_
