#ifndef _VOLUMESLIDER_H_
#define _VOLUMESLIDER_H_

#include <QtGui>
#include <vlc/vlc.h>

class VlcVolumeSlider : public QWidget
{
  Q_OBJECT
public:
  VlcVolumeSlider(libvlc_media_player_t *player, QWidget *parent = 0);
  ~VlcVolumeSlider();

public slots:
  void mute();
  void setVolume(const int &volume);
  int volume() const;
  void volumeControl(const bool &up);
  void volumeDown() { volumeControl(false); }
  void volumeUp() { volumeControl(true); }

protected:
  virtual bool eventFilter(QObject*, QEvent*);

private slots:
  void updateVolume();

private:
  libvlc_media_player_t *currentPlayer_;
  int currentVolume_;

  QPushButton *muteButton_;
  QIcon volumeIcon;
  QIcon mutedIcon;
  QSlider *slider_;

};

#endif // _VOLUMESLIDER_H_
