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
#include <phonon>

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

class QTcpServer;
class QTcpSocket;

QT_BEGIN_NAMESPACE
class QPushButton;
class QLabel;
class QSlider;
class QTextEdit;
class QMenu;
QT_END_NAMESPACE

#define MAX_FILE_POS    20

class MediaPlayer;
class ControlWidget;

class MWidget : public QWidget
{
    Q_OBJECT
public:
    MWidget(MediaPlayer *player, QWidget * parent = 0);
    QLabel *mLabel;
public slots:
    void slotShowLocalTime();
    void slotShowEndTime();
    void showWidget(QString);

protected:
    void timerEvent(QTimerEvent *);
private:
    MediaPlayer *m_player;
    QBasicTimer timerShowWidget;

};

class ControlWidget : public QWidget
{
    Q_OBJECT
public:
    ControlWidget(MediaPlayer *player, QWidget * parent = 0);
    QIcon playIcon;
    QIcon pauseIcon;
    QIcon volumeIcon;
    QIcon mutedIcon;
    QPushButton *openButton;
    QPushButton *playButton;
    QPushButton *stopButton;
    QPushButton *rewindButton;
    QPushButton *forwardButton;
    QPushButton *playlistButton;
    Phonon::SeekSlider *slider;
    QLabel *statusLabel;
    QLabel *timeLabel;
    QPushButton *volumeButton;
    Phonon::VolumeSlider *volume;
    QWidget *buttonPanelWidget;
private:
    MediaPlayer *m_player;

};

class MediaVideoWidget : public Phonon::VideoWidget
{
    Q_OBJECT

public:
    MediaVideoWidget(MediaPlayer *player, QWidget *parent = 0);

protected:
    void mouseDoubleClickEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void dropEvent(QDropEvent *e);
    void dragEnterEvent(QDragEnterEvent *e);
    void timerEvent(QTimerEvent *);

private:
    MediaPlayer *m_player;
    QBasicTimer *timerMouseSClick;

};

// ======================================================================
class MediaPlayer : public QMainWindow {
    Q_OBJECT
public:
    MediaPlayer(const QString &fileName);
    virtual ~MediaPlayer();
    void writeSettings();
    void readSettings ();
    void initVideoWindow();
    void setFile(const QString &text);
    void addFile(QString fileName);
    void dragEnterEvent(QDragEnterEvent *e);
    void dragMoveEvent(QDragMoveEvent *e);
    void dropEvent(QDropEvent *e);
    void handleDrop(QDropEvent *e);

    Phonon::MediaObject m_pmedia;
    Phonon::AudioOutput m_AudioOutput;

    QToolBar *controlPanel;
    QDockWidget *playListDoc;
    QMenu *fileMenu;
    QAction *fullScreenAction;
    QAction *playPauseAction;
    ControlWidget *cWidget;
    QRect nGeometryWindows;
    QBasicTimer *timerFullScreen;
    int curPlayList;

public slots:
    void moveWindowToCenter();
    void openFile();
    void openUrl();
    void playPause();
    void stop();
    void rewind();
    void forward();
    void playlistShow();
    void updateInfo();
    void updateTime();
    void aspectChanged(QAction *);
    void setVolumeOnOff();
    void setFullScreen(bool);
    void playListDoubleClicked(QModelIndex);

    void receiveMessage(const QString&);

    virtual void slotNewConnection();
            void slotReadClient   ();

private slots:
    void currentSourceChanged ( const Phonon::MediaSource & newSource );
    void stateChanged(Phonon::State newstate, Phonon::State oldstate);
    void volumeChanged(qreal);
    void hasVideoChanged(bool);
    void finished();
    void bufferStatus(int percent);
    void selectAudio();
    void showContextMenu(const QPoint &);
    void slotWindowNormal();
    void sendToClient(QTcpSocket* pSocket, const QString& cmd, const QString& str);

protected:
    virtual void closeEvent(QCloseEvent*);
    virtual void showEvent(QShowEvent*);
    bool eventFilter(QObject*, QEvent*);
    void timerEvent(QTimerEvent *pe);
    virtual void resizeEvent(QResizeEvent*);
    virtual void moveEvent(QMoveEvent*);

private:
    QString lang;
    QString fileNameP[MAX_FILE_POS];
    long filePos[MAX_FILE_POS];

    QStandardItemModel *model;
    QTableView *playListView;

    QStackedWidget sWidget;
    QLabel *logoLabel;
//    QWidget m_videoWindow;
    MediaVideoWidget *m_videoWidget;
    Phonon::Path m_audioOutputPath;
    MWidget *mLabel;

    QBasicTimer *timerUpdateInfo;

    QIcon playIcon;
    QIcon pauseIcon;
    QIcon volumeIcon;
    QIcon mutedIcon;
    QPushButton *openButton;
    QPushButton *playButton;
    QPushButton *stopButton;
    QPushButton *rewindButton;
    QPushButton *forwardButton;
    QPushButton *playlistButton;
    Phonon::SeekSlider *slider;
    QLabel *statusLabel;
    QLabel *timeLabel;
    QPushButton *volumeButton;
    Phonon::VolumeSlider *volume;
    QWidget *buttonPanelWidget;
    bool fullScreenOn;

    QTcpServer* m_ptcpServer;
    quint16     m_nNextBlockSize;

signals:
    void signalWindowNormal();
};
#endif  //_MediaPlayer_h_
