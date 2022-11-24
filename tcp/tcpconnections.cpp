#include "forwardthread.h"
#include "tcpconnections.h"
#include <QHostAddress>

TcpConnections::TcpConnections(QObject *parent)
  : QObject{parent}
{
  qDebug() << this << "Created";
}

TcpConnections::~TcpConnections()
{
  qDebug() << this << "Destroyed";
}

/*
    线程安全（其他线程可能在删除socket/添加socket）
    返回连接数量
*/
int TcpConnections::count()
{
  QReadWriteLock lock;
  lock.lockForRead();

  int value = m_connections.count();

  lock.unlock();
  return value;
}

QList<QTcpSocket *> TcpConnections::getSockets()
{
  return QList<QTcpSocket*>(m_connections.keys());
}

void TcpConnections::removeSocket(QTcpSocket *socket)
{
  if(!socket) return;
  if(!m_connections.contains(socket)) return;

  qDebug() << this << "removing socket = " << socket;
  if(socket->isOpen()){
      qDebug() << this << "socket is open, attempting to close it " << socket;
      socket->disconnect();
      socket->close();
    }
  qDebug() << this << "deleting socket " << socket;

  m_connections.remove(socket);
  socket->deleteLater();

  qDebug() << this << "client count = " << m_connections.count();
}

void TcpConnections::disconnected()
{
  if(!sender()) return;
  qDebug() << this << "Disconnecting socket " << sender();

  QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
  if(!sender()) return;
  removeSocket(socket);
}

void TcpConnections::error(QAbstractSocket::SocketError socketError)
{
  if(!sender()) return;
  qDebug() << this << "Disconnecting socket " << sender() << "error = " << socketError;

  QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
  if(!sender()) return;
  removeSocket(socket);
}

void TcpConnections::start()
{
  qDebug() << this << "connectinos started on " << QThread::currentThread();
}

void TcpConnections::quit()
{
  if(!sender()) return;
  qDebug() << this << "connections quitting " ;
  foreach (QTcpSocket* socket, m_connections.keys()) {
      qDebug() << this << "closing sockets " << socket;
      removeSocket(socket);
    }
  qDebug() << this << "finishing";
  emit finished();
}

void TcpConnections::accept(qintptr handler, TcpConnection *connection)
{
  qDebug() << "*** HEY WATCH THIS***";
  QTcpSocket* socket = new QTcpSocket(this);
  if(!socket->setSocketDescriptor(handler)){
      qWarning() << this << "无法接受连接 " << handler;
      connection->deleteLater();
      return;
    }
  qDebug() << QHostAddress(socket->peerAddress()) << ":" << socket->peerPort();

  connect(socket, &QTcpSocket::disconnected, this, &TcpConnections::disconnected);
  connect(socket, static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error), this, &TcpConnections::error);

  connection->moveToThread(QThread::currentThread());
  connection->setSocket(socket);

  m_connections.insert(socket, connection);

  qDebug() << this << "clients = " << m_connections.count();
  emit socket->connected();
}

void TcpConnections::forward(int type, TcpConnection *sender)
{
    // 转发子线程
    QThread* fthread = new QThread;
    ForwardThread *forwardTask = new ForwardThread;
    forwardTask->bytes = sender->getSocket()->readAll();
    forwardTask->sender = sender;
    forwardTask->moveToThread(fthread);
    forwardTask->sockets = getSockets();
    fthread->start();

    if(type == 0)
      {
        // 群聊转发
        if(count())
          {
            qDebug() << this << "群聊转发, 在线人数" << count();         
            forwardTask->groupChat();
          }
        else return;
      }
    else if(type == 1)
      {
        // 私聊转发
        forwardTask->privateChat();
      }

    delete forwardTask;

    qDebug() << this << "转发子线程结束";
    fthread->quit();
    fthread->wait();

    fthread->deleteLater();
}
