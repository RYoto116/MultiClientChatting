#include "loginrunnable.h"
#include <regex>

LoginRunnable::LoginRunnable()
{
  fileManager = new MyFileManager(this);
}

void LoginRunnable::run()
{
  /*
    struct DataPackage  (登录注册时发送）
    {
        qint32 header;
        char account[20];
        char passwd[20];
    };
  */
  qDebug() << this << "启动登录子线程" << QThread::currentThread();
  char* ptr = buffer.data();
  int res = FAIL;
  if (!buffer.isEmpty()) {
      memcpy(account, ptr, 20);
      memcpy(passwd, ptr+20, 20);
      qDebug() << this << "read from client......" << buffer.size();
      qDebug() << this << "登录账号:" << account;
      qDebug() << this << "登录密码:" << passwd;
      memcpy(connection->account, account, strlen(account));
      memcpy(connection->passwd, passwd, strlen(passwd));
      res =  fileManager->checkLogIn(connection->account, connection->passwd);
    }

  qDebug() << this << "登录结果: " << res;
  qDebug() << this << "登录任务结束，退出子线程" << QThread::currentThread();
  emit result(res);
}
