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
//#include <QStandardItemModel>
//#include <QSettings>

#include <phonon>

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
    MWidget(QWidget * parent = 0);
    QLabel *mLabel;

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

private:
    MediaPlayer *m_player;

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
    QBasicTimer timerFullScreen;

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

private slots:
    void currentSourceChanged ( const Phonon::MediaSource & newSource );
    void stateChanged(Phonon::State newstate, Phonon::State oldstate);
    void volumeChanged(qreal);
    void hasVideoChanged(bool);
    void bufferStatus(int percent);
    void showContextMenu(const QPoint &);
    void slotWindowNormal();

protected:
    virtual void closeEvent(QCloseEvent*);
    virtual void showEvent(QShowEvent*);
    bool eventFilter(QObject*, QEvent*);
    void timerEvent(QTimerEvent *pe);

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

    QBasicTimer timerUpdateInfo;
    QBasicTimer timerSetCursor;

//    MWidget mWidget;

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

signals:
    void signalWindowNormal();
};
#endif  //_MediaPlayer_h_
