/* ======================================================================
**  MediaPlayer.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2009 by Max Schlee
** ======================================================================
*/

#ifndef _MediaPlayer_h_
#define _MediaPlayer_h_

#include <QtGui>
#include <vlc/vlc.h>
#include "VideoWidget.h"
#include "SeekSlider.h"
#include "VolumeSlider.h"
#include "Explorer.h"

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

class MediaPlayer;

struct libvlc_media_player_t;
extern libvlc_media_player_t *_curPlayer;

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

    libvlc_instance_t *_vlcinstance;
    libvlc_media_player_t *_m_player;
    libvlc_media_t *_m;

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
    QString getCurrentSourceName();
    quint64 getCurrentTime();
    quint64 getTotalTime();

public slots:
    void receiveMessage(const QString&);

    void moveWindowToCenter();
    void openFile();
    void playPause();
    void stop();
    void rewind();
    void forward();
    void playlistShow();
    void playListDoubleClicked(QModelIndex);
    void setCurrentSource(const QString&, bool);
    void setFullScreen(bool);

private slots:
    void stateChanged();
    void showContextMenu(const QPoint &);
    void slotWindowNormal();
    void addFile(QString fileName);
    void resizeWindow(int);
    void saveFilePos();

protected:
    virtual bool eventFilter(QObject*, QEvent*);
    virtual void closeEvent(QCloseEvent*);
    virtual void resizeEvent(QResizeEvent*);
    virtual void moveEvent(QMoveEvent*);

private:
    QString lang;
    QString fileNameP[MAX_FILE_POS];
    long filePos[MAX_FILE_POS];
    bool serverOn;
    QString openFilePath;

    QStandardItemModel *model;
    QTableView *playListView;
    ExplorerWidget *explorerView;

    QStackedWidget sWidget;
    QLabel *logoLabel;
    VlcVideoWidget *m_videoWidget;

    QIcon playIcon;
    QIcon pauseIcon;
    QPushButton *openButton;
    QPushButton *playButton;
    QPushButton *stopButton;
    QPushButton *rewindButton;
    QPushButton *forwardButton;
    QPushButton *playlistButton;
    VlcSeekSlider *seekSlider;
    QLabel *statusLabel;
    QLabel *timeLabel;
    VlcVolumeSlider *volumeSlider;
    QWidget *buttonPanelWidget;
    bool fullScreenOn;

    QTimer *_timerState;

signals:
    void signalWindowNormal();
};
#endif  //_MediaPlayer_h_
