#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QHostAddress>
#include <QObject>
#include <QTcpServer>
#include <QThreadPool>
#include "tcpconnections.h"

class TcpServer : public QTcpServer
{
  Q_OBJECT
public:
  explicit TcpServer(QObject *parent = nullptr);
  ~TcpServer();

  virtual bool listen(const QHostAddress &address, qint16 port);
  virtual void close();
  virtual qint16 port();

protected:
  QThread* m_thread;
  QThreadPool* pool;
  TcpConnections *m_connections;

  virtual void incomingConnection(qintptr descriptor);
  virtual void accept(qintptr descriptor, TcpConnection* connection);

signals:
  void accepting(qintptr handler, TcpConnection* connection);
  void finished();
  void forward(int type, TcpConnection* sender);

public slots:
  void complete();
};

#endif // TCPSERVER_H
