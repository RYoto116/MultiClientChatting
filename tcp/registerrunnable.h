#ifndef REGISTERRUNNABLE_H
#define REGISTERRUNNABLE_H

#include <QRunnable>
#include <QObject>
#include <QDebug>
#include <QThread>
#include "header.h"
#include "myfilemanager.h"
#include "tcpconnection.h"

class RegisterRunnable :public QObject, public QRunnable
{
  Q_OBJECT
public:
  RegisterRunnable();

  QByteArray buffer;
  TcpConnection* connection;
  char account[20];
  char passwd[20];
  MyFileManager* fileManager;

protected:
  void run();
signals:
  void result(int); // 返回注册结果
};

#endif // REGISTERRUNNABLE_H
