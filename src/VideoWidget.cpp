#include "VideoWidget.h"

VlcVideoWidget::VlcVideoWidget(MediaPlayer *player, QWidget *parent)
  : QWidget(parent),
    media_player_(player)
{
  setMouseTracking(true);
  setCursor(Qt::PointingHandCursor);
  setAcceptDrops(true);
  setContextMenuPolicy(Qt::CustomContextMenu);

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
  if(media_player_->isFullScreen()) {
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
  if(media_player_->isFullScreen()) {
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
  media_player_->fullScreenAction->setChecked(!media_player_->isFullScreen());
}

void VlcVideoWidget::dropEvent(QDropEvent *event)
{
  media_player_->handleDrop(event);
}

void VlcVideoWidget::dragEnterEvent(QDragEnterEvent *event) {
  if (event->mimeData()->hasUrls())
    event->acceptProposedAction();
}

void VlcVideoWidget::hideMouse()
{
  if(media_player_->isFullScreen()) {
    if (!media_player_->fileMenu->isVisible()) setCursor(Qt::BlankCursor);
    timerMouse_->stop();
    emit mouseHide();
  }
}
