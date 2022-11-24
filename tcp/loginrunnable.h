#ifndef LOGINRUNNABLE_H
#define LOGINRUNNABLE_H

#include <QRunnable>
#include <QObject>
#include <QDebug>
#include <QThread>
#include "header.h"
#include "myfilemanager.h"
#include "tcpconnection.h"

class LoginRunnable : public QObject, public QRunnable
{
  Q_OBJECT
public:
  LoginRunnable();

  QByteArray buffer;
  TcpConnection* connection;
  char account[20];
  char passwd[20];
  MyFileManager* fileManager;

protected:
  void run();
signals:
  void result(int); // 返回登录结果
};

#endif // LOGINRUNNABLE_H
