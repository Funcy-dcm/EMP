/* ======================================================================
**  MediaPlayer.cpp
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2009 by Max Schlee
** ======================================================================
*/

#include <QtGui>
#include "EMP.h"

MediaVideoWidget::MediaVideoWidget(MediaPlayer *player, QWidget *parent) :
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

void MediaVideoWidget::setFullScreen(bool enabled)
{
    Phonon::VideoWidget::setFullScreen(enabled);
    emit fullScreenChanged(enabled);
}

void MediaVideoWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    Phonon::VideoWidget::mouseDoubleClickEvent(e);
    setFullScreen(!isFullScreen());
}

void MediaVideoWidget::mousePressEvent(QMouseEvent *e)
{
    Phonon::VideoWidget::mousePressEvent(e);
    m_player->playPause();
}

void MediaVideoWidget::keyPressEvent(QKeyEvent *e)
{
    if (!e->modifiers()) {
        if (e->key() == Qt::Key_Space) {
            m_player->playPause();
            e->accept();
            return;
        } else if (e->key() == Qt::Key_Escape) {
            setFullScreen(false);
            m_player->playPause();
            e->accept();
            return;
        } else if (e->key() == Qt::Key_F11) {
            setFullScreen(!isFullScreen());
            e->accept();
            return;
        }
    }
    Phonon::VideoWidget::keyPressEvent(e);
}

bool MediaVideoWidget::event(QEvent *e)
{
    switch(e->type()) {
    case QEvent::Close:
        //we just ignore the cose events on the video widget
        //this prevents ALT+F4 from having an effect in fullscreen mode
        e->ignore();
        return true;
    case QEvent::MouseMove:
        setCursor(Qt::PointingHandCursor);
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
                setCursor(Qt::PointingHandCursor);
            }
        }
        break;
    default:
        break;
    }
    return Phonon::VideoWidget::event(e);
}

void MediaVideoWidget::timerEvent(QTimerEvent *e)
{
    if (e->timerId() == m_timer.timerId()) {
        //let's store the cursor shape
        setCursor(Qt::BlankCursor);
    }
    Phonon::VideoWidget::timerEvent(e);
}

void MediaVideoWidget::dropEvent(QDropEvent *e)
{
    m_player->handleDrop(e);
}

void MediaVideoWidget::dragEnterEvent(QDragEnterEvent *e) {
    if (e->mimeData()->hasUrls())
        e->acceptProposedAction();
}


// ----------------------------------------------------------------------
MediaPlayer::MediaPlayer(const QString &filePath) :
    playButton(0),
    m_AudioOutput(Phonon::VideoCategory),
    m_videoWidget(new MediaVideoWidget(this))
{
    setWindowTitle("EMP");

    readSettings();

    setContextMenuPolicy(Qt::CustomContextMenu);
    m_videoWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    openButton    = new QPushButton(this);
    playButton    = new QPushButton(this);
    stopButton    = new QPushButton(this);
    rewindButton  = new QPushButton(this);
    forwardButton = new QPushButton(this);

    nameLabel  = new QLabel(this);
    timeLabel  = new QLabel(this);

    slider = new Phonon::SeekSlider(this);
    slider->setMediaObject(&m_pmedia);
    slider->setCursor(Qt::PointingHandCursor);

    volume = new Phonon::VolumeSlider(this);
    volume->setAudioOutput(&m_AudioOutput);
    volume->setFixedWidth(100);
    volume->setCursor(Qt::PointingHandCursor);
    qApp->installEventFilter( this );

    openButton->setIcon(QIcon(":/Res/Open.png"));
    playIcon = QIcon(":/Res/Play1.png");
    pauseIcon = QIcon(":/Res/Pause1.png");
    playButton->setIcon(playIcon);
    playButton->setObjectName("playButton");
    stopButton->setIcon(QIcon(":/Res/Stop1.png"));
    rewindButton->setIcon(QIcon(":/Res/Rewind1.png"));
    forwardButton->setIcon(QIcon(":/Res/Forward1.png"));

    openButton->setToolTip(tr("Open"));
    playButton->setToolTip(tr("Play"));
    stopButton->setToolTip(tr("Stop"));
    rewindButton->setToolTip(tr("Previous"));
    forwardButton->setToolTip(tr("Next"));

    openButton->setFocusPolicy(Qt::NoFocus);
    playButton->setFocusPolicy(Qt::NoFocus);
    stopButton->setFocusPolicy(Qt::NoFocus);
    rewindButton->setFocusPolicy(Qt::NoFocus);
    forwardButton->setFocusPolicy(Qt::NoFocus);

    openButton->setCursor(Qt::PointingHandCursor);
    playButton->setCursor(Qt::PointingHandCursor);
    stopButton->setCursor(Qt::PointingHandCursor);
    rewindButton->setCursor(Qt::PointingHandCursor);
    forwardButton->setCursor(Qt::PointingHandCursor);

    timeLabel->setAlignment(Qt::AlignRight);
//    timeLabel->setMinimumHeight(10);
    /*timeLabel->setAutoFillBackground(true);
    QPalette pal = timeLabel->palette();
    pal.setColor(QPalette::Background, Qt::black);
    pal.setColor(QPalette::WindowText, Qt::white);
    timeLabel->setPalette(pal);*/

    nameLabel->setAlignment(Qt::AlignLeft);

    //Layout setup
    QHBoxLayout* phbxLayout = new QHBoxLayout;
    phbxLayout->addWidget(openButton);
    phbxLayout->addSpacing(10);
    phbxLayout->addWidget(playButton);
    phbxLayout->addWidget(stopButton);
    phbxLayout->addWidget(rewindButton);
    phbxLayout->addWidget(forwardButton);
    phbxLayout->addSpacing(10);
    phbxLayout->addStretch();
    phbxLayout->addWidget(volume);

    QHBoxLayout* phbxLayout2 = new QHBoxLayout;
    phbxLayout2->addWidget(slider);

    QHBoxLayout* phbxLayout3 = new QHBoxLayout;
    phbxLayout3->addWidget(nameLabel);
    phbxLayout3->addSpacing(20);
    phbxLayout3->addWidget(timeLabel);

    QVBoxLayout* pvbxLayout = new QVBoxLayout(this);
    pvbxLayout->setMargin(0);
    pvbxLayout->setSpacing(0);

    initVideoWindow();
    pvbxLayout->addWidget(&m_videoWindow);
    m_videoWindow.hide();

    QVBoxLayout *buttonPanelLayout = new QVBoxLayout();
    buttonPanelLayout->setMargin(5);
    buttonPanelLayout->setSpacing(5);
    buttonPanelLayout->addLayout(phbxLayout3);
    buttonPanelLayout->addLayout(phbxLayout2);
    buttonPanelLayout->addLayout(phbxLayout);

    QWidget *buttonPanelWidget = new QWidget(this);
    buttonPanelWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    buttonPanelWidget->setLayout(buttonPanelLayout);
    pvbxLayout->addWidget(buttonPanelWidget);
    QHBoxLayout *labelLayout = new QHBoxLayout();
    pvbxLayout->addLayout(labelLayout);

    setLayout(pvbxLayout);

    connect(openButton, SIGNAL(clicked()), this, SLOT(slotOpen()));
    connect(playButton, SIGNAL(clicked()), this, SLOT(playPause()));
    connect(stopButton, SIGNAL(clicked()), this, SLOT(stop()));
    connect(rewindButton, SIGNAL(clicked()), this, SLOT(rewind()));
    connect(forwardButton, SIGNAL(clicked()), this, SLOT(forward()));

    connect(m_videoWidget, SIGNAL(customContextMenuRequested(const QPoint &)), SLOT(showContextMenu(const QPoint &)));
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), SLOT(showContextMenu(const QPoint &)));
    connect(&m_pmedia, SIGNAL(metaDataChanged()), this, SLOT(updateInfo()));
    connect(&m_pmedia, SIGNAL(totalTimeChanged(qint64)), this, SLOT(updateTime()));
    connect(&m_pmedia, SIGNAL(tick(qint64)), this, SLOT(updateTime()));
    connect(&m_pmedia, SIGNAL(stateChanged(Phonon::State, Phonon::State)), this, SLOT(stateChanged(Phonon::State, Phonon::State)));
//    connect(&m_pmedia, SIGNAL(bufferStatus(int)), this, SLOT(bufferStatus(int)));
    connect(&m_pmedia, SIGNAL(seekableChanged(bool)), this, SLOT(seekableChanged1(bool)));
    connect(&m_pmedia, SIGNAL(hasVideoChanged(bool)), this, SLOT(hasVideoChanged(bool)));

    m_pmedia.setTickInterval(250);

    playButton->setEnabled(false);
    rewindButton->setEnabled(false);
    forwardButton->setEnabled(false);
    setAcceptDrops(true);

    m_audioOutputPath = Phonon::createPath(&m_pmedia, &m_AudioOutput);
    Phonon::createPath(&m_pmedia, m_videoWidget);

    if (!filePath.isEmpty())
        setFile(filePath);
    resize(minimumSizeHint());

    moveWindowToCenter();

}

// ----------------------------------------------------------------------
/*virtual*/ MediaPlayer::~MediaPlayer()
{

}

// ----------------------------------------------------------------------
void MediaPlayer::readSettings()
{
    QString key, key_value;

    QString AppFileName = qApp->applicationDirPath()+"/EMP.ini";
    QSettings *m_settings = new QSettings(AppFileName, QSettings::IniFormat);

    QString strLocalLang = QLocale::system().name();
    m_settings->beginGroup("/Settings");
    lang = m_settings->value("/Lang", strLocalLang).toString();
    m_settings->endGroup();

    m_settings->beginGroup("/FilePosition");
    for (int i = 0; i < MAX_FILE_POS; ++i) {
        key = "/FileName" + QString::number(i);
        fileNameP[i] = m_settings->value(key, "").toString();
        key_value = "/FilePosition" + QString::number(i);
        filePos[i] = m_settings->value(key_value, 0).toInt();
    }
    m_settings->endGroup();
}

// ----------------------------------------------------------------------
void MediaPlayer::writeSettings()
{
    QString key, key_value;

    QString AppFileName = qApp->applicationDirPath()+"/EMP.ini";
    QSettings *m_settings = new QSettings(AppFileName, QSettings::IniFormat);

    m_settings->beginGroup("/Settings");
    m_settings->setValue("/Lang", lang);
    m_settings->endGroup();

    m_settings->beginGroup("/FilePosition");
    for (int i = 0; i < MAX_FILE_POS; ++i) {
        key = "/FileName" + QString::number(i);
        m_settings->setValue(key, fileNameP[i]);
        key_value = "/FilePosition" + QString::number(i);
        m_settings->setValue(key_value, (qlonglong)filePos[i]);
    }
    m_settings->endGroup();
}

/*virtual*/ void MediaPlayer::closeEvent(QCloseEvent* pe)
{
    if (m_pmedia.hasVideo()) {
        bool findOk = 0;
        for (int i = 0; i < MAX_FILE_POS; ++i) {
            if (fileNameP[i] == m_pmedia.currentSource().fileName()) {
                filePos[i] = m_pmedia.currentTime();
                findOk = 1;
                break;
            }
        }

        if (!findOk) {
            for (int i = MAX_FILE_POS-1; i > 0 ; --i) {
                fileNameP[i] = fileNameP[i-1];
                filePos[i] = filePos[i-1];
            }
            fileNameP[0] = m_pmedia.currentSource().fileName();
            filePos[0] = m_pmedia.currentTime();
        }

        if (m_pmedia.currentTime() == m_pmedia.totalTime()) {
            for (int i = 0; i < MAX_FILE_POS; ++i) {
                if (fileNameP[i] == m_pmedia.currentSource().fileName()) {
                    filePos[i] = 0;
                    break;
                }
            }
        }
    }
    if (m_pmedia.state() != Phonon::StoppedState) m_pmedia.stop();
    writeSettings();
}

/*virtual*/ void MediaPlayer::keyPressEvent(QKeyEvent *pe)
{
    if (!pe->modifiers()) {
        if (pe->key() == Qt::Key_Space) {
            playPause();
            pe->accept();
            return;
        } else if (pe->key() == Qt::Key_F11 && m_pmedia.hasVideo()) {
          m_videoWidget->setFullScreen(!isFullScreen());
          pe->accept();
          return;
        }
    }
    QWidget::keyPressEvent(pe);
}

/*virtual*/ bool MediaPlayer::eventFilter(QObject* pobj, QEvent* pe)
{
    if (pe->type() == QEvent::ToolTip) {
        if (pobj == volume) {
            QString vol = tr("Volume") + " [" + QString::number(qreal(m_AudioOutput.volume()*100.0f)) + "%]";
            volume->setToolTip(vol);
//            return true;
        }
    }
    return false;
}

void MediaPlayer::seekableChanged1(bool b)
{
    if (b) {
//        m_pmedia.stop();
//        moveWindowToCenter();
    }
}

void MediaPlayer::moveWindowToCenter()
{
    QRect frect = frameGeometry();
    frect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(frect.topLeft());
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
    }
}

void MediaPlayer::handleDrop(QDropEvent *e)
{
    QList<QUrl> urls = e->mimeData()->urls();
    if (e->proposedAction() == Qt::MoveAction){
        // ��������� � �������
        for (int i=0; i<urls.size(); i++)
            m_pmedia.enqueue(Phonon::MediaSource(urls[i].toLocalFile()));
    } else {
        // ������ ����� �������
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
    forwardButton->setEnabled(m_pmedia.queue().size() > 0);
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

void MediaPlayer::stop()
{
    for (int i = 0; i < MAX_FILE_POS; ++i) {
        if (fileNameP[i] == m_pmedia.currentSource().fileName()) {
            fileNameP[i] = "";
            filePos[i] = 0;
            break;
        }
    }
    m_pmedia.stop();
}

void MediaPlayer::setFile(const QString &fileName)
{
    m_pmedia.setCurrentSource(Phonon::MediaSource(fileName));
}

void MediaPlayer::stateChanged(Phonon::State newstate, Phonon::State oldstate)
{
    Q_UNUSED(oldstate);

    if (oldstate == Phonon::LoadingState) {
//        m_videoWindow.setVisible(m_pmedia.hasVideo());
        QRect videoHintRect = QRect(QPoint(0, 0), m_videoWindow.sizeHint());
        QRect newVideoRect = QApplication::desktop()->screenGeometry().intersected(videoHintRect);
        if (m_pmedia.hasVideo()){
            // Flush event que so that sizeHint takes the
            // recently shown/hidden m_videoWindow into account:
            qApp->processEvents();
            resize(sizeHint());
        } else
            resize(minimumSize());
        //
        for (int i = 0; i < MAX_FILE_POS; ++i) {
            if (fileNameP[i] == m_pmedia.currentSource().fileName()) {
                if (m_pmedia.isSeekable())
                    m_pmedia.seek(filePos[i]);
                break;
            }
        }
        moveWindowToCenter();
        m_pmedia.play();
        //
    }

    switch (newstate) {
        case Phonon::ErrorState:
            QMessageBox::warning(this, "Phonon Mediaplayer", m_pmedia.errorString(), QMessageBox::Close);
            if (m_pmedia.errorType() == Phonon::FatalError) {
                playButton->setEnabled(false);
                rewindButton->setEnabled(false);
            } else {
                m_pmedia.pause();
            }
            break;
        case Phonon::StoppedState:
        case Phonon::PausedState: 
            playButton->setIcon(playIcon);
            playButton->setToolTip(tr("Play"));
            if (m_pmedia.currentSource().type() != Phonon::MediaSource::Invalid){
                playButton->setEnabled(true);
                rewindButton->setEnabled(true);
            } else {
                playButton->setEnabled(false);
                rewindButton->setEnabled(false);
            }
            if (m_pmedia.currentTime() == m_pmedia.totalTime()) {
                m_videoWidget->setFullScreen(false);
            }
            break;
        case Phonon::PlayingState:
            playButton->setEnabled(true);
            playButton->setIcon(pauseIcon);
            playButton->setToolTip(tr("Pause"));
            if (m_pmedia.hasVideo())
                m_videoWindow.show();
        case Phonon::BufferingState:
            rewindButton->setEnabled(true);
            break;
        case Phonon::LoadingState:
            rewindButton->setEnabled(false);
            break;
    }
}

void MediaPlayer::updateInfo()
{
    QString fileName = m_pmedia.currentSource().fileName();
    fileName = fileName.right(fileName.length() - fileName.lastIndexOf('/') - 1);
    fileName = fileName.left(fileName.lastIndexOf('.'));

    nameLabel->setText(fileName);
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

void MediaPlayer::rewind()
{
    m_pmedia.setTickInterval(50);
    m_pmedia.seek(0);
}


void MediaPlayer::forward()
{
    QList<Phonon::MediaSource> queue = m_pmedia.queue();
    if (queue.size() > 0) {
        setFile(queue[0].fileName());
        forwardButton->setEnabled(queue.size() > 1);
    }
}

void MediaPlayer::hasVideoChanged(bool bHasVideo)
{
    m_videoWindow.setVisible(bHasVideo);
}

void MediaPlayer::showContextMenu(const QPoint &p)
{
//    fileMenu->popup(m_videoWidget->isFullScreen() ? p : mapToGlobal(p));
}
