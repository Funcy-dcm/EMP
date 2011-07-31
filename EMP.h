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

class MediaVideoWidget : public Phonon::VideoWidget
{
    Q_OBJECT

public:
    MediaVideoWidget(MediaPlayer *player, QWidget *parent = 0);
    QLabel *tLabel;

public slots:
    // Over-riding non-virtual Phonon::VideoWidget slot
    void setFullScreen(bool);

signals:
    void fullScreenChanged(bool);

protected:
    void mouseDoubleClickEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);
    bool event(QEvent *e);
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

    QWidget *buttonPanelWidget;

public slots:
    void moveWindowToCenter();
    void slotOpen();
    void playPause();
    void stop();
    void rewind();
    void forward();
    void playlistShow();
    void updateInfo();
    void updateTime();

private slots:
    void stateChanged(Phonon::State newstate, Phonon::State oldstate);
    void volumeChanged(qreal);
    void hasVideoChanged(bool);
    void showContextMenu(const QPoint &);

protected:
    virtual void closeEvent(QCloseEvent*);
    virtual void showEvent(QShowEvent*);
    virtual void keyPressEvent(QKeyEvent *pe);
    bool eventFilter(QObject*, QEvent*);
    void timerEvent(QTimerEvent *pe);

private:
    QString lang;
    QString fileNameP[MAX_FILE_POS];
    long filePos[MAX_FILE_POS];

    QStandardItemModel *model;
    QTableView *playListView;
    QDockWidget *playListDoc;

    QIcon playIcon;
    QIcon pauseIcon;
    QMenu *fileMenu;
    QPushButton *openButton;
    QPushButton *playButton;
    QPushButton *stopButton;
    QPushButton *rewindButton;
    QPushButton *forwardButton;
    QPushButton *playlistButton;
    Phonon::SeekSlider *slider;
    QLabel *nameLabel;
    QLabel *timeLabel;
    Phonon::VolumeSlider *volume;

    QWidget m_videoWindow;
    Phonon::MediaObject m_pmedia;
    Phonon::AudioOutput m_AudioOutput;
    Phonon::VideoWidget *m_videoWidget;
    Phonon::Path m_audioOutputPath;

    QBasicTimer m_timer;

};
#endif  //_MediaPlayer_h_
