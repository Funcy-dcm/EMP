/* ======================================================================
**  MediaPlayer.cpp
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2009 by Max Schlee
** ======================================================================
*/

#include <QtGui>
#include <QtNetwork>
#include "VersionNo.h"
#include "EMP.h"

libvlc_media_player_t *_curPlayer = NULL;

void MediaPlayer::receiveMessage(const QString& message)
{
    qDebug() << QString("Received message: %1").arg(message);
    if (!message.isEmpty()){
        activateWindow();

        model->clear();
        model->setColumnCount(4);
        playListView->setColumnHidden(0, true);
        playListView->setColumnHidden(3, true);
        playListView->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
        playListView->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
        playListView->horizontalHeader()->setResizeMode(2, QHeaderView::ResizeToContents);

        curPlayList = 0;
        setFile(message);
        addFile(message);
    }
}

// ----------------------------------------------------------------------
MediaPlayer::MediaPlayer(const QString &filePath)
{
    setWindowTitle("EMP");
    qDebug() << "2";

    m_videoWidget = new VlcVideoWidget(this);

    const char * const vlc_args[] = {
        "-I", "dummy",
        "--ignore-config",
        "--no-media-library",
        "--reset-plugins-cache",
        "--no-stats",
        "--no-osd",
        //        "--verbose=2",
        "--no-video-title-show"
    };
    _vlcinstance=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
    _m_player = libvlc_media_player_new (_vlcinstance);
    _curPlayer = _m_player;

    _timerState = new QTimer(this);

    setContextMenuPolicy(Qt::CustomContextMenu);
    m_videoWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    m_videoWidget->setCursor(Qt::PointingHandCursor);

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
    seekSlider = new VlcSeekSlider(wgt);
    seekSlider->setObjectName("seekSlider");
    volumeSlider = new VlcVolumeSlider(wgt);
    volumeSlider->setObjectName("volumeSlider");

    playListDoc = new QDockWidget("PLAYLIST", this);
    playListDoc->setObjectName("playListDoc");
    playListDoc->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    model = new QStandardItemModel(0, 4);
    playListView = new QTableView(this);

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

    openButton->setCursor(Qt::PointingHandCursor);
    playButton->setCursor(Qt::PointingHandCursor);
    stopButton->setCursor(Qt::PointingHandCursor);
    rewindButton->setCursor(Qt::PointingHandCursor);
    forwardButton->setCursor(Qt::PointingHandCursor);
    playlistButton->setCursor(Qt::PointingHandCursor);

    timeLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    statusLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QHBoxLayout* phbxLayout = new QHBoxLayout;
    phbxLayout->addWidget(openButton);
    phbxLayout->addSpacing(10);
    phbxLayout->addWidget(playButton);
    phbxLayout->addWidget(stopButton);
    phbxLayout->addWidget(rewindButton);
    phbxLayout->addWidget(forwardButton);
    phbxLayout->addStretch(1);
    phbxLayout->addSpacing(10);
    phbxLayout->addWidget(volumeSlider);
    phbxLayout->addSpacing(10);
    phbxLayout->addWidget(playlistButton);

    QHBoxLayout* phbxLayout2 = new QHBoxLayout;
    phbxLayout2->addWidget(seekSlider);

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
    playListView->setSelectionMode(QAbstractItemView::ExtendedSelection);

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
    addDockWidget(Qt::RightDockWidgetArea, playListDoc);
    playListDoc->hide();

    initVideoWindow();
    setCentralWidget(&sWidget);

    // Create menu bar:
    fileMenu = new QMenu(this);
    QAction *openFileAction = fileMenu->addAction(tr("Open File..."));

    fileMenu->addSeparator();
    playPauseAction = fileMenu->addAction(tr("Play/Pause"),
                                          this, SLOT(playPause()), Qt::Key_Space);
    playPauseAction->setCheckable(true);
    playPauseAction->setChecked(true);

    fileMenu->addSeparator();

    fullScreenAction = new QAction(this);
    fullScreenAction->setText(tr("Full screen"));
    fullScreenAction->setCheckable(true);
    fullScreenAction->setChecked(false);
    fullScreenAction->setShortcut(QKeySequence( Qt::AltModifier + Qt::Key_Return ));
    fullScreenAction->setShortcutContext(Qt::WindowShortcut);
    fullScreenAction->setPriority(QAction::QAction::HighPriority);
    connect(fullScreenAction, SIGNAL(toggled(bool)), this, SLOT(setFullScreen(bool)));
    addAction(fullScreenAction);
    fileMenu->addAction(fullScreenAction);

    connect(openFileAction, SIGNAL(triggered(bool)), this, SLOT(openFile()));
    connect(this, SIGNAL(signalWindowNormal()), this, SLOT(slotWindowNormal()), Qt::QueuedConnection);

    connect(openButton, SIGNAL(clicked()), this, SLOT(openFile()));
    connect(playButton, SIGNAL(clicked()), this, SLOT(playPause()));
    connect(stopButton, SIGNAL(clicked()), this, SLOT(stop()));
    connect(rewindButton, SIGNAL(clicked()), this, SLOT(rewind()));
    connect(forwardButton, SIGNAL(clicked()), this, SLOT(forward()));
    connect(playlistButton, SIGNAL(clicked()), this, SLOT(playlistShow()));

    connect(_timerState, SIGNAL(timeout()), this, SLOT(stateChanged()));

    playButton->setEnabled(false);
    rewindButton->setEnabled(false);
    forwardButton->setEnabled(false);
    setAcceptDrops(true);



    readSettings();
    controlPanel->show();

    if (!filePath.isEmpty()){
        setFile(filePath);
        addFile(filePath);
    } else {
        QFile filePL(qApp->applicationDirPath()+ "/default.epl");
        if (filePL.open(QIODevice::ReadOnly)){
            int rowCount = 0;
            QString fileName;
            QDataStream stream(&filePL);
            stream.setVersion(QDataStream::Qt_4_7);
            stream >> rowCount;
            for (int i = 0; i < rowCount; i++) {
                stream >> fileName;
                addFile(fileName);
            }
            if (stream.status() != QDataStream::Ok){
                qDebug() << "Ошибка чтения файла";
            }
            if (rowCount) playButton->setEnabled(true);
        }
        filePL.close();
    }
    fullScreenOn = false;
    setWindowState(Qt::WindowNoState);
    resize(minimumSizeHint());
    moveWindowToCenter();

    qApp->installEventFilter( this );
    activateWindow();

    curPlayList = 0;

    _timerState->start(10);
}

// ----------------------------------------------------------------------
/*virtual*/ MediaPlayer::~MediaPlayer()
{
    libvlc_media_player_stop (_m_player);
    libvlc_media_player_release (_m_player);
    libvlc_release (_vlcinstance);
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

    //    if (!isFullScreen()) {
    m_settings->setValue("GeometryState", saveGeometry());
    m_settings->setValue("ToolBarsState", saveState());
    //    }
}

/*virtual*/ void MediaPlayer::closeEvent(QCloseEvent* pe)
{
    qDebug() << "closeEventStart";
    if (libvlc_video_get_track_count(_m_player)) {
        qDebug() << "closeEvent(1)";
        bool findOk = 0;
        QModelIndex indexNew = model->index(curPlayList, 3);
        QString fileName = model->data(indexNew).toString();
        int fullTime = libvlc_media_player_get_length(_curPlayer);
        int currentTime = libvlc_media_player_get_time(_curPlayer);
        for (int i = 0; i < MAX_FILE_POS; ++i) {
            if (fileNameP[i] == fileName) {
                filePos[i] = currentTime;
                findOk = 1;
                break;
            }
        }

        if (!findOk) {
            for (int i = MAX_FILE_POS-1; i > 0 ; --i) {
                fileNameP[i] = fileNameP[i-1];
                filePos[i] = filePos[i-1];
            }
            fileNameP[0] = fileName;
            filePos[0] = currentTime;
        }

        if (currentTime == fullTime) {
            for (int i = 0; i < MAX_FILE_POS; ++i) {
                if (fileNameP[i] == fileName) {
                    filePos[i] = 0;
                    break;
                }
            }
        }
    }
    fullScreenAction->setChecked(false);
    writeSettings();

    QFile filePL(qApp->applicationDirPath()+ "/default.epl");
    if (filePL.open(QIODevice::WriteOnly)){
        QString fileName;
        QModelIndex index;
        QDataStream stream(&filePL);
        stream.setVersion(QDataStream::Qt_4_7);
        stream << model->rowCount();
        for (int i = 0; i < model->rowCount(); i++) {
            index = model->index(i, 3);
            fileName = model->data(index).toString();
            stream << fileName;
        }
        if (stream.status() != QDataStream::Ok){
            qDebug() << "Ошибка записи";
        }
    }
    filePL.close();

    qDebug() << "closeEventStop";
}

/*virtual*/ void MediaPlayer::resizeEvent(QResizeEvent* pe)
{

}

/*virtual*/ void MediaPlayer::moveEvent(QMoveEvent* pe)
{

}

void MediaPlayer::slotWindowNormal()
{
    setGeometry(nGeometryWindows);
}

void MediaPlayer::moveWindowToCenter()
{
    QRect frect = frameGeometry();
    frect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(frect.topLeft());
}

void MediaPlayer::initVideoWindow()
{
    QLabel *blackWidget = new QLabel(this);
    blackWidget->setObjectName("blackWidget");
    blackWidget->setCursor(Qt::BlankCursor);

    logoLabel = new QLabel(this);
    logoLabel->setObjectName("logoLabel");
    logoLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QPixmap pix;
    if (pix.load(qApp->applicationDirPath()+"/logo/logoEx.png") == 0)
        pix.load(":/res/Logo7.png");
    logoLabel->setPixmap(pix);

    QLabel *textb = new QLabel(logoLabel);
    textb->setText(QString("v")+STRFILEVER);
    textb->move(5, 5);

    sWidget.setMinimumSize(250, 200);
    sWidget.setContentsMargins(0, 0, 0, 0);
    sWidget.addWidget(m_videoWidget);   //0
    sWidget.addWidget(logoLabel);       //1
    sWidget.addWidget(blackWidget);     //2
    sWidget.setCurrentIndex(1);
    //
    blackWidget->setAttribute(Qt::WA_OpaquePaintEvent);
    blackWidget->setAttribute(Qt::WA_PaintOnScreen);
    blackWidget->setAttribute(Qt::WA_NoSystemBackground);
    QPalette p = palette();
    p.setColor(backgroundRole(), Qt::black);
    blackWidget->setPalette(p);
    //
}

bool MediaPlayer::isActive()
{
    if (_curPlayer == NULL)
        return false;

    libvlc_state_t state;
    state = libvlc_media_player_get_state(_curPlayer);

    if (state == libvlc_NothingSpecial || state == libvlc_Ended || state == libvlc_Error)
        return false;
    else
        return true;
}

void MediaPlayer::stateChanged()
{
    static int state_t = -1;

    timeLabel->setText(seekSlider->timeString);

    if (_m_player == NULL)
        return;

    int state;
    state = libvlc_media_player_get_state(_m_player);

//    if (state == libvlc_Playing)

    if (state != state_t) {
        switch (state) {
        case libvlc_NothingSpecial:
            qDebug() << "libvlc_NothingSpecial";
            break;
        case libvlc_Opening:
            qDebug() << "libvlc_Opening";
            break;
        case libvlc_Buffering:
            qDebug() << "libvlc_Buffering";
            break;
        case libvlc_Playing:
        {
            if (state_t == libvlc_Opening) {
                QModelIndex indexNew = model->index(curPlayList, 3);
                QString fileName = model->data(indexNew).toString();
                for (int i = 0; i < MAX_FILE_POS; ++i) {
                    if (fileNameP[i] == fileName) {
        //                if (m_pmedia.isSeekable())
                        libvlc_media_player_set_time(_m_player, filePos[i]);
                        qDebug() << "set_time:" << filePos[i];
                        break;
                    }
                }
                statusLabel->setText("");
            }
            sWidget.setCurrentIndex(0);
            playButton->setEnabled(true);
            playButton->setIcon(pauseIcon);
            playButton->setToolTip(tr("Pause"));
            rewindButton->setEnabled(true);
            break;
        }
        case libvlc_Stopped:
            sWidget.setCurrentIndex(1);
        case libvlc_Paused:
            playButton->setIcon(playIcon);
            playButton->setToolTip(tr("Play"));
            playButton->setEnabled(true);
            rewindButton->setEnabled(true);
            break;
        case libvlc_Ended:
            playButton->setIcon(playIcon);
            playButton->setToolTip(tr("Play"));
            break;
        case libvlc_Error:
            break;
        }
        state_t = state;
    }
}

// ----------------------------------------------------------------------
void MediaPlayer::openFile()
{
    QString filters = "";
    filters += tr("Media files (all types)") + " (" + EXTENSIONS_MEDIA + ");;";
    filters += tr("Video files") + " (" + EXTENSIONS_VIDEO + ");;";
    filters += tr("Audio files") + " (" + EXTENSIONS_AUDIO + ");;";
    //    filters += tr("Playlist files") + " (" + EXTENSIONS_PLAYLIST + ");;";
    filters += tr("All files") + " (*)";

    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open File..."), QDir::homePath(), filters);

    if (fileNames.size() > 0) {
        statusLabel->setText("Open...");

        model->clear();
        model->setColumnCount(4);
        playListView->setColumnHidden(0, true);
        playListView->setColumnHidden(3, true);
        playListView->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
        playListView->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
        playListView->horizontalHeader()->setResizeMode(2, QHeaderView::ResizeToContents);

        curPlayList = 0;
        QString fileName = fileNames[0];
        setFile(fileName);
        addFile(fileName);
        for (int i=1; i<fileNames.size(); i++)
            addFile(fileNames[i]);
    }
}

void MediaPlayer::playPause()
{
    if (libvlc_media_player_get_state(_m_player) == libvlc_Playing) {
        libvlc_audio_set_volume(_m_player, 0);
        libvlc_media_player_pause(_m_player);
    } else if ((libvlc_media_player_get_state(_m_player) == libvlc_Paused) ||
               (libvlc_media_player_get_state(_m_player) == libvlc_Stopped)) {
        libvlc_audio_set_volume(_m_player, volumeSlider->volume());
        libvlc_media_player_play(_m_player);
    } else {
        QModelIndex indexNew;
        if (curPlayList >= 0) {

        } else if (playListView->selectionModel()->hasSelection()) {
            curPlayList = playListView->selectionModel()->selectedRows().at(0).row();
            indexNew = model->index(curPlayList, 1);
            model->setData(indexNew, Qt::white, Qt::TextColorRole);
            model->setData(indexNew, QBrush(QColor(100, 100, 100), Qt::SolidPattern), Qt::BackgroundRole);
            indexNew = model->index(curPlayList, 2);
            model->setData(indexNew, Qt::white, Qt::TextColorRole);
            model->setData(indexNew, QBrush(QColor(100, 100, 100), Qt::SolidPattern), Qt::BackgroundRole);
        }
        indexNew = model->index(curPlayList, 3);
        QString fileName = model->data(indexNew).toString();

        _m = libvlc_media_new_path(_vlcinstance, QUrl::fromLocalFile(fileName).toEncoded());
        libvlc_media_player_set_media(_m_player, _m);
#if defined(Q_WS_WIN)
        libvlc_media_player_set_hwnd(_m_player, m_videoWidget->winId());
#elif defined(Q_WS_MAC)
        libvlc_media_player_set_agl(_m_player, m_videoWidget->winId());
#else // Q_WS_X11
        int windid = m_videoWidget->winId();
        libvlc_media_player_set_xwindow(_m_player, windid);
#endif // Q_WS_*
        libvlc_media_player_play(_m_player);
    }

    //    if (m_player.state() == Phonon::PlayingState) {
    //        qDebug() << "playPause(1)";
    //        m_player.pause();
    //        playPauseAction->setChecked(true);
    //        if (isFullScreen()) mLabel->showWidget(tr("Pause"));
    //    } else if (m_player.state() == Phonon::PausedState) {
    //        qDebug() << "playPause(2)";
    //        if (m_player.currentTime() == m_player.totalTime())
    //            m_player.seek(0);
    //        m_player.play();
    //        playPauseAction->setChecked(true);
    //        if (isFullScreen()) mLabel->showWidget(tr("Play"));
    //    } else {
    //        qDebug() << "playPause(3)";
    //        QModelIndex indexNew;
    //        if (curPlayList >= 0) {

    //        } else if (playListView->selectionModel()->hasSelection()) {
    //            curPlayList = playListView->selectionModel()->selectedRows().at(0).row();
    //            indexNew = model->index(curPlayList, 1);
    //            model->setData(indexNew, Qt::white, Qt::TextColorRole);
    //            model->setData(indexNew, QBrush(QColor(100, 100, 100), Qt::SolidPattern), Qt::BackgroundRole);
    //            indexNew = model->index(curPlayList, 2);
    //            model->setData(indexNew, Qt::white, Qt::TextColorRole);
    //            model->setData(indexNew, QBrush(QColor(100, 100, 100), Qt::SolidPattern), Qt::BackgroundRole);
    //        }
    //        indexNew = model->index(curPlayList, 3);
    //        QString fileName = model->data(indexNew).toString();
    //        m_player.setCurrentSource(Phonon::MediaSource(fileName));
    //    }
    //    qDebug() << m_player.state();
}

void MediaPlayer::stop()
{
    if (isActive()) {
        QModelIndex indexNew = model->index(curPlayList, 3);
        QString fileName = model->data(indexNew).toString();
        for (int i = 0; i < MAX_FILE_POS; ++i) {
            if (fileNameP[i] == fileName) {
                fileNameP[i] = "";
                filePos[i] = 0;
                break;
            }
        }
        libvlc_media_player_stop(_m_player);
    }
    playPauseAction->setChecked(false);
}

void MediaPlayer::rewind()
{
    //    QString fileName;
    //    QModelIndex index;
    //    if (curPlayList > 0){
    //        index = model->index(curPlayList, 1);
    //        model->setData(index, QColor(187, 187, 187), Qt::TextColorRole);
    //        model->setData(index, QBrush(QColor(32, 32, 32), Qt::SolidPattern), Qt::BackgroundRole);
    //        index = model->index(curPlayList, 2);
    //        model->setData(index, QColor(187, 187, 187), Qt::TextColorRole);
    //        model->setData(index, QBrush(QColor(32, 32, 32), Qt::SolidPattern), Qt::BackgroundRole);
    //        curPlayList--;
    //        index = model->index(curPlayList, 1);
    //        model->setData(index, Qt::white, Qt::TextColorRole);
    //        model->setData(index, QBrush(QColor(100, 100, 100), Qt::SolidPattern), Qt::BackgroundRole);
    //        index = model->index(curPlayList, 2);
    //        model->setData(index, Qt::white, Qt::TextColorRole);
    //        model->setData(index, QBrush(QColor(100, 100, 100), Qt::SolidPattern), Qt::BackgroundRole);

    //        index = model->index(curPlayList, 3);
    //        fileName = model->data(index).toString();
    //        m_player.setCurrentSource(Phonon::MediaSource(fileName));
    //    } else {
    //        m_player.setTickInterval(50);
    //        m_player.seek(0);
    //    }
}

void MediaPlayer::forward()
{
    ////    QList<Phonon::MediaSource> queue = m_player.queue();
    ////    if (queue.size() > 0) {
    ////        setFile(queue[0].fileName());
    ////        forwardButton->setEnabled(queue.size() > 1);
    ////        cWidget->forwardButton->setEnabled(queue.size() > 1);
    ////    }
    //    QString fileName;
    //    QModelIndex index;

    //    int row = model->rowCount();
    //    if ((curPlayList < row-1) && (curPlayList >= 0)){
    //        index = model->index(curPlayList, 1);
    //        model->setData(index, QColor(187, 187, 187), Qt::TextColorRole);
    //        model->setData(index, QBrush(QColor(32, 32, 32), Qt::SolidPattern), Qt::BackgroundRole);
    //        index = model->index(curPlayList, 2);
    //        model->setData(index, QColor(187, 187, 187), Qt::TextColorRole);
    //        model->setData(index, QBrush(QColor(32, 32, 32), Qt::SolidPattern), Qt::BackgroundRole);
    //        curPlayList++;
    //        index = model->index(curPlayList, 1);
    //        model->setData(index, Qt::white, Qt::TextColorRole);
    //        model->setData(index, QBrush(QColor(100, 100, 100), Qt::SolidPattern), Qt::BackgroundRole);
    //        index = model->index(curPlayList, 2);
    //        model->setData(index, Qt::white, Qt::TextColorRole);
    //        model->setData(index, QBrush(QColor(100, 100, 100), Qt::SolidPattern), Qt::BackgroundRole);

    //        index = model->index(curPlayList, 3);
    //        fileName = model->data(index).toString();
    //        m_player.setCurrentSource(Phonon::MediaSource(fileName));
    //    }
}

void MediaPlayer::playlistShow()
{
    if (playListDoc->isVisible()) {
        playListDoc->hide();
    } else {
        playListDoc->show();
    }
//    if (mLabel->isVisible()) {
//        QPoint pos;
//        pos.setX(geometry().x() + width() - mLabel->width() - 25);
//        if (playListDoc->isVisible()) pos.setX(pos.x() - playListDoc->width());
//        pos.setY(geometry().y() + 25);
//        mLabel->move(pos);
//    }
}

void MediaPlayer::setFullScreen(bool enabled)
{
    static bool viewPlaylist;
    if (!enabled){
        m_videoWidget->setCursor(Qt::PointingHandCursor);
    } else {
        m_videoWidget->setCursor(Qt::BlankCursor);
    }

    timerFullScreen->stop();
    sWidget.setCurrentIndex(2);
    if (!enabled){
        //        mLabel->hide();
        //        cWidget->hide();
        controlPanel->show();
        if (viewPlaylist) playListDoc->show();
        qApp->processEvents();
        fullScreenOn = true;
        setWindowState(windowState() & ~Qt::WindowFullScreen);
        fullScreenOn = false;
    } else {
        controlPanel->hide();
        viewPlaylist = playListDoc->isVisible();
        playListDoc->hide();
        qApp->processEvents();
        fullScreenOn = true;
        setWindowState(windowState() ^ Qt::WindowFullScreen);
        fullScreenOn = false;
        //        cWidget->show();
        timerFullScreen->start(3000, this);
    }
    sWidget.setCurrentIndex(0);
    setFocus();
}

void MediaPlayer::setFile(const QString &fileName)
{
    if (model->rowCount() == 0) {
        /* Create a new LibVLC media descriptor */
        _m = libvlc_media_new_path(_vlcinstance, QUrl::fromLocalFile(fileName).toEncoded());
        libvlc_media_player_set_media(_m_player, _m);
        _curPlayer = _m_player;

        //        libvlc_media_track_info_t *tracks = NULL;
        //        int num = libvlc_media_get_tracks_info(_m, &tracks);
        //        num = 0;

#if defined(Q_WS_WIN)
        libvlc_media_player_set_hwnd(_m_player, m_videoWidget->winId());
#elif defined(Q_WS_MAC)
        libvlc_media_player_set_agl(_m_player, m_videoWidget->winId());
#else // Q_WS_X11
        int windid = m_videoWidget->winId();
        libvlc_media_player_set_xwindow(_m_player, windid);
#endif // Q_WS_*

        /* Play */
        libvlc_media_player_play(_m_player);
//        _isPlaying=true;
//        sWidget.setCurrentIndex(0);
    }
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
    model->setData(index, Name, Qt::ToolTipRole);

    index = model->index(row, 1);                           // 1
    model->setData(index, QString::number(row+1) + ". " + Name, Qt::DisplayRole);
    model->setData(index, Name, Qt::ToolTipRole);
    if (row == 0) {
        model->setData(index, Qt::white, Qt::TextColorRole);
        model->setData(index, QBrush(QColor(100, 100, 100), Qt::SolidPattern), Qt::BackgroundRole);
    }

    index = model->index(row, 2);                           // 2
    model->setData(index, Name, Qt::ToolTipRole);
    if (row == 0) {
        model->setData(index, Qt::white, Qt::TextColorRole);
        model->setData(index, QBrush(QColor(100, 100, 100), Qt::SolidPattern), Qt::BackgroundRole);
    }

    index = model->index(row, 3);                           // 3
    model->setData(index, fileName);

    row = model->rowCount();
    forwardButton->setEnabled((curPlayList < row-1) && (curPlayList >= 0));
    //    cWidget->forwardButton->setEnabled((curPlayList < row-1) && (curPlayList >= 0));
}

void MediaPlayer::playing(bool play, bool buffering)
{
    static bool play_t = false;
    if ((play_t != play) && play) {
        play_t = play;
        sWidget.setCurrentIndex(1);

        QPoint posCOld = frameGeometry().center();
        //        if (m_player->hasVideo()) {
        qDebug() << "processEvents";
        qApp->processEvents();
        if (!isFullScreen()) {
            qDebug() << "processEvents (resize)";

            QSize frame;
            QSize newSize;
            frame.setWidth(frameSize().width() - size().width());
            frame.setHeight(frameSize().height() - size().height());
            newSize.setWidth(m_videoWidget->sizeHint().width() + frame.width());
            newSize.setHeight(m_videoWidget->sizeHint().height() + buttonPanelWidget->height() +
                              buttonPanelWidget->pos().y() + 0 + frame.height());
            if (playListDoc->isVisible()) newSize.setWidth(newSize.width() + playListDoc->width());

            QRect videoHintRect = QRect(QPoint(0, 0), newSize);
            QRect newVideoRect = QApplication::desktop()->availableGeometry(0).intersected(videoHintRect);

            resize(newVideoRect.width()- frame.width(), newVideoRect.height() - frame.height());

            QPoint posNew = newVideoRect.center();
            posNew = posCOld - posNew;

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
            move(posNew);

            qDebug() << posCOld.x();
            qDebug() << posCOld.y();
            posCOld = frameGeometry().center();
            qDebug() << posCOld.x();
            qDebug() << posCOld.y();
        }

        //        } else if (!isFullScreen()) {
        //            resize(minimumSize());
        //            QSize frame = frameSize();
        //            QRect hintRect = QRect(QPoint(0, 0), frame);
        //            QPoint posNew = hintRect.center();
        //            posNew = posCOld - posNew;
        //            move(posNew);
        //        }
        sWidget.setCurrentIndex(0);
    }
}

void MediaPlayer::showContextMenu(const QPoint &p)
{
    m_videoWidget->setCursor(Qt::ArrowCursor);
    fileMenu->popup(isFullScreen() ? p : mapToGlobal(p));
}
