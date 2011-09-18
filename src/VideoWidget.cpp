#include <QtGui>
#include "VideoWidget.h"

VlcVideoWidget::VlcVideoWidget(QWidget *parent)
    : QWidget(parent),
      _hide(true),
      _currentRatio(""),
      _currentCrop(""),
      _currentFilter("")
{
//    setMouseTracking(true);
    setCursor(Qt::PointingHandCursor);
    _widget = new QWidget(this);
    _widget->setCursor(Qt::PointingHandCursor);
//    _widget->setMouseTracking(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->addWidget(_widget);
    setLayout(layout);

    _timerMouse = new QTimer(this);
    connect(_timerMouse, SIGNAL(timeout()), this, SLOT(hideMouse()));
    _timerSettings = new QTimer(this);
}

VlcVideoWidget::~VlcVideoWidget()
{
    delete _timerMouse;
    delete _timerSettings;
    delete _widget;
}

//Events:
void VlcVideoWidget::mouseMoveEvent(QMouseEvent *event)
{
    event->ignore();
    if(isFullScreen()) {
        emit mouseShow(event->globalPos());
    }

    if(isFullScreen() && _hide) {
        qApp->setOverrideCursor(Qt::ArrowCursor);

        _timerMouse->start(1000);
    }
}
void VlcVideoWidget::mousePressEvent(QMouseEvent *event)
{
    event->ignore();

    if(event->button() == Qt::RightButton) {
        qApp->setOverrideCursor(Qt::ArrowCursor);
        emit rightClick(event->globalPos());
    }
}
void VlcVideoWidget::wheelEvent(QWheelEvent *event)
{
    event->ignore();

    if(event->delta() > 0)
        emit wheel(true);
    else
        emit wheel(false);
}

void VlcVideoWidget::hideMouse()
{
    if(isFullScreen() && _hide) {
        qApp->setOverrideCursor(Qt::BlankCursor);
        _timerMouse->stop();
        emit mouseHide();
    }
}