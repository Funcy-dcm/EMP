#ifndef _VIDEOWIDGET_H_
#define _VIDEOWIDGET_H_

#include <QtGui>
#include "emp.h"

class VlcVideoWidget : public QWidget
{
  Q_OBJECT
public:
  VlcVideoWidget(MediaPlayer *player, QWidget *parent);
  ~VlcVideoWidget();
  WId widgetId() { return this->winId(); }

protected:
  void mouseMoveEvent(QMouseEvent *event);
  void mousePressEvent(QMouseEvent *event);
  //    void wheelEvent(QWheelEvent *event);
  void mouseDoubleClickEvent(QMouseEvent *event);
  void dropEvent(QDropEvent *event);
  void dragEnterEvent(QDragEnterEvent *event);

signals:
  //    void wheel(const bool);
  void mouseShow(const QPoint);
  void mouseHide();

public slots:
  //    void enableMouseHide() { _hide = true; }
  //    void disableMouseHide() { _hide = false; }

private slots:
  void hideMouse();
  void showMouse();

private:
  MediaPlayer *mediaPlayer_;
  QTimer *timerMouse_;

};

#endif // _VIDEOWIDGET_H_
