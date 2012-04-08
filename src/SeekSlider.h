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

#include <QtGui>
#include <vlc/vlc.h>

class VlcSeekSlider : public QWidget
{
  Q_OBJECT
public:
  VlcSeekSlider(libvlc_media_player_t *player, QWidget *parent = 0);
  ~VlcSeekSlider();
  QString timeString;

protected:
  virtual bool eventFilter(QObject*, QEvent*);

private slots:
  void changeTime(int);
  void sliderPressed();
  void updateTime();

private:
  libvlc_media_player_t *currentPlayer_;
  QSlider *seek_;
  QTimer *timer_;
};

#endif // _SEEKWIDGET_H_
