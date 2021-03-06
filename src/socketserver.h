#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#include <QtCore>
#include <QTcpServer>
#include "emp.h"

class MediaPlayer;

class SocketServer : public QTcpServer
{
  Q_OBJECT
public:
  SocketServer(MediaPlayer *player, QObject *parent = 0);
public slots:
  virtual void slotNewConnection();
  void slotReadClient();
private slots:
  void sendToClient(QTcpSocket* pSocket, const QString& cmd, const QString& str);
private:
  MediaPlayer *mediaPlayer_;
  quint16     nextBlockSize_;

};

#endif // SOCKETSERVER_H
