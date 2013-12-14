#ifndef _MediaPlayer_h_
#define _MediaPlayer_h_

#include <QtGui>
#include <vlc/vlc.h>
#include "seekslider.h"
#include "volumeslider.h"

#define EXTENSIONS_AUDIO " *.a52 *.aac *.ac3 *.adt *.adts *.aif *.aifc *.aiff *.amr *.aob"\
  " *.ape *.cda *.dts *.flac *.it *.m4a *.m4p *.mid *.mka *.mlp *.mod *.mp1 *.mp2"\
  " *.mp3 *.mpc *.oga *.ogg *.oma *.rmi *.s3m *.spx *.tta *.voc"\
  " *.vqf *.w64 *.wav *.wma *.wv *.xa *.xm"

#define EXTENSIONS_VIDEO " *.3g2 *.3gp *.3gp2 *.3gpp *.amv *.asf *.avi *.bin *.cue *.divx *.dv *.flv *.gxf *.iso *.m1v *.m2v"\
  " *.m2t *.m2ts *.m4v *.mkv *.mov *.mp2 *.mp2v *.mp4 *.mp4v *.mpa *.mpe *.mpeg *.mpeg1"\
  " *.mpeg2 *.mpeg4 *.mpg *.mpv2 *.mts *.mxf *.nsv *.nuv"\
  " *.ogg *.ogm *.ogv *.ogx *.ps"\
  " *.rec *.rm *.rmvb *.tod *.ts *.tts *.vob *.vro *.webm *.wmv"

#define EXTENSIONS_PLAYLIST " *.asx *.b4s *.ifo *.m3u *.m3u8 *.pls *.ram *.rar *.sdp *.vlc *.xspf *.zip"

#define EXTENSIONS_MEDIA EXTENSIONS_VIDEO EXTENSIONS_AUDIO /*EXTENSIONS_PLAYLIST*/

#define MAX_FILE_POS    20

// ======================================================================
class MediaPlayer : public QMainWindow {
  Q_OBJECT

public:
  MediaPlayer(const QString&);
  virtual ~MediaPlayer();
  void dragEnterEvent(QDragEnterEvent *e);
  void dragMoveEvent(QDragMoveEvent *e);
  void dropEvent(QDropEvent *e);
  void handleDrop(QDropEvent *e);

  libvlc_instance_t *vlc_instance_;

  QStackedWidget sWidget;

  void writeSettings();
  void readSettings ();

  static bool isActive();

  void initVideoWindow();

  QToolBar *controlPanel;
  QDockWidget *playListDoc;
  QMenu *fileMenu;
  QAction *fullScreenAction;
  QAction *playPauseAction;
  QRect nGeometryWindows;
  QBasicTimer *timerFullScreen;
  int curPlayList;
  QString homeFilePath;
  QString getCurrentSourceName();
  quint64 getCurrentTime();
  quint64 getTotalTime();

  void saveFilePos();

public slots:
  void receiveMessage(const QString&);

  void moveWindowToCenter();
  void openFile();
  void playPause();
  void pause();
  void stop();
  void rewind();
  void forward();
  void playlistShow();
  void playListDoubleClicked(QModelIndex);
  void addFile(QString fileName);
  void setCurrentSource(const QString&, bool);
  void setFullScreen(bool);
  void initPlayList();

private slots:
  void stateChanged();
  void showContextMenu(const QPoint &);
  void slotWindowNormal();
  void resizeWindow(int);
  void audioSetDeviceType(QAction *pAct);
  void getSpuDescription();
  void setSpuDescription(QAction *pAct);

protected:
  virtual bool eventFilter(QObject *obj, QEvent *event);
  virtual void closeEvent(QCloseEvent*);
  virtual void resizeEvent(QResizeEvent*);
  virtual void moveEvent(QMoveEvent*);

private:
  VlcSeekSlider *seekSlider;
  VlcVolumeSlider *volumeSlider;

  QString lang;
  QString fileNameP[MAX_FILE_POS];
  long filePos[MAX_FILE_POS];
  bool serverOn;
  QString openFilePath;

  QStandardItemModel *model;
  QTableView *playListView;

  QLabel *logoLabel;

  QIcon playIcon;
  QIcon pauseIcon;
  QPushButton *openButton;
  QPushButton *playButton;
  QPushButton *stopButton;
  QPushButton *rewindButton;
  QPushButton *forwardButton;
  QPushButton *playlistButton;
  QLabel *statusLabel;
  QLabel *timeLabel;
  QWidget *buttonPanelWidget;
  bool fullScreenOn;

  QTimer *_timerState;
  int has_vout_t;

  QMenu *spuMenu_;
  QActionGroup *spuGroup_;
  QActionGroup *audioGroup_;

signals:
  void signalWindowNormal();
};
#endif  //_MediaPlayer_h_
