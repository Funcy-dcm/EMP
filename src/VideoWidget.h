#ifndef _VIDEOWIDGET_H_
#define _VIDEOWIDGET_H_

#include <QtCore/QTimer>
#include <QtGui/QWidget>

class VlcVideoWidget : public QWidget
{
    Q_OBJECT
public:
    VlcVideoWidget(QWidget *parent = 0);
    ~VlcVideoWidget();
    WId widgetId() { return _widget->winId(); }

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

signals:
    void rightClick(const QPoint);
    void wheel(const bool);
    void mouseShow(const QPoint);
    void mouseHide();

public slots:
    void enableMouseHide() { _hide = true; }
    void disableMouseHide() { _hide = false; }

private slots:
    void hideMouse();

private:
    QWidget *_widget;
    QTimer *_timerMouse;
    QTimer *_timerSettings;

    bool _hide;

    QString _currentRatio;
    QString _currentCrop;
    QString _currentFilter;
};

#endif // _VIDEOWIDGET_H_
