#include "registerrunnable.h"

RegisterRunnable::RegisterRunnable()
{
  fileManager = new MyFileManager(this);
}

void RegisterRunnable::run()
{
  qDebug() << this << "启动注册子线程" << QThread::currentThread();
  char* ptr = buffer.data();
  int res = FAIL;
  if (!buffer.isEmpty()) {
      memcpy(account, ptr, 20);
      memcpy(passwd, ptr+20, 20);
      qDebug() << this << "read from client......" << buffer.size();
      qDebug() << this << "注册账号:" << account << "\t" << sizeof(account);
      qDebug() << this << "注册密码:" << passwd << "\t" << sizeof(passwd);
      res = fileManager->checkRegister(account, passwd);
    }
  qDebug() << this << "注册结果: " << res;
  qDebug() << this << "注册任务结束，退出子线程" << QThread::currentThread();
  emit result(res);
}


