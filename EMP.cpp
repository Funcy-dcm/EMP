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
/*
class MLabel : public QLabel
{
public:
    MLabel(QWidget *p = 0) : QLabel(p)
    {

        //Transparency
        setWindowOpacity(0.8);

    }
}
*/
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

/*
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);
    tLabel = new QLabel(this);
    tLabel->setAttribute(Qt::WA_NoSystemBackground);
    tLabel->setAttribute(Qt::WA_TranslucentBackground);
//    tLabel->setBackgroundRole(QPalette::NoRole);
//    tLabel->setAlignment(Qt::AlignHCenter);
    tLabel->setText("Text111111111111111111111111111111111111111111");
    tLabel->setObjectName("tLabel");

//    tLabel->setFixedHeight(20);

    tLabel->setMinimumWidth(100);

    tLabel->setWindowOpacity(0.5);
//    setWindowOpacity(0.5);


//    tLabel->setGeometry(0,0,width(), height());
    tLabel->setSizePolicy( QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
    tLabel->resize(width(), 20);
//    tLabel->move(50, 50);

//    QLabel *nLabel = new QLabel(this);
//    QPixmap pixmap(nLabel->size());
//    pixmap.fill(Qt::transparent);
//    nLabel->render(&pixmap,QPoint(),QRegion(), DrawChildren);
//    tLabel->setMask(pixmap.mask());
*/

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
        if (e->key() == Qt::Key_Escape) {
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
    /*case QEvent::Close:
        //we just ignore the cose events on the video widget
        //this prevents ALT+F4 from having an effect in fullscreen mode
        e->ignore();
        return true;*/
    case QEvent::MouseMove:
        setCursor(Qt::PointingHandCursor);
//        m_player->buttonPanelWidget->setWindowModality(Qt::ApplicationModal);
        //fall through
    case QEvent::WindowStateChange:
        {
            //we just update the state of the checkbox, in case it wasn't already
            m_action.setChecked(windowState() & Qt::WindowFullScreen);
//            const Qt::WindowFlags flags = m_player->windowFlags();
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

    setContextMenuPolicy(Qt::CustomContextMenu);
    m_videoWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    QWidget *wgt = new QWidget;

    buttonPanelWidget = new QWidget(this);
    openButton    = new QPushButton(wgt);
    playButton    = new QPushButton(wgt);
    stopButton    = new QPushButton(wgt);
    rewindButton  = new QPushButton(wgt);
    forwardButton = new QPushButton(wgt);
    playlistButton = new QPushButton(wgt);
    nameLabel  = new QLabel(wgt);
    timeLabel  = new QLabel(wgt);
    slider = new Phonon::SeekSlider(wgt);
    volume = new Phonon::VolumeSlider(wgt);

    playListDoc = new QDockWidget("PLAYLIST", this);
    playListDoc->setObjectName("playListDoc");
    playListDoc->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    model = new QStandardItemModel(0, 4);
    playListView = new QTableView;

    slider->setMediaObject(&m_pmedia);
    slider->setCursor(Qt::PointingHandCursor);

    volume->setAudioOutput(&m_AudioOutput);
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

    timeLabel->setAlignment(Qt::AlignRight);
    nameLabel->setAlignment(Qt::AlignLeft);


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
//    playListView->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    playListView->verticalHeader()->setDefaultSectionSize ( playListView->verticalHeader()->minimumSectionSize () );
//    playListView->setWordWrap(false);

    playListView->setFocusPolicy(Qt::NoFocus);
    playListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    playListView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    playListDoc->setWidget(playListView);
    playListDoc->setMinimumWidth(160);
    addDockWidget(Qt::RightDockWidgetArea, playListDoc);

    //Layout setup
    QHBoxLayout* phbxLayout = new QHBoxLayout;
    phbxLayout->addWidget(openButton);
    phbxLayout->addSpacing(10);
    phbxLayout->addWidget(playButton);
    phbxLayout->addWidget(stopButton);
    phbxLayout->addWidget(rewindButton);
    phbxLayout->addWidget(forwardButton);
//    phbxLayout->addSpacing(10);
    phbxLayout->addStretch();
    phbxLayout->addWidget(volume);
    phbxLayout->addSpacing(10);
    phbxLayout->addWidget(playlistButton);

    QHBoxLayout* phbxLayout2 = new QHBoxLayout;
    phbxLayout2->addWidget(slider);

    QHBoxLayout* phbxLayout3 = new QHBoxLayout;
    phbxLayout3->addWidget(nameLabel);
    phbxLayout3->addSpacing(20);
    phbxLayout3->addWidget(timeLabel);

    QVBoxLayout* pvbxLayout = new QVBoxLayout(wgt);
    pvbxLayout->setMargin(0); pvbxLayout->setSpacing(0);

    initVideoWindow();
    pvbxLayout->addWidget(&m_videoWindow);
    m_videoWindow.hide();

    QVBoxLayout *buttonPanelLayout = new QVBoxLayout();
    buttonPanelLayout->setMargin(5);
    buttonPanelLayout->setSpacing(5);
    buttonPanelLayout->addLayout(phbxLayout2);
    buttonPanelLayout->addLayout(phbxLayout3);
    buttonPanelLayout->addLayout(phbxLayout);

    buttonPanelWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    buttonPanelWidget->setLayout(buttonPanelLayout);
//    pvbxLayout->addWidget(buttonPanelWidget);
    QHBoxLayout *labelLayout = new QHBoxLayout();
    pvbxLayout->addLayout(labelLayout);

    wgt->setLayout(pvbxLayout);

    QStatusBar *controlPanel = new QStatusBar;
    controlPanel->addPermanentWidget(buttonPanelWidget, 1);
    controlPanel->setContentsMargins(0, 0, 0, 0);
    controlPanel->setWindowFlags(Qt::FramelessWindowHint);
    controlPanel->setSizeGripEnabled(false);

    connect(openButton, SIGNAL(clicked()), this, SLOT(slotOpen()));
    connect(playButton, SIGNAL(clicked()), this, SLOT(playPause()));
    connect(stopButton, SIGNAL(clicked()), this, SLOT(stop()));
    connect(rewindButton, SIGNAL(clicked()), this, SLOT(rewind()));
    connect(forwardButton, SIGNAL(clicked()), this, SLOT(forward()));
    connect(playlistButton, SIGNAL(clicked()), this, SLOT(playlistShow()));

    connect(m_videoWidget, SIGNAL(customContextMenuRequested(const QPoint &)), SLOT(showContextMenu(const QPoint &)));
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), SLOT(showContextMenu(const QPoint &)));
    connect(&m_pmedia, SIGNAL(metaDataChanged()), this, SLOT(updateInfo()));
    connect(&m_pmedia, SIGNAL(totalTimeChanged(qint64)), this, SLOT(updateTime()));
    connect(&m_pmedia, SIGNAL(tick(qint64)), this, SLOT(updateTime()));
    connect(&m_pmedia, SIGNAL(stateChanged(Phonon::State, Phonon::State)), this, SLOT(stateChanged(Phonon::State, Phonon::State)));
//    connect(&m_pmedia, SIGNAL(bufferStatus(int)), this, SLOT(bufferStatus(int)));
    connect(&m_AudioOutput, SIGNAL(volumeChanged(qreal)), this, SLOT(volumeChanged(qreal)));
    connect(&m_pmedia, SIGNAL(hasVideoChanged(bool)), this, SLOT(hasVideoChanged(bool)));

    m_pmedia.setTickInterval(250);

    playButton->setEnabled(false);
    rewindButton->setEnabled(false);
    forwardButton->setEnabled(false);
    setAcceptDrops(true);

    m_audioOutputPath = Phonon::createPath(&m_pmedia, &m_AudioOutput);
    Phonon::createPath(&m_pmedia, m_videoWidget);

    setCentralWidget(wgt);
    setStatusBar(controlPanel);

    readSettings();

    if (!filePath.isEmpty()){
        setFile(filePath);
        addFile(filePath);
    }
    resize(minimumSizeHint());

    moveWindowToCenter();

    qApp->installEventFilter( this );

//    m_videoWindow.setWindowOpacity(0.5);
//    m_videoWindow.setAttribute(Qt::WA_TranslucentBackground);
//    setAttribute(Qt::WA_NoSystemBackground);
//    setAttribute(Qt::WA_TranslucentBackground);
//    buttonPanelWidget->setAttribute(Qt::WA_TranslucentBackground);
//    buttonPanelWidget->setWindowFlags(Qt::FramelessWindowHint);
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

/*virtual*/ void MediaPlayer::showEvent(QShowEvent*)
{

}

/*virtual*/ void MediaPlayer::keyPressEvent(QKeyEvent *pe)
{
    /*if (!pe->modifiers()) {
        if (pe->key() == Qt::Key_Space) {
            playPause();
            pe->accept();
            return;
        } else if (pe->key() == Qt::Key_F11 && m_pmedia.hasVideo()) {
            m_videoWidget->setFullScreen(!isFullScreen());
//            buttonPanelWidget->setVisible(true);
            pe->accept();
            return;
        } else if (pe->key() == Qt::Key_Left) {
            if (m_pmedia.isSeekable()) {
                long pos = m_pmedia.currentTime() - 15000;
                if (pos) m_pmedia.seek(pos);
                else m_pmedia.seek(0);
            }
            pe->accept();
            return;
        } else if (pe->key() == Qt::Key_Right) {
            if (m_pmedia.isSeekable()) {
                long pos = m_pmedia.currentTime() + 15000;
                m_pmedia.seek(pos);
            }
            pe->accept();
            return;
        }
    }
    QWidget::keyPressEvent(pe);*/
}

/*virtual*/ bool MediaPlayer::eventFilter(QObject* pobj, QEvent* pe)
{
    if (pe->type() == QEvent::Close) {
        if (pobj == m_videoWidget) {
            close();
            return false;
        }
    }

    if (pe->type() == QEvent::ToolTip) {
        if (pobj == volume) {
            QString vol = tr("Volume") + " [" + QString::number(qreal(m_AudioOutput.volume()*100.0f)) + "%]";
            volume->setToolTip(vol);
        }
    }

    if (pe->type() == QEvent::KeyPress) {
        if (!((QKeyEvent*)pe)->modifiers()) {
            if (((QKeyEvent*)pe)->key() == Qt::Key_Space) {
                playPause();
                return true;
            } else if (((QKeyEvent*)pe)->key() == Qt::Key_F11 && m_pmedia.hasVideo()) {
                m_videoWidget->setFullScreen(!isFullScreen());
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
    nameLabel->setText(vol);
    m_timer.start(5000, this);
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
    if (fileNames.size() > 0) {
        m_pmedia.clearQueue();
        model->clear();
        QString fileName = fileNames[0];
        setFile(fileName);
        addFile(fileName);
        for (int i=1; i<fileNames.size(); i++)
          addFile(fileNames[i]);
//        for (int i=1; i<fileNames.size(); i++)
//            m_pmedia.enqueue(Phonon::MediaSource(fileNames[i]));
    }
}

void MediaPlayer::handleDrop(QDropEvent *e)
{
    QList<QUrl> urls = e->mimeData()->urls();
    if (e->proposedAction() == Qt::MoveAction){
        // Добавляем в очередь
        for (int i=0; i<urls.size(); i++)
            m_pmedia.enqueue(Phonon::MediaSource(urls[i].toLocalFile()));
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
/*                    for (int i=1; i< entries.size(); ++i)
                        m_pmedia.enqueue(fileName + QDir::separator() + entries[i])*/;
                }
            } else {
                setFile(fileName);
                addFile(fileName);
                for (int i=1; i<urls.size(); i++)
                  addFile(urls[i].toLocalFile());
//                for (int i=1; i<urls.size(); i++)
//                    m_pmedia.enqueue(Phonon::MediaSource(urls[i].toLocalFile()));
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
    Q_UNUSED(oldstate);

    if (oldstate == Phonon::LoadingState) {
//        m_videoWindow.setVisible(m_pmedia.hasVideo());
        if (m_pmedia.hasVideo()){
            // Flush event que so that sizeHint takes the
            // recently shown/hidden m_videoWindow into account:
            qApp->processEvents();
            QPoint posCOld = geometry().center();
            QSize frame;
            QSize newSize;
            frame.setWidth(frameSize().width() - size().width());
            frame.setHeight(frameSize().height() - size().height());
            newSize.setWidth(m_videoWindow.sizeHint().width() + frame.width());
            newSize.setHeight(m_videoWindow.sizeHint().height() + buttonPanelWidget->height() +
                    buttonPanelWidget->pos().y() + 0 + frame.height());
            if (playListDoc->isVisible()) newSize.setWidth(newSize.width() + playListDoc->width());

            QRect videoHintRect = QRect(QPoint(0, 0), newSize);
            QRect newVideoRect = QApplication::desktop()->screenGeometry().intersected(videoHintRect);

            QPoint posNew;
            posNew.setX(posCOld.x() - newVideoRect.width()/2);
            posNew.setY(posCOld.y() - newVideoRect.height()/2);
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

            setGeometry(posNew.x(), posNew.y(),
                        newVideoRect.width()- frame.width(),
                        newVideoRect.height() - frame.height());
            move(posNew);

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
//        moveWindowToCenter();
        m_pmedia.play();
        //
    }

    switch (newstate) {
        case Phonon::ErrorState:
            QMessageBox::warning(this, "EMP", m_pmedia.errorString(), QMessageBox::Close);
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
//            if (m_pmedia.hasVideo())
//                m_videoWindow.show();
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
