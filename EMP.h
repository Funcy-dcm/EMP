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

#include <QtGui/QMainWindow>
#include <QtGui/QApplication>
#include <QtCore/QTimerEvent>
#include <QtGui/QShowEvent>
#include <QtGui/QIcon>
#include <QtCore/QBasicTimer>
#include <QtGui/QAction>
#include <QStandardItemModel>
#include <QtGui/QTableView>
#include <QtGui/QLabel>
#include <QSettings>

#include <phonon/mediaobject.h>
#include <phonon/audiooutput.h>
#include <phonon/mediasource.h>
#include <phonon/videowidget.h>
#include <phonon/seekslider.h>
#include <phonon/volumeslider.h>
#include <phonon/effect.h>
#include <phonon/backendcapabilities.h>
#include <phonon/effect.h>
#include <phonon/effectparameter.h>

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
    QLabel *statusLabel;
    QIcon playIcon;
    QIcon pauseIcon;
    QPixmap volumeIcon;
    QPixmap mutedIcon;
    QPushButton *openButton;
    QPushButton *playButton;
    QPushButton *stopButton;
    QPushButton *rewindButton;
    QPushButton *forwardButton;
    QPushButton *playlistButton;
    Phonon::SeekSlider *slider;
    QLabel *nameLabel;
    QLabel *timeLabel;
    QLabel *volumeLabel;
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

public slots:
    // Over-riding non-virtual Phonon::VideoWidget slot
    void setFullScreen(bool);

signals:
    void fullScreenChanged(bool);

protected:
    void mouseDoubleClickEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
//    bool event(QEvent *e);
    void timerEvent(QTimerEvent *e);
    void dropEvent(QDropEvent *e);
    void dragEnterEvent(QDragEnterEvent *e);

private:
    MediaPlayer *m_player;
    QBasicTimer m_timer;
    QAction m_action;
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
    bool volumeOnOff;

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

private slots:
    void stateChanged(Phonon::State newstate, Phonon::State oldstate);
    void volumeChanged(qreal);
    void hasVideoChanged(bool);
    void bufferStatus(int percent);
    void showContextMenu(const QPoint &);

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

    QWidget m_videoWindow;
    Phonon::VideoWidget *m_videoWidget;
    Phonon::Path m_audioOutputPath;

    QBasicTimer m_timer;

    MWidget mWidget;
    ControlWidget *cWidget;
    ControlWidget *cWidget1;

};
#endif  //_MediaPlayer_h_
