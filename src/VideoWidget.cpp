#include "VideoWidget.h"

VlcVideoWidget::VlcVideoWidget(MediaPlayer *player, QWidget *parent)
  : QWidget(parent),
    mediaPlayer_(player)
{
  setMouseTracking(true);
  setCursor(Qt::PointingHandCursor);
  setAcceptDrops(true);
  setContextMenuPolicy(Qt::CustomContextMenu);

  setAttribute(Qt::WA_OpaquePaintEvent);
  setAttribute(Qt::WA_PaintOnScreen);
  setAttribute(Qt::WA_NoSystemBackground);
  QPalette p = palette();
  p.setColor(backgroundRole(), Qt::black);
  setPalette(p);

  timerMouse_ = new QTimer(this);
  connect(timerMouse_, SIGNAL(timeout()), this, SLOT(hideMouse()));
}

VlcVideoWidget::~VlcVideoWidget()
{
}

//Events:
void VlcVideoWidget::mouseMoveEvent(QMouseEvent *event)
{
  event->ignore();

  setCursor(Qt::PointingHandCursor);
  if(mediaPlayer_->isFullScreen()) {
    timerMouse_->start(1000);
    emit mouseShow(event->globalPos());
  }
}
void VlcVideoWidget::mousePressEvent(QMouseEvent *event)
{
  event->ignore();

  if (event->button() == Qt::LeftButton) {
    setCursor(Qt::PointingHandCursor);
  }
  if(mediaPlayer_->isFullScreen()) {
    timerMouse_->start(1000);
    emit mouseShow(event->globalPos());
  }
}
//void VlcVideoWidget::wheelEvent(QWheelEvent *event)
//{
//    event->ignore();

//    if(event->delta() > 0)
//        emit wheel(true);
//    else
//        emit wheel(false);
//}

void VlcVideoWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
  Q_UNUSED(event)
  mediaPlayer_->fullScreenAction->setChecked(!mediaPlayer_->isFullScreen());
}

void VlcVideoWidget::dropEvent(QDropEvent *event)
{
  mediaPlayer_->handleDrop(event);
}

void VlcVideoWidget::dragEnterEvent(QDragEnterEvent *event) {
  if (event->mimeData()->hasUrls())
    event->acceptProposedAction();
}

void VlcVideoWidget::hideMouse()
{
  if(mediaPlayer_->isFullScreen()) {
    if (!mediaPlayer_->fileMenu->isVisible()) setCursor(Qt::BlankCursor);
    timerMouse_->stop();
    emit mouseHide();
  }
}
