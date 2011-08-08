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

MWidget::MWidget(QWidget *p) : QWidget(p,  Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint)
{
    setWindowModality(Qt::WindowModal);

//    setWindowOpacity(0.3);

    mLabel = new QLabel(this);
    mLabel->setObjectName("mLabel");
    mLabel->setMinimumHeight(40);
    mLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
//    mLabel->setAttribute(Qt::WA_TranslucentBackground);
    mLabel->setText("Test");
    QVBoxLayout* pvbxLayout = new QVBoxLayout(this);
    pvbxLayout->setContentsMargins(0, 0, 0, 0);
    pvbxLayout->addSpacing(15);
    pvbxLayout->addWidget(mLabel);
    pvbxLayout->addStretch(1);

//    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);

    setGeometry(0 , 0, QApplication::desktop()->geometry().width(), QApplication::desktop()->geometry().height());
}

ControlWidget::ControlWidget(MediaPlayer *player, QWidget *p) :
    QWidget(p,  Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint),
    m_player(player)
{
    setWindowModality(Qt::WindowModal);
    setAttribute(Qt::WA_TranslucentBackground);
    setObjectName("controlWidget");

    buttonPanelWidget = new QWidget(this);
    buttonPanelWidget->setObjectName("buttonPanelWidget");
    openButton    = new QPushButton(this);
    playButton    = new QPushButton(this);
    stopButton    = new QPushButton(this);
    rewindButton  = new QPushButton(this);
    forwardButton = new QPushButton(this);
//    playlistButton = new QPushButton(this);
    statusLabel  = new QLabel(this);
    statusLabel->setObjectName("statusLabel");
    timeLabel  = new QLabel(this);
    timeLabel->setObjectName("timeLabel");
    slider = new Phonon::SeekSlider(this);
    slider->setObjectName("seekSliderOver");
    volume = new Phonon::VolumeSlider(this);
    volume->setObjectName("volumeSliderOver");
    volume->setMuteVisible(false);

    volumeLabel = new QLabel(this);
    volumeIcon = style()->standardPixmap(QStyle::SP_MediaVolume);
    mutedIcon = style()->standardPixmap(QStyle::SP_MediaVolumeMuted);
    volumeLabel->setPixmap(volumeIcon);
    volumeLabel->setCursor(Qt::PointingHandCursor);

    slider->setMediaObject(&player->m_pmedia);
    slider->setCursor(Qt::PointingHandCursor);

    volume->setAudioOutput(&player->m_AudioOutput);
    volume->setFixedWidth(100);
    volume->setCursor(Qt::PointingHandCursor);

    openButton->setIcon(QIcon(":/Res/Open.png"));
    playIcon = QIcon(":/Res/Play1.png");
    pauseIcon = QIcon(":/Res/Pause1.png");
    playButton->setIcon(playIcon);
    playButton->setObjectName("playButton");
    stopButton->setIcon(QIcon(":/Res/Stop1.png"));
    rewindButton->setIcon(QIcon(":/Res/Rewind1.png"));
    forwardButton->setIcon(QIcon(":/Res/Forward1.png"));
//    playlistButton->setIcon(QIcon(":/res/Playlist1.png"));

    openButton->setToolTip(tr("Open"));
    playButton->setToolTip(tr("Play"));
    stopButton->setToolTip(tr("Stop"));
    rewindButton->setToolTip(tr("Previous"));
    forwardButton->setToolTip(tr("Next"));
//    playlistButton->setToolTip(tr("Playlist (show/hide)"));

    openButton->setFocusPolicy(Qt::NoFocus);
    playButton->setFocusPolicy(Qt::NoFocus);
    stopButton->setFocusPolicy(Qt::NoFocus);
    rewindButton->setFocusPolicy(Qt::NoFocus);
    forwardButton->setFocusPolicy(Qt::NoFocus);
//    playlistButton->setFocusPolicy(Qt::NoFocus);
    volume->setFocusPolicy(Qt::NoFocus);
    slider->setFocusPolicy(Qt::NoFocus);

    openButton->setCursor(Qt::PointingHandCursor);
    playButton->setCursor(Qt::PointingHandCursor);
    stopButton->setCursor(Qt::PointingHandCursor);
    rewindButton->setCursor(Qt::PointingHandCursor);
    forwardButton->setCursor(Qt::PointingHandCursor);
//    playlistButton->setCursor(Qt::PointingHandCursor);

    timeLabel->setAlignment(Qt::AlignRight);
    statusLabel->setAlignment(Qt::AlignLeft);

    //Layout setup
    QHBoxLayout* phbxLayout = new QHBoxLayout;
    phbxLayout->addWidget(openButton);
    phbxLayout->addSpacing(10);
    phbxLayout->addWidget(playButton);
    phbxLayout->addWidget(stopButton);
    phbxLayout->addWidget(rewindButton);
    phbxLayout->addWidget(forwardButton);
    phbxLayout->addStretch();
    phbxLayout->addWidget(volumeLabel);
    phbxLayout->addWidget(volume);
//    phbxLayout->addSpacing(10);
//    phbxLayout->addWidget(playlistButton);

    QHBoxLayout* phbxLayout2 = new QHBoxLayout;
    phbxLayout2->addWidget(slider);

    QHBoxLayout* phbxLayout3 = new QHBoxLayout;
    phbxLayout3->addWidget(statusLabel);
    phbxLayout3->addSpacing(20);
    phbxLayout3->addWidget(timeLabel);

    QVBoxLayout *buttonPanelLayout = new QVBoxLayout();
    buttonPanelLayout->setMargin(5);
    buttonPanelLayout->setSpacing(5);
    buttonPanelLayout->addLayout(phbxLayout2);
    buttonPanelLayout->addLayout(phbxLayout3);
    buttonPanelLayout->addLayout(phbxLayout);

    buttonPanelWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    buttonPanelWidget->setLayout(buttonPanelLayout);

    QVBoxLayout* pvbxLayout = new QVBoxLayout(this);
    pvbxLayout->setContentsMargins(0, 0, 0, 0);
    pvbxLayout->addWidget(buttonPanelWidget);
    setLayout(pvbxLayout);

    connect(openButton, SIGNAL(clicked()), m_player, SLOT(openFile()));
    connect(playButton, SIGNAL(clicked()), m_player, SLOT(playPause()));
    connect(stopButton, SIGNAL(clicked()), m_player, SLOT(stop()));
    connect(rewindButton, SIGNAL(clicked()), m_player, SLOT(rewind()));
    connect(forwardButton, SIGNAL(clicked()), m_player, SLOT(forward()));
//    connect(playlistButton, SIGNAL(clicked()), m_player, SLOT(playlistShow()));


    setMinimumWidth(QApplication::desktop()->availableGeometry().width());
    move(0, QApplication::desktop()->availableGeometry().height() - sizeHint().height());
}

MediaVideoWidget::MediaVideoWidget(MediaPlayer *player, QWidget *parent) :
    Phonon::VideoWidget(parent), m_player(player), m_action(this)
{
    m_action.setCheckable(true);
    m_action.setChecked(false);
    m_action.setShortcut(QKeySequence( Qt::AltModifier + Qt::Key_Return));
    m_action.setShortcutContext(Qt::WindowShortcut);
    m_action.setPriority(QAction::LowPriority);
    connect(&m_action, SIGNAL(toggled(bool)), SLOT(setFullScreen(bool)));
    addAction(&m_action);
    setAcceptDrops(true);
}

void MediaVideoWidget::setFullScreen(bool enabled)
{
//    setFocus();
    if (!enabled) {
        m_timer.stop();
        m_player->activateWindow();
        setCursor(Qt::PointingHandCursor);
        m_player->cWidget->hide();
    } else {
        activateWindow();
        setCursor(Qt::BlankCursor);
        m_player->cWidget->show();
        m_timer.start(3000, this);
    }
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
//    if (e->button() == Qt::LeftButton) m_player->playPause();
    setCursor(Qt::PointingHandCursor);
}

void MediaVideoWidget::mouseMoveEvent(QMouseEvent *e)
{
    setCursor(Qt::PointingHandCursor);
    if (isFullScreen()) {
        if (!m_player->fileMenu->isVisible()) {
            m_timer.start(5000, this);
            m_player->cWidget->show();
        }
    }
}

void MediaVideoWidget::timerEvent(QTimerEvent *e)
{
    if (e->timerId() == m_timer.timerId()) {
        if (!m_player->fileMenu->isVisible()) setCursor(Qt::BlankCursor);
        if (!m_player->cWidget->underMouse()) m_player->cWidget->hide();
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
//    playButton(0),
    m_AudioOutput(Phonon::VideoCategory),
    m_videoWidget(new MediaVideoWidget(this))
{
    setWindowTitle("EMP");

    setContextMenuPolicy(Qt::CustomContextMenu);
    m_videoWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    cWidget = new ControlWidget(this);

    QWidget *wgt = new QWidget;

    buttonPanelWidget = new QWidget(this);
    openButton    = new QPushButton(wgt);
    playButton    = new QPushButton(wgt);
    stopButton    = new QPushButton(wgt);
    rewindButton  = new QPushButton(wgt);
    forwardButton = new QPushButton(wgt);
    playlistButton = new QPushButton(wgt);
    statusLabel  = new QLabel(wgt);
    timeLabel  = new QLabel(wgt);
    slider = new Phonon::SeekSlider(wgt);
    slider->setObjectName("seekSlider");
    volume = new Phonon::VolumeSlider(wgt);
    volume->setObjectName("volumeSlider");
    volume->setAudioOutput(&m_AudioOutput);
    volume->setFixedWidth(100);
    volume->setCursor(Qt::PointingHandCursor);
    volume->setMuteVisible(false);

    volumeLabel = new QLabel(this);
    volumeIcon = style()->standardPixmap(QStyle::SP_MediaVolume);
    mutedIcon = style()->standardPixmap(QStyle::SP_MediaVolumeMuted);
    volumeLabel->setPixmap(volumeIcon);
    volumeLabel->setCursor(Qt::PointingHandCursor);

    playListDoc = new QDockWidget("PLAYLIST", this);
    playListDoc->setObjectName("playListDoc");
    playListDoc->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    model = new QStandardItemModel(0, 4);
    playListView = new QTableView;

    slider->setMediaObject(&m_pmedia);
    slider->setCursor(Qt::PointingHandCursor);

    openButton->setIcon(QIcon(":/Res/Open.png"));
    playIcon = QIcon(":/Res/Play1.png");
    pauseIcon = QIcon(":/Res/Pause1.png");
    playButton->setIcon(playIcon);
    playButton->setObjectName("playButton");
    stopButton->setIcon(QIcon(":/Res/Stop1.png"));
    rewindButton->setIcon(QIcon(":/Res/Rewind1.png"));
    forwardButton->setIcon(QIcon(":/Res/Forward1.png"));
    playlistButton->setIcon(QIcon(":/res/Playlist1.png"));

    openButton->setToolTip(tr("Open"));
    playButton->setToolTip(tr("Play"));
    stopButton->setToolTip(tr("Stop"));
    rewindButton->setToolTip(tr("Previous"));
    forwardButton->setToolTip(tr("Next"));
    playlistButton->setToolTip(tr("Playlist (show/hide)"));

    openButton->setFocusPolicy(Qt::NoFocus);
    playButton->setFocusPolicy(Qt::NoFocus);
    stopButton->setFocusPolicy(Qt::NoFocus);
    rewindButton->setFocusPolicy(Qt::NoFocus);
    forwardButton->setFocusPolicy(Qt::NoFocus);
    playlistButton->setFocusPolicy(Qt::NoFocus);
    volume->setFocusPolicy(Qt::NoFocus);
    slider->setFocusPolicy(Qt::NoFocus);

    openButton->setCursor(Qt::PointingHandCursor);
    playButton->setCursor(Qt::PointingHandCursor);
    stopButton->setCursor(Qt::PointingHandCursor);
    rewindButton->setCursor(Qt::PointingHandCursor);
    forwardButton->setCursor(Qt::PointingHandCursor);
    playlistButton->setCursor(Qt::PointingHandCursor);

    timeLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    statusLabel->setAlignment(Qt::AlignLeft);

    QHBoxLayout* phbxLayout = new QHBoxLayout;
    phbxLayout->addWidget(openButton);
    phbxLayout->addSpacing(10);
    phbxLayout->addWidget(playButton);
    phbxLayout->addWidget(stopButton);
    phbxLayout->addWidget(rewindButton);
    phbxLayout->addWidget(forwardButton);
//    phbxLayout->addSpacing(10);
    phbxLayout->addStretch();
    phbxLayout->addWidget(volumeLabel);
    phbxLayout->addWidget(volume);
    phbxLayout->addSpacing(10);
    phbxLayout->addWidget(playlistButton);

    QHBoxLayout* phbxLayout2 = new QHBoxLayout;
    phbxLayout2->addWidget(slider);

    QHBoxLayout* phbxLayout3 = new QHBoxLayout;
    phbxLayout3->addWidget(statusLabel);
    phbxLayout3->addSpacing(20);
    phbxLayout3->addWidget(timeLabel);

    QVBoxLayout *buttonPanelLayout = new QVBoxLayout();
    buttonPanelLayout->setMargin(5);
    buttonPanelLayout->setSpacing(5);
    buttonPanelLayout->addLayout(phbxLayout2);
    buttonPanelLayout->addLayout(phbxLayout3);
    buttonPanelLayout->addLayout(phbxLayout);

    buttonPanelWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    buttonPanelWidget->setLayout(buttonPanelLayout);

    controlPanel = new QToolBar;
    controlPanel->setObjectName("controlPanel");
    controlPanel->setFloatable(false);
    controlPanel->setMovable(false);
    controlPanel->addWidget(buttonPanelWidget);
    addToolBar(Qt::BottomToolBarArea, controlPanel);

    playListView->setModel(model);
    playListView->setObjectName("playList");
    playListView->setColumnHidden(0, true);
    playListView->setColumnHidden(3, true);
    playListView->horizontalHeader()->hide();
    playListView->verticalHeader()->hide();
    playListView->setSelectionBehavior( QAbstractItemView::SelectRows );
    playListView->setSelectionMode(QAbstractItemView::ContiguousSelection);

    playListView->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
    playListView->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    playListView->horizontalHeader()->setResizeMode(2, QHeaderView::ResizeToContents);
    playListView->setShowGrid(false);
    playListView->verticalHeader()->setDefaultSectionSize ( playListView->verticalHeader()->minimumSectionSize () );

    playListView->setFocusPolicy(Qt::NoFocus);
    playListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    playListView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    playListDoc->setWidget(playListView);
    playListDoc->setMinimumWidth(160);
    playListDoc->setParent(m_videoWidget);
    addDockWidget(Qt::RightDockWidgetArea, playListDoc);
    playListDoc->hide();

    initVideoWindow();
//    m_videoWindow.hide();
    setCentralWidget(&m_videoWindow);

    // Create menu bar:
    fileMenu = new QMenu(this);
    QAction *openFileAction = fileMenu->addAction(tr("Open File..."));
    QAction *openUrlAction = fileMenu->addAction(tr("Open Location..."));

    fileMenu->addSeparator();
    playPauseAction = fileMenu->addAction(tr("Play/Pause"),
                                                   this, SLOT(playPause()), Qt::Key_Space);
    playPauseAction->setCheckable(true);
    playPauseAction->setChecked(true);

    fileMenu->addSeparator();
    fullScreenAction = fileMenu->addAction(tr("Full screen"),
                                                    m_videoWidget, SLOT(setFullScreen(bool)), Qt::Key_F11);
    fullScreenAction->setCheckable(true);

    QMenu *aspectMenu = fileMenu->addMenu(tr("Aspect ratio"));
    QActionGroup *aspectGroup = new QActionGroup(aspectMenu);
    connect(aspectGroup, SIGNAL(triggered(QAction *)), this, SLOT(aspectChanged(QAction *)));
    aspectGroup->setExclusive(true);
    QAction *aspectActionAuto = aspectMenu->addAction(tr("Default"));
    aspectActionAuto->setCheckable(true);
    aspectActionAuto->setChecked(true);
    aspectActionAuto->setObjectName("aspectAuto");
    aspectGroup->addAction(aspectActionAuto);
    QAction *aspectAction4_3 = aspectMenu->addAction("4/3");
    aspectAction4_3->setCheckable(true);
    aspectAction4_3->setObjectName("aspect4/3");
    aspectGroup->addAction(aspectAction4_3);
    QAction *aspectAction16_9 = aspectMenu->addAction("16/9");
    aspectAction16_9->setCheckable(true);
    aspectAction16_9->setObjectName("aspect16/9");
    aspectGroup->addAction(aspectAction16_9);
    QAction *aspectActionScale = aspectMenu->addAction(tr("Stretch to window"));
    aspectActionScale->setCheckable(true);
    aspectActionScale->setObjectName("aspectScale");
    aspectGroup->addAction(aspectActionScale);

//    fileMenu->addSeparator();
//    QAction *settingsAction = fileMenu->addAction(tr("&Settings..."));

//    openButton->setMenu(fileMenu);

    connect(openFileAction, SIGNAL(triggered(bool)), this, SLOT(openFile()));
    connect(openUrlAction, SIGNAL(triggered(bool)), this, SLOT(openUrl()));

    connect(openButton, SIGNAL(clicked()), this, SLOT(openFile()));
    connect(playButton, SIGNAL(clicked()), this, SLOT(playPause()));
    connect(stopButton, SIGNAL(clicked()), this, SLOT(stop()));
    connect(rewindButton, SIGNAL(clicked()), this, SLOT(rewind()));
    connect(forwardButton, SIGNAL(clicked()), this, SLOT(forward()));
    connect(playlistButton, SIGNAL(clicked()), this, SLOT(playlistShow()));

    connect(m_videoWidget, SIGNAL(customContextMenuRequested(const QPoint &)), SLOT(showContextMenu(const QPoint &)));
    connect(&m_pmedia, SIGNAL(metaDataChanged()), this, SLOT(updateInfo()));
    connect(&m_pmedia, SIGNAL(totalTimeChanged(qint64)), this, SLOT(updateTime()));
    connect(&m_pmedia, SIGNAL(tick(qint64)), this, SLOT(updateTime()));
    connect(&m_pmedia, SIGNAL(stateChanged(Phonon::State, Phonon::State)), this, SLOT(stateChanged(Phonon::State, Phonon::State)));
    connect(&m_pmedia, SIGNAL(bufferStatus(int)), this, SLOT(bufferStatus(int)));
    connect(&m_AudioOutput, SIGNAL(volumeChanged(qreal)), this, SLOT(volumeChanged(qreal)));
    connect(&m_pmedia, SIGNAL(hasVideoChanged(bool)), this, SLOT(hasVideoChanged(bool)));

    m_pmedia.setTickInterval(100);

    playButton->setEnabled(false);
    rewindButton->setEnabled(false);
    forwardButton->setEnabled(false);
    setAcceptDrops(true);

    m_audioOutputPath = Phonon::createPath(&m_pmedia, &m_AudioOutput);
    Phonon::createPath(&m_pmedia, m_videoWidget);

    readSettings();

    if (!filePath.isEmpty()){
        setFile(filePath);
        addFile(filePath);
    }
    resize(minimumSizeHint());

    moveWindowToCenter();

    qApp->installEventFilter( this );
    activateWindow();
}

// ----------------------------------------------------------------------
/*virtual*/ MediaPlayer::~MediaPlayer()
{
    qDebug() << "~MediaPlayer";
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

    restoreGeometry(m_settings->value("GeometryState").toByteArray());
    restoreState(m_settings->value("ToolBarsState").toByteArray());
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

    m_settings->setValue("GeometryState", saveGeometry());
    m_settings->setValue("ToolBarsState", saveState());
}

/*virtual*/ void MediaPlayer::closeEvent(QCloseEvent* pe)
{
    qDebug() << "closeEventStart";
    if (m_pmedia.hasVideo()) {
        qDebug() << "closeEvent(1)";
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
    delete m_videoWidget;
    writeSettings();
    qDebug() << "closeEventStop";
}

/*virtual*/ void MediaPlayer::showEvent(QShowEvent*)
{

}

/*virtual*/ bool MediaPlayer::eventFilter(QObject* pobj, QEvent* pe)
{
    if (pe->type() == QEvent::Close) {
        if (pobj == m_videoWidget) {
            qDebug() << "QEvent::Close (m_videoWidget)";
            close();
            return true;
        } else {
            qDebug() << "QEvent::Close (1)";
        }
    }

    if (pe->type() == QEvent::ToolTip) {
        if ((pobj == cWidget->volume) || (pobj == volume)) {
            QString vol = tr("Volume") + " [" + QString::number(qreal(m_AudioOutput.volume()*100.0f)) + "%]";
            cWidget->volume->setToolTip(vol);
            volume->setToolTip(vol);
        }
    }

    if (pe->type() == QEvent::MouseButtonPress) {
        if ((pobj->parent() == cWidget->volume) || (pobj->parent() == cWidget->slider) ||
            (pobj->parent() == volume) || (pobj->parent() == slider)) {
            if (((QMouseEvent*)pe)->button() == Qt::LeftButton) {
                QMouseEvent* pe1 = new QMouseEvent(QEvent::MouseButtonPress, ((QMouseEvent*)pe)->pos(),
                                              Qt::MidButton, Qt::MidButton, Qt::NoModifier);
                QApplication::sendEvent(pobj, pe1);
            }
        }
        if ((pobj == cWidget->volumeLabel) || (pobj == volumeLabel)) {
            setVolumeOnOff();
        }
    }

    if (pe->type() == QEvent::KeyPress) {
        if (!((QKeyEvent*)pe)->modifiers()) {
            if (((QKeyEvent*)pe)->key() == Qt::Key_Escape) {
                if (m_videoWidget->isFullScreen()) {
                    m_videoWidget->setFullScreen(false);
                    playPause();
                    return true;
                }
            } else if (((QKeyEvent*)pe)->key() == Qt::Key_Space) {
                playPause();
                return true;
            } else if (((QKeyEvent*)pe)->key() == Qt::Key_F11 && m_pmedia.hasVideo()) {
//                  if (isFullScreen()){
//                      cWidget1->hide();
//                      showNormal();
//                      controlPanel->show();
//                      playListDoc->show();
//                  } else {
//                      controlPanel->hide();
//                      playListDoc->hide();
//                      showFullScreen();
//                      cWidget1->show();
//                  }

                m_videoWidget->setFullScreen(!m_videoWidget->isFullScreen());
                return true;
            } else if (((QKeyEvent*)pe)->key() == Qt::Key_Left) {
                if (m_pmedia.isSeekable()) {
                    long pos = m_pmedia.currentTime() - 15000;
                    if (pos > 0) m_pmedia.seek(pos);
                    else m_pmedia.seek(0);
                }
                return true;
            } else if (((QKeyEvent*)pe)->key() == Qt::Key_Right) {
                if (m_pmedia.isSeekable()) {
                    long pos = m_pmedia.currentTime() + 15000;
                    if (pos < m_pmedia.totalTime()) m_pmedia.seek(pos);
                }
                return true;
            } else if (((QKeyEvent*)pe)->key() == Qt::Key_Up) {
                qreal vol = m_AudioOutput.volume() + 0.05;
                if (vol <= 1) m_AudioOutput.setVolume(vol);
                else m_AudioOutput.setVolume(1);
                return true;
            } else if (((QKeyEvent*)pe)->key() == Qt::Key_Down) {
                qreal vol = m_AudioOutput.volume() - 0.05;
                if (vol >= 0.01) m_AudioOutput.setVolume(vol);
                else m_AudioOutput.setVolume(0);
                return true;
            }
        }
    }

    return false;
}

void MediaPlayer::timerEvent(QTimerEvent *pe)
{
    if (pe->timerId() == m_timer.timerId()) {
        updateInfo();
    }
    QWidget::timerEvent(pe);
}

void MediaPlayer::volumeChanged(qreal v)
{
    QString vol = tr("Volume") + " [" + QString::number(qreal(v*100.0f)) + "%]";
    statusLabel->setText(vol);
    cWidget->statusLabel->setText(vol);
    m_timer.start(5000, this);
}

void MediaPlayer::setVolumeOnOff () {
    if (m_AudioOutput.isMuted()) {
        m_AudioOutput.setMuted(false);
        volumeLabel->setPixmap(volumeIcon);
        cWidget->volumeLabel->setPixmap(cWidget->volumeIcon);
    } else {
        m_AudioOutput.setMuted(true);
        volumeLabel->setPixmap(mutedIcon);
        cWidget->volumeLabel->setPixmap(cWidget->mutedIcon);
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
    m_videoWindow.setMinimumSize(250, 200);
}

// ----------------------------------------------------------------------
void MediaPlayer::openFile()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this);
    if (fileNames.size() > 0) {
        m_pmedia.clearQueue();
//        model->clear();
        QString fileName = fileNames[0];
        setFile(fileName);
        addFile(fileName);
        for (int i=1; i<fileNames.size(); i++)
          addFile(fileNames[i]);
//        for (int i=1; i<fileNames.size(); i++)
//            m_pmedia.enqueue(Phonon::MediaSource(fileNames[i]));
    }
}

void MediaPlayer::openUrl()
{
    QSettings settings;
    settings.beginGroup(QLatin1String("BrowserMainWindow"));
    QString sourceURL = settings.value("location").toString();
    sourceURL = QInputDialog::getText(this, tr("Open Location"), tr("Please enter a valid address here:"), QLineEdit::Normal, sourceURL);
    if (!sourceURL.isEmpty()) {
        setFile(sourceURL);
        settings.setValue("location", sourceURL);
    }
}

void MediaPlayer::handleDrop(QDropEvent *e)
{
    qDebug() << "handleDropStart";
    activateWindow();
    QList<QUrl> urls = e->mimeData()->urls();
    if (e->proposedAction() == Qt::MoveAction){
        // Добавляем в очередь
//        for (int i=0; i<urls.size(); i++)
//            m_pmedia.enqueue(Phonon::MediaSource(urls[i].toLocalFile()));
    } else {
        // Создаём новую очередь
        m_pmedia.clearQueue();
        if (urls.size() > 0) {
            QString fileName = urls[0].toLocalFile();
            QDir dir(fileName);
            if (dir.exists()) {
                dir.setFilter(QDir::Files);
                QStringList entries = dir.entryList();
                if (entries.size() > 0) {
                    setFile(fileName + QDir::separator() +  entries[0]);
                    addFile(fileName + QDir::separator() +  entries[0]);
                    for (int i=1; i<entries.size(); i++)
                      addFile(fileName + QDir::separator() + entries[i]);
                    for (int i=1; i< entries.size(); ++i)
                        m_pmedia.enqueue(fileName + QDir::separator() + entries[i]);
                }
                qDebug() << "handleDrop1";
            } else {
                setFile(fileName);
                addFile(fileName);
                for (int i=1; i<urls.size(); i++)
                  addFile(urls[i].toLocalFile());
                for (int i=1; i<urls.size(); i++)
                    m_pmedia.enqueue(Phonon::MediaSource(urls[i].toLocalFile()));
                qDebug() << "handleDrop2";
            }
        }
    }
    forwardButton->setEnabled(m_pmedia.queue().size() > 0);
    cWidget->forwardButton->setEnabled(m_pmedia.queue().size() > 0);
    qDebug() << "handleDropStop";
}

void MediaPlayer::dropEvent(QDropEvent *e)
{
    qDebug() << "dropEvent";
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
    if (m_pmedia.state() == Phonon::PlayingState) {
        qDebug() << "playPause(1)";
        m_pmedia.pause();
        playPauseAction->setChecked(true);
    } else if (m_pmedia.state() == Phonon::PausedState) {
        qDebug() << "playPause(2)";
        if (m_pmedia.currentTime() == m_pmedia.totalTime())
            m_pmedia.seek(0);
        m_pmedia.play();
        playPauseAction->setChecked(true);
    } else qDebug() << "playPause(3)";
    cWidget->activateWindow();
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
    playPauseAction->setChecked(false);
}

void MediaPlayer::playlistShow()
{
    if (playListDoc->isVisible()) {
        playListDoc->hide();
    } else {
        playListDoc->show();
    }
}

void MediaPlayer::setFile(const QString &fileName)
{
    if (model->rowCount() == 0)
        m_pmedia.setCurrentSource(Phonon::MediaSource(fileName));
}

void MediaPlayer::addFile(QString fileName)
{
    QString Name = fileName;
    int t = Name.lastIndexOf('\\');
    if (t < 0) t = Name.lastIndexOf('/');
    Name = Name.right(Name.length() - t - 1);

    int row = model->rowCount();
    model->insertRows(row, 1);
    QModelIndex index = model->index(row, 0);               // 0
    model->setData(index, QString::number(row));

    index = model->index(row, 1);                           // 1
    model->setData(index, QString::number(row+1) + ". " + Name, Qt::DisplayRole);
    if (row == 0) {
        model->setData(index, Qt::white, Qt::TextColorRole);
        model->setData(index, QBrush(QColor(100, 100, 100), Qt::SolidPattern), Qt::BackgroundRole);
    }

    long len = 0;
    QString timeString;
    int sec = len/1000;
    int min = sec/60;
    int hour = min/60;
    int msec = len;
    QTime mTime(hour%60, min%60, sec%60, msec%1000);
    QString timeFormat = "h:mm:ss";
    timeString = mTime.toString(timeFormat);
    index = model->index(row, 2);                           // 2
    model->setData(index, timeString);
    if (row == 0) {
        model->setData(index, Qt::white, Qt::TextColorRole);
        model->setData(index, QBrush(QColor(100, 100, 100), Qt::SolidPattern), Qt::BackgroundRole);
    }

    index = model->index(row, 3);                           // 3
    model->setData(index, fileName);
}

void MediaPlayer::stateChanged(Phonon::State newstate, Phonon::State oldstate)
{
    qDebug() << "stateChanged";
    Q_UNUSED(oldstate);

    if (oldstate == Phonon::LoadingState) {
//        m_videoWindow.setVisible(m_pmedia.hasVideo());
        if (m_pmedia.hasVideo()){
            // Flush event que so that sizeHint takes the
            // recently shown/hidden m_videoWindow into account:
            qDebug() << "processEvents";
            qApp->processEvents();
            QPoint posCOld = frameGeometry().center();
            QSize frame;
            QSize newSize;
            frame.setWidth(frameSize().width() - size().width());
            frame.setHeight(frameSize().height() - size().height());
            newSize.setWidth(m_videoWindow.sizeHint().width() + frame.width());
            newSize.setHeight(m_videoWindow.sizeHint().height() + buttonPanelWidget->height() +
                    buttonPanelWidget->pos().y() + 0 + frame.height());
            if (playListDoc->isVisible()) newSize.setWidth(newSize.width() + playListDoc->width());

            QRect videoHintRect = QRect(QPoint(0, 0), newSize);
            QRect newVideoRect = QApplication::desktop()->availableGeometry(0).intersected(videoHintRect);

            QPoint posNew;
            posNew.setX(posCOld.x() - (newVideoRect.width()- frame.width())/2);
            posNew.setY(posCOld.y() - (newVideoRect.height() - frame.height())/2);
            if (posNew.x() < 0) posNew.setX(0);
            else if ((posNew.x() + newVideoRect.width()) >
                     QApplication::desktop()->availableGeometry(0).width()) {
                posNew.setX(QApplication::desktop()->availableGeometry(0).width() -
                            newVideoRect.width());
            }
            if (posNew.y() < 0) posNew.setY(0);
            else if ((posNew.y() + newVideoRect.height()) >
                     QApplication::desktop()->availableGeometry(0).height()) {
                posNew.setY(QApplication::desktop()->availableGeometry(0).height() -
                            newVideoRect.height());
            }

            resize(newVideoRect.width()- frame.width(), newVideoRect.height() - frame.height());
            move(posNew);

        } else resize(minimumSize());

        for (int i = 0; i < MAX_FILE_POS; ++i) {
            if (fileNameP[i] == m_pmedia.currentSource().fileName()) {
                if (m_pmedia.isSeekable())
                    m_pmedia.seek(filePos[i]);
                break;
            }
        }
        m_pmedia.play();
        qDebug() << "Play";
    }

    switch (newstate) {
        case Phonon::ErrorState:
            QMessageBox::warning(this, "EMP", m_pmedia.errorString(), QMessageBox::Close);
            if (m_pmedia.errorType() == Phonon::FatalError) {
                playButton->setEnabled(false);
                rewindButton->setEnabled(false);
                cWidget->playButton->setEnabled(false);
                cWidget->rewindButton->setEnabled(false);
            } else {
                m_pmedia.pause();
            }
            break;
        case Phonon::StoppedState:
        case Phonon::PausedState: 
            playButton->setIcon(cWidget->playIcon);
            playButton->setToolTip(tr("Play"));
            cWidget->playButton->setIcon(cWidget->playIcon);
            cWidget->playButton->setToolTip(tr("Play"));
            if (m_pmedia.currentSource().type() != Phonon::MediaSource::Invalid){
                playButton->setEnabled(true);
                rewindButton->setEnabled(true);
                cWidget->playButton->setEnabled(true);
                cWidget->rewindButton->setEnabled(true);
            } else {
                playButton->setEnabled(false);
                rewindButton->setEnabled(false);
                cWidget->playButton->setEnabled(false);
                cWidget->rewindButton->setEnabled(false);
            }
            if (m_pmedia.currentTime() == m_pmedia.totalTime()) {
                m_videoWidget->setFullScreen(false);
            }
            break;
        case Phonon::PlayingState:
            qDebug() << "PlayingState";
            playButton->setEnabled(true);
            playButton->setIcon(cWidget->pauseIcon);
            playButton->setToolTip(tr("Pause"));
            cWidget->playButton->setEnabled(true);
            cWidget->playButton->setIcon(cWidget->pauseIcon);
            cWidget->playButton->setToolTip(tr("Pause"));
//            if (m_pmedia.hasVideo())
//                m_videoWindow.show();
        case Phonon::BufferingState:
            rewindButton->setEnabled(true);
            cWidget->rewindButton->setEnabled(true);
            break;
        case Phonon::LoadingState:
            rewindButton->setEnabled(false);
            cWidget->rewindButton->setEnabled(false);
            break;
    }
}

void MediaPlayer::bufferStatus(int percent)
{
    if (percent == 0 || percent == 100){
        statusLabel->setText("");
        cWidget->statusLabel->setText("");
    } else {
        QString str("(");
        str += QString::number(percent);
        str += "%)";
        statusLabel->setText(str);
        cWidget->statusLabel->setText(str);
    }
    qDebug() << "bufferStatus";
}

void MediaPlayer::updateInfo()
{
    QString fileName = m_pmedia.currentSource().fileName();
    fileName = fileName.right(fileName.length() - fileName.lastIndexOf('/') - 1);
    fileName = fileName.left(fileName.lastIndexOf('.'));
    statusLabel->setText(fileName);
    cWidget->statusLabel->setText(fileName);
//    mWidget.mLabel->setText(fileName);

//    QPoint posNew;
//    QPoint posCOld = frameGeometry().center();
//    m_videoWindow.setVisible(m_pmedia.hasVideo());
    if (!m_pmedia.hasVideo()) resize(minimumSize());
//    posNew.setX(posCOld.x() - frameSize().width()/2);
//    posNew.setY(posCOld.y() - frameSize().height()/2);
//    move(posNew);
    QString t;
    long pos = m_pmedia.currentTime();
    t.sprintf("updateInfo %d", (int)pos);
    qDebug() << t;
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

        long nPos = len - pos;
        sec = nPos/1000;
        min = sec/60;
        hour = min/60;
        msec = nPos;
        QTime nPlayTime(hour%60, min%60, sec%60, msec%1000);

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
            timeString += " (-" + nPlayTime.toString(timeFormat) + ")";
            timeString += " / " + stopTime.toString(timeFormat);
    }
    timeLabel->setText(timeString);
    cWidget->timeLabel->setText(timeString);
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
        cWidget->forwardButton->setEnabled(queue.size() > 1);
    }
}

void MediaPlayer::hasVideoChanged(bool bHasVideo)
{
    QPoint posNew;
    QPoint posCOld = frameGeometry().center();
//    m_videoWindow.setVisible(bHasVideo);
    posNew.setX(posCOld.x() - frameSize().width()/2);
    posNew.setY(posCOld.y() - frameSize().height()/2);
    move(posNew);
}

void MediaPlayer::showContextMenu(const QPoint &p)
{
    m_videoWidget->setCursor(Qt::ArrowCursor);
    fileMenu->popup(m_videoWidget->isFullScreen() ? p : mapToGlobal(p));
}

void MediaPlayer::aspectChanged(QAction *act)
{
    if (act->objectName() == "aspect16/9")
        m_videoWidget->setAspectRatio(Phonon::VideoWidget::AspectRatio16_9);
    else if (act->objectName() == "aspectScale")
        m_videoWidget->setAspectRatio(Phonon::VideoWidget::AspectRatioWidget);
    else if (act->objectName() == "aspect4/3")
        m_videoWidget->setAspectRatio(Phonon::VideoWidget::AspectRatio4_3);
    else
        m_videoWidget->setAspectRatio(Phonon::VideoWidget::AspectRatioAuto);
}
