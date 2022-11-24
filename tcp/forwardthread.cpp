#include "forwardthread.h"

ForwardThread::ForwardThread(QObject *parent)
  : QObject{parent}
{
}

void ForwardThread::groupChat()
{
  /**
   * Struct Message
    {
      qint32 header;->已读
      qint32 id;
      qint32 channel;
      char txt[100];
    };
  */
  int id, channel;

  char* ptr = bytes.data();
  memcpy((char*)&id, ptr, 4);
  memcpy((char*)&channel, ptr+4, 4);

  qDebug() << this << "转发子线程" << QThread::currentThread();

  MessagePackage pack(GROUP, id, channel);
  memcpy(pack.account, sender->account, 20);
  memcpy(pack.message, ptr+8, 100);

  qDebug() << this << "id: " << pack.id;
  qDebug() << this << "channel: " << pack.channel;
  qDebug() << this << "账号:" << pack.account;
  qDebug() << "信息: " << pack.message;

  QByteArray myByteArray;
  myByteArray.append(reinterpret_cast<char*>(&pack), sizeof (pack));

  for (auto& socket: sockets) {
      if(sender->getState() != ONLINE) continue;
      socket->write(myByteArray.data(), sizeof(pack));
      socket->flush();
    }
  qDebug() << this << "群聊转发子任务结束";
}

void ForwardThread::privateChat()
{
  qDebug() << this << "私聊转发子任务开始";
  qDebug() << this << "私聊转发子任务结束";
}


