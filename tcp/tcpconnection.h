#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QMutex>
#include <QThreadPool>
#include "header.h"

class TcpConnection : public QObject
{
  Q_OBJECT

public:
  static int globalID;
  char account[20];
  char passwd[20];

  explicit TcpConnection(QObject *parent = nullptr);
  ~TcpConnection();
  virtual void setSocket(QTcpSocket* socket);
  QTcpSocket* getSocket();
  void registering();
  void loggingIn();
  int getState();

  int requestType;
  int clientID; // 注册后返回给客户端
  int channel; // 用户加入的频道，后面改为容器
  int state;

protected:
  QTcpSocket* m_socket;

signals:
  void selGroup(int channel);
  void exitGroup(int channel);
  void textMessage(int type, TcpConnection *connection);

public slots:
  virtual void connected();
  virtual void disconnected();
  virtual void readyRead();
  virtual void bytesWritten(qint64 bytes);
  virtual void stateChanged(QAbstractSocket::SocketState socketState);
  virtual void error(QAbstractSocket::SocketError socketError);

  void changeState(int res);
  void initPackageResponse(int res); // 改变在线状态并响应
};

#endif // TCPCONNECTION_H
