#include "osdwidget.h"

OSDWidget::OSDWidget(MediaPlayer *player, QWidget *p) :
    QWidget(p,  Qt::Tool | Qt::FramelessWindowHint),
    m_player(player)
{
//    setWindowModality(Qt::WindowModal);
    setAttribute(Qt::WA_TranslucentBackground);
    setFocusPolicy(Qt::NoFocus);

//    setWindowOpacity(0.3);

    mLabel = new QLabel(this);
    mLabel->setObjectName("mLabel");
    mLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    mLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    mLabel->setMinimumWidth(100);

    QAction *showLocalTime = new QAction(this);
    showLocalTime->setShortcut(QKeySequence( Qt::Key_T ));
    showLocalTime->setShortcutContext(Qt::ApplicationShortcut);
    connect(showLocalTime, SIGNAL(triggered()), this, SLOT(slotShowLocalTime()));
    m_player->addAction(showLocalTime);

    QAction *showEndTime = new QAction(this);
    showEndTime->setShortcut(QKeySequence( Qt::Key_E ));
    showEndTime->setShortcutContext(Qt::ApplicationShortcut);
    connect(showEndTime, SIGNAL(triggered()), this, SLOT(slotShowEndTime()));
    m_player->addAction(showEndTime);

    QVBoxLayout* pvbxLayout = new QVBoxLayout(this);
    pvbxLayout->setContentsMargins(0, 0, 0, 0);
    pvbxLayout->addWidget(mLabel);
    setLayout(pvbxLayout);

    mLabel->setText("EMP");
}

void OSDWidget::showWidget(QString str)
{
    hide();
//    mLabel->setText("<CENTER><img src=\":/Res/Pause1.png\"><CENTER>"
//                    "<CENTER>" + str);
    mLabel->setText(str);

    QPoint pos;
    show();
    pos.setX(m_player->geometry().x() + m_player->width() - width() - 25);
    if (m_player->playListDoc->isVisible()) pos.setX(pos.x() - m_player->playListDoc->width());
    pos.setY(m_player->geometry().y() + 25);
    move(pos);

    timerShowWidget.start(5000, this);
}

void OSDWidget::timerEvent(QTimerEvent *pe)
{
    if (pe->timerId() == timerShowWidget.timerId()) {
        timerShowWidget.stop();
        hide();
    }
}

void OSDWidget::slotShowLocalTime()
{
    QString str = QDateTime::currentDateTime().toString("h:mm");
    showWidget(str);
}

void OSDWidget::slotShowEndTime()
{
    long len = m_player->getTotalTime();
    long pos = m_player->getCurrentTime();
    QString timeString = " ";
    if (pos || len)
    {
        long nPos = len - pos;
        int sec = nPos/1000;
        int min = sec/60;
        int hour = min/60;
        int msec = nPos;
        QTime nPlayTime(hour%60, min%60, sec%60, msec%1000);
        QString timeFormat = "m:ss";
        if (hour > 0)
            timeFormat = "h:mm:ss";
        if (len) timeString = nPlayTime.toString(timeFormat);
    }
    showWidget(timeString);
}
