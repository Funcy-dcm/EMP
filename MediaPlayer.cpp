/* ======================================================================
**  MediaPlayer.cpp
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2009 by Max Schlee
** ======================================================================
*/

#include <QtGui>
#include "MediaPlayer.h"

class MediaVideoWidget : public Phonon::VideoWidget
{
public:
    MediaVideoWidget(MediaPlayer *player, QWidget *parent = 0) :
          Phonon::VideoWidget(parent), m_player(player), m_action(this)
    {
        m_action.setCheckable(true);
        m_action.setChecked(false);
        m_action.setShortcut(QKeySequence( Qt::AltModifier + Qt::Key_Return));
        m_action.setShortcutContext(Qt::WindowShortcut);
        connect(&m_action, SIGNAL(toggled(bool)), SLOT(setFullScreen(bool)));
        addAction(&m_action);
        setAcceptDrops(true);
    }

protected:
    void mouseDoubleClickEvent(QMouseEvent *e)
    {
        Phonon::VideoWidget::mouseDoubleClickEvent(e);
        setFullScreen(!isFullScreen());
    }

    void mousePressEvent(QMouseEvent *e)
    {
        Phonon::VideoWidget::mousePressEvent(e);
        m_player->playPause();
    }

    void keyPressEvent(QKeyEvent *e)
    {
        if (e->key() == Qt::Key_Space && !e->modifiers()) {
            m_player->playPause();
            e->accept();
            return;
        } else if (e->key() == Qt::Key_Escape && !e->modifiers()) {
            setFullScreen(false);
            m_player->playPause();
            e->accept();
            return;
        } else if (e->key() == Qt::Key_F11 && !e->modifiers()) {
            setFullScreen(!isFullScreen());
            e->accept();
            return;
        }
        Phonon::VideoWidget::keyPressEvent(e);
    }

    bool event(QEvent *e)
    {
        switch(e->type()) {
        case QEvent::Close:
            //we just ignore the cose events on the video widget
            //this prevents ALT+F4 from having an effect in fullscreen mode
            e->ignore();
            return true;
        case QEvent::MouseMove:
            unsetCursor();
            //fall through
        case QEvent::WindowStateChange:
            {
                //we just update the state of the checkbox, in case it wasn't already
                m_action.setChecked(windowState() & Qt::WindowFullScreen);
                const Qt::WindowFlags flags = m_player->windowFlags();
                if (windowState() & Qt::WindowFullScreen) {
                    m_timer.start(1000, this);
                } else {
                    m_timer.stop();
                    unsetCursor();
                }
            }
            break;
        default:
            break;
        }
        return Phonon::VideoWidget::event(e);
    }

    void timerEvent(QTimerEvent *e)
    {
        if (e->timerId() == m_timer.timerId()) {
            //let's store the cursor shape
            setCursor(Qt::BlankCursor);
        }
        Phonon::VideoWidget::timerEvent(e);
    }

    void dropEvent(QDropEvent *e)
    {
        m_player->handleDrop(e);
    }

    void dragEnterEvent(QDragEnterEvent *e) {
        if (e->mimeData()->hasUrls())
            e->acceptProposedAction();
    }
private:
    MediaPlayer *m_player;
    QBasicTimer m_timer;
    QAction m_action;
};

// ----------------------------------------------------------------------
MediaPlayer::MediaPlayer(const QString &filePath) :
    pcmdPlay(0),
    m_AudioOutput(Phonon::VideoCategory),
    m_videoWidget(new MediaVideoWidget(this))
{
    setWindowTitle("EMP");

    setContextMenuPolicy(Qt::CustomContextMenu);
    m_videoWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    pcmdPlay    = new QPushButton(this);
    pcmdStop    = new QPushButton(this);
    QPushButton* pcmdOpen = new QPushButton("Open");
    timeLabel  = new QLabel(this);

    slider = new Phonon::SeekSlider(this);
    slider->setMediaObject(&m_pmedia);
    volume = new Phonon::VolumeSlider(this);
/*
    volume->setRange(0, 100);
    volume->setValue(100);
*/
    volume->setAudioOutput(&m_AudioOutput);
    volume->setFixedWidth(80);

//    volumeLabel = new QLabel(this);
    volumeIcon = style()->standardPixmap(QStyle::SP_MediaVolume);
    mutedIcon = style()->standardPixmap(QStyle::SP_MediaVolumeMuted);
//    volumeLabel->setPixmap(volumeIcon);

    playIcon = QIcon(":/Res/Play.png"); //style()->standardIcon(QStyle::SP_MediaPlay);
    pauseIcon = QIcon(":/Res/Pause.png"); //style()->standardIcon(QStyle::SP_MediaPause);
    pcmdPlay->setIcon(playIcon);
    pcmdPlay->setObjectName("pcmdPlay");
    pcmdStop->setIcon(QIcon(":/Res/Stop.png"));

    pcmdPlay->setFocusPolicy(Qt::NoFocus);
    pcmdStop->setFocusPolicy(Qt::NoFocus);
    pcmdOpen->setFocusPolicy(Qt::NoFocus);

    timeLabel->setAlignment(Qt::AlignCenter | Qt::AlignRight);
    timeLabel->setMinimumHeight(10);
    timeLabel->setAutoFillBackground(true);
    QPalette pal = timeLabel->palette();
    pal.setColor(QPalette::Background, Qt::black);
    pal.setColor(QPalette::WindowText, Qt::white);
    timeLabel->setPalette(pal);

    //Layout setup
    QHBoxLayout* phbxLayout = new QHBoxLayout;
    phbxLayout->addWidget(pcmdOpen);
    phbxLayout->addSpacing(10);
    phbxLayout->addWidget(pcmdPlay);
    phbxLayout->addWidget(pcmdStop);
    phbxLayout->addSpacing(10);
    phbxLayout->addStretch();

//    phbxLayout->addWidget(volumeLabel);
    phbxLayout->addWidget(volume);

    QHBoxLayout* phbxLayout2 = new QHBoxLayout;
    phbxLayout2->addWidget(slider);

    QVBoxLayout* pvbxLayout = new QVBoxLayout(this);
    pvbxLayout->setMargin(0);
    pvbxLayout->setSpacing(0);

    initVideoWindow();
    pvbxLayout->addWidget(&m_videoWindow);
    QVBoxLayout *buttonPanelLayout = new QVBoxLayout();
    m_videoWindow.hide();

    buttonPanelLayout->setMargin(5);
    buttonPanelLayout->setSpacing(5);
    buttonPanelLayout->addLayout(phbxLayout2);
    buttonPanelLayout->addWidget(timeLabel);
    buttonPanelLayout->addLayout(phbxLayout);

    QWidget *buttonPanelWidget = new QWidget(this);
    buttonPanelWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    buttonPanelWidget->setLayout(buttonPanelLayout);
    pvbxLayout->addWidget(buttonPanelWidget);
    QHBoxLayout *labelLayout = new QHBoxLayout();
    pvbxLayout->addLayout(labelLayout);

    setLayout(pvbxLayout);

    connect(pcmdPlay, SIGNAL(clicked()), this, SLOT(playPause()));
    connect(pcmdStop, SIGNAL(clicked()), &m_pmedia, SLOT(stop()));
//    connect(volume, SIGNAL(valueChanged(int)), this, SLOT(setVolume(int)));
    connect(pcmdOpen, SIGNAL(clicked()), this, SLOT(slotOpen()));
//    connect(m_videoWidget, SIGNAL(customContextMenuRequested(const QPoint &)), SLOT(showContextMenu(const QPoint &)));
//    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), SLOT(showContextMenu(const QPoint &)));
    connect(&m_pmedia, SIGNAL(totalTimeChanged(qint64)), this, SLOT(updateTime()));
    connect(&m_pmedia, SIGNAL(tick(qint64)), this, SLOT(updateTime()));
    connect(&m_pmedia, SIGNAL(stateChanged(Phonon::State, Phonon::State)), this, SLOT(stateChanged(Phonon::State, Phonon::State)));
//    connect(&m_pmedia, SIGNAL(bufferStatus(int)), this, SLOT(bufferStatus(int)));

    m_pmedia.setTickInterval(250);

    pcmdPlay->setEnabled(false);
    setAcceptDrops(true);

    m_audioOutputPath = Phonon::createPath(&m_pmedia, &m_AudioOutput);
    Phonon::createPath(&m_pmedia, m_videoWidget);

    if (!filePath.isEmpty())
        setFile(filePath);
    resize(minimumSizeHint());

//    moveWindowToCenter();
}

void MediaPlayer::moveWindowToCenter()
{
    QRect frect = frameGeometry();
    frect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(frect.topLeft());
}

// ----------------------------------------------------------------------
/*virtual*/ MediaPlayer::~MediaPlayer()
{

}

/*virtual*/ void MediaPlayer::closeEvent(QCloseEvent* pe)
{
    m_pmedia.stop();
}

/*virtual*/ void MediaPlayer::keyPressEvent(QKeyEvent *pe)
{
    if (pe->key() == Qt::Key_Space && !pe->modifiers()) {
        playPause();
        pe->accept();
        return;
    } else if (pe->key() == Qt::Key_F11 && !pe->modifiers() && m_pmedia.hasVideo()) {
      m_videoWidget->setFullScreen(!isFullScreen());
      pe->accept();
      return;
    }
    QWidget::keyPressEvent(pe);
}

void MediaPlayer::initVideoWindow()
{
    QVBoxLayout *videoLayout = new QVBoxLayout();
    videoLayout->addWidget(m_videoWidget);
    videoLayout->setContentsMargins(0, 0, 0, 0);
    m_videoWindow.setLayout(videoLayout);
    m_videoWindow.setMinimumSize(100, 100);
}

// ----------------------------------------------------------------------
void MediaPlayer::slotOpen()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this);
    m_pmedia.clearQueue();
    if (fileNames.size() > 0) {
        QString fileName = fileNames[0];
        setFile(fileName);
        for (int i=1; i<fileNames.size(); i++)
            m_pmedia.enqueue(Phonon::MediaSource(fileNames[i]));
        m_pmedia.play();
    }
}

void MediaPlayer::handleDrop(QDropEvent *e)
{
    QList<QUrl> urls = e->mimeData()->urls();
    if (e->proposedAction() == Qt::MoveAction){
        // Just add to the que:
        for (int i=0; i<urls.size(); i++)
            m_pmedia.enqueue(Phonon::MediaSource(urls[i].toLocalFile()));
    } else {
        // Create new que:
        m_pmedia.clearQueue();
        if (urls.size() > 0) {
            QString fileName = urls[0].toLocalFile();
            QDir dir(fileName);
            if (dir.exists()) {
                dir.setFilter(QDir::Files);
                QStringList entries = dir.entryList();
                if (entries.size() > 0) {
                    setFile(fileName + QDir::separator() +  entries[0]);
                    for (int i=1; i< entries.size(); ++i)
                        m_pmedia.enqueue(fileName + QDir::separator() + entries[i]);
                }
            } else {
                setFile(fileName);
                for (int i=1; i<urls.size(); i++)
                    m_pmedia.enqueue(Phonon::MediaSource(urls[i].toLocalFile()));
            }
        }
    }
//    forwardButton->setEnabled(m_pmedia.queue().size() > 0);
    m_pmedia.play();
}


void MediaPlayer::dropEvent(QDropEvent *e)
{
    if (e->mimeData()->hasUrls() && e->proposedAction() != Qt::LinkAction) {
        e->acceptProposedAction();
        handleDrop(e);
    } else {
        e->ignore();
    }
}


void MediaPlayer::dragEnterEvent(QDragEnterEvent *e)
{
    dragMoveEvent(e);
}


void MediaPlayer::dragMoveEvent(QDragMoveEvent *e)
{
    if (e->mimeData()->hasUrls()) {
        if (e->proposedAction() == Qt::CopyAction || e->proposedAction() == Qt::MoveAction){
            e->acceptProposedAction();
        }
    }
}

void MediaPlayer::playPause()
{
    if (m_pmedia.state() == Phonon::PlayingState)
        m_pmedia.pause();
    else {
        if (m_pmedia.currentTime() == m_pmedia.totalTime())
            m_pmedia.seek(0);
        m_pmedia.play();
    }
}

void MediaPlayer::setVolume(int volume)
{
    Q_UNUSED(volume);
    m_AudioOutput.setVolume(volume/100.0f);
    if (volumeLabel) {
        if (volume == 0)
            volumeLabel->setPixmap(mutedIcon);
        else
            volumeLabel->setPixmap(volumeIcon);
    }
}

void MediaPlayer::setFile(const QString &fileName)
{
//    setWindowTitle(fileName.right(fileName.length() - fileName.lastIndexOf('/') - 1));
    m_pmedia.setCurrentSource(Phonon::MediaSource(fileName));
    m_pmedia.play();
}

void MediaPlayer::stateChanged(Phonon::State newstate, Phonon::State oldstate)
{
    Q_UNUSED(oldstate);


    if (oldstate == Phonon::LoadingState) {
        m_videoWindow.setVisible(m_pmedia.hasVideo());
//        info->setVisible(!m_pmedia.hasVideo());
        QRect videoHintRect = QRect(QPoint(0, 0), m_videoWindow.sizeHint());
        QRect newVideoRect = QApplication::desktop()->screenGeometry().intersected(videoHintRect);
        if (m_pmedia.hasVideo()){
            // Flush event que so that sizeHint takes the
            // recently shown/hidden m_videoWindow into account:
            qApp->processEvents();
            resize(sizeHint());
        } else
            resize(minimumSize());
    }


    switch (newstate) {
        case Phonon::ErrorState:
            QMessageBox::warning(this, "Phonon Mediaplayer", m_pmedia.errorString(), QMessageBox::Close);
            if (m_pmedia.errorType() == Phonon::FatalError) {
                pcmdPlay->setEnabled(false);
//                rewindButton->setEnabled(false);
            } else {
                m_pmedia.pause();
            }
            break;
        case Phonon::PausedState:
        case Phonon::StoppedState:
            pcmdPlay->setIcon(playIcon);
            if (m_pmedia.currentSource().type() != Phonon::MediaSource::Invalid){
                pcmdPlay->setEnabled(true);
//                rewindButton->setEnabled(true);
            } else {
                pcmdPlay->setEnabled(false);
//                rewindButton->setEnabled(false);
            }
            break;
        case Phonon::PlayingState:
            pcmdPlay->setEnabled(true);
            pcmdPlay->setIcon(pauseIcon);
            if (m_pmedia.hasVideo())
                m_videoWindow.show();
            // Fall through
        case Phonon::BufferingState:
//            rewindButton->setEnabled(true);
            break;
        case Phonon::LoadingState:
//            rewindButton->setEnabled(false);
            break;
    }
}

void MediaPlayer::updateTime()
{
    long len = m_pmedia.totalTime();
    long pos = m_pmedia.currentTime();
    QString timeString;
    if (pos || len)
    {
        int sec = pos/1000;
        int min = sec/60;
        int hour = min/60;
        int msec = pos;

        QTime playTime(hour%60, min%60, sec%60, msec%1000);
        sec = len / 1000;
        min = sec / 60;
        hour = min / 60;
        msec = len;

        QTime stopTime(hour%60, min%60, sec%60, msec%1000);
        QString timeFormat = "m:ss";
        if (hour > 0)
            timeFormat = "h:mm:ss";
        timeString = playTime.toString(timeFormat);
        if (len)
            timeString += " / " + stopTime.toString(timeFormat);
    }
    timeLabel->setText(timeString);
}
/*
void MediaPlayer::showContextMenu(const QPoint &p)
{
    fileMenu->popup(m_videoWidget->isFullScreen() ? p : mapToGlobal(p));
}*/
