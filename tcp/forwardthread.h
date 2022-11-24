#ifndef FORWARDTHREAD_H
#define FORWARDTHREAD_H

#include "tcpconnections.h"
#include <QObject>

class ForwardThread : public QObject
{
  Q_OBJECT
public:
  explicit ForwardThread(QObject *parent = nullptr);
  void groupChat();
  void privateChat();
  TcpConnection* sender;
  QList<QTcpSocket*> sockets;
  QByteArray bytes;

signals:

};

#endif // FORWARDTHREAD_H
