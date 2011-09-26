#ifndef EXPLORER_H
#define EXPLORER_H

#include <QtGui/QTableView>
#include <QtGui/QHeaderView>
#include <QtGui/QDirModel>
#include "EMP.h"

class ExplorerWidget : public QTableView
{
    Q_OBJECT
public:
    ExplorerWidget(MediaPlayer *player, QWidget *parent = 0);
    ~ExplorerWidget();

protected:
    /*virtual*/ void keyPressEvent(QKeyEvent*);

public slots:
    void slotKeyLeft();
    void slotKeyRight();
    void slotKeyUp();
    void slotKeyDown();

private slots:
    void slotSetIndex(const QModelIndex&);

private:
    MediaPlayer *m_player;
    QDirModel *model;
    QModelIndex oldIndex;

};

#endif // EXPLORER_H
