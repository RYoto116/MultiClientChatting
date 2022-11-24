#ifndef TCPCONNECTIONS_H
#define TCPCONNECTIONS_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QTcpSocket>
#include <QMap>
#include <QReadWriteLock>
#include <QThreadPool>
#include <QQueue>
#include "tcpconnection.h"
#include "header.h"

class TcpConnections : public QObject
{
  Q_OBJECT
public:
  explicit TcpConnections(QObject *parent = nullptr);
  ~TcpConnections();
  virtual int count();
  QList<QTcpSocket*> getSockets();

  QQueue<TcpConnection*> events; // 活跃事件队列

protected:
  QMap<QTcpSocket*, TcpConnection*> m_connections;
//  QMap<int, TcpConnection*> m_groups;
  void removeSocket(QTcpSocket *socket);
signals:
  void quitting();
  void finished();
protected slots:
  void disconnected();
  void error(QAbstractSocket::SocketError socketError);
public slots:
  void start();
  void quit();
  void accept(qintptr handler, TcpConnection *conneciton);
  void forward(int type, TcpConnection* sender);
};

#endif // TCPCONNECTIONS_H
