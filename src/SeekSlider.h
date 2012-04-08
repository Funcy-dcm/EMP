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
