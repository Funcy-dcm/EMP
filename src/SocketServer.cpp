
#include <QtNetwork>
#include <QMessageBox>
#include <QDebug>
#include "SocketServer.h"

SocketServer::SocketServer(MediaPlayer *player, QObject *parent) :
    QTcpServer(parent),
    m_player(player)
{
    m_nNextBlockSize = 0;
    if (!listen(QHostAddress::Any, 13551)) {
        QMessageBox::critical(0,
                              "Server Error",
                              "Unable to start the server:"
                              + errorString()
                             );
        close();
    } else {
        connect(this, SIGNAL(newConnection()),
                this,         SLOT(slotNewConnection())
               );
        qDebug() << "Connection";
    }
}

/*virtual*/ void SocketServer::slotNewConnection()
{
    qDebug() << "NewConnection";
    QTcpSocket* pClientSocket = nextPendingConnection();
    connect(pClientSocket, SIGNAL(disconnected()),
            pClientSocket, SLOT(deleteLater())
           );
    connect(pClientSocket, SIGNAL(readyRead()),
            this,          SLOT(slotReadClient())
           );

    sendToClient(pClientSocket, "*OK", NULL);
}

// ----------------------------------------------------------------------
void SocketServer::slotReadClient()
{
    QString str;
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    QDataStream in(pClientSocket);
    for (;;) {
        if (!m_nNextBlockSize) {
            if (pClientSocket->bytesAvailable() < sizeof(quint16)) {
                break;
            }
            in >> m_nNextBlockSize;
        }

        if (pClientSocket->bytesAvailable() < m_nNextBlockSize) {
            break;
        }

        QString cmd;
        char *str1;
        in >> str1;
        cmd.sprintf("%s", str1);

        m_nNextBlockSize = 0;

        if (cmd == "Name") {
            str = m_player->getCurrentSourceName();
        }

        sendToClient(pClientSocket, cmd, str);
    }
}

// ----------------------------------------------------------------------
void SocketServer::sendToClient(QTcpSocket* pSocket, const QString& cmd, const QString& str)
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out << quint16(0);
    out << cmd.toAscii();
    if (str.size() > 0) {
        QString fileName = str;
        fileName = fileName.right(fileName.length() - fileName.lastIndexOf('/') - 1);
        fileName = fileName.left(fileName.lastIndexOf('.'));
        out << fileName;
        out << m_player->getCurrentTime();
        out << m_player->getTotalTime();
    }

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    pSocket->write(arrBlock);
}
