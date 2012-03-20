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
  void onFullScreen(bool);

private slots:
  void slotSetIndex(const QModelIndex&);
  void setFilters();

private:
  MediaPlayer *m_player;
  QDirModel *model;
  QModelIndex oldIndex;
  QStringList strFilters;

};

#endif // EXPLORER_H
