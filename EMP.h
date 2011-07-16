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

#include <QtGui/QWidget>
#include <QtGui/QApplication>
#include <QtCore/QTimerEvent>
#include <QtGui/QShowEvent>
#include <QtGui/QIcon>
#include <QtCore/QHash>
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
class QThread;
class QPushButton;
class QLabel;
class QSlider;
class QTextEdit;
class QMenu;
QT_END_NAMESPACE

#define MAX_FILE_POS    20


// ======================================================================
class MediaPlayer : public QWidget {
    Q_OBJECT
public:
    MediaPlayer(const QString &fileName);
    virtual ~MediaPlayer();
    void writeSettings();
    void readSettings ();

public slots:
    void moveWindowToCenter();
    void initVideoWindow();
    void slotOpen();
    void rewind();
    void forward();
    void setVolume(int);
    void dragEnterEvent(QDragEnterEvent *e);
    void dragMoveEvent(QDragMoveEvent *e);
    void dropEvent(QDropEvent *e);
    void handleDrop(QDropEvent *e);
    void setFile(const QString &text);
    void playPause();
    void stop();
    void updateTime();
    void seekableChanged1(bool);

private slots:
    void stateChanged(Phonon::State newstate, Phonon::State oldstate);
    void showContextMenu(const QPoint &);


protected:
    virtual void closeEvent(QCloseEvent*);
    virtual void keyPressEvent(QKeyEvent *pe);

private:
    QSettings  m_settings;
    QString fileNameP[MAX_FILE_POS];
    long filePos[MAX_FILE_POS];

    QIcon playIcon;
    QIcon pauseIcon;
    QPixmap volumeIcon;
    QPixmap mutedIcon;
    QMenu *fileMenu;
    QPushButton *playButton;
    QPushButton *stopButton;
    QPushButton *rewindButton;
    QPushButton *forwardButton;
    Phonon::SeekSlider *slider;
    QLabel *volumeLabel;
    QLabel *timeLabel;
    QLabel *progressLabel;
    Phonon::VolumeSlider *volume;
    QSlider *m_hueSlider;
    QSlider *m_satSlider;
    QSlider *m_contSlider;
    QLabel *info;
    long duration;
    QHash <QString, QWidget*> propertyControllers;
    Phonon::Effect *nextEffect;
    QDialog *settingsDialog;


    QWidget m_videoWindow;
    Phonon::MediaObject m_pmedia;
    Phonon::AudioOutput m_AudioOutput;
    Phonon::VideoWidget *m_videoWidget;
    Phonon::Path m_audioOutputPath;


};
#endif  //_MediaPlayer_h_
