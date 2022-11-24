#ifndef HEADER_H
#define HEADER_H

#include "qglobal.h"
const int LOGIN    = 0b010001; // 17
const int REGISTER = 0b010010; // 18
const int EXIT_1   = 0b010000; // 16 退出登录注册
const int SELGROUP = 0b100001; // 33
const int EXIT_2   = 0b100000; // 32 退出群组
const int GROUP    = 0b100010; // 34
const int SUCCESS  = 0b000001; // 1
const int FAIL     = 0b000000; // 0
const int INVALID  = 0b000010; // 2
//const int PRIVATE = 11;

const int OFFLINE = 0;
const int ONLINE = 1;

#pragma pack(1)
//(对客户端登录注册需求的响应，对应于DataPackage）
struct InitPackage
{
    qint32 header;
    qint32 sig; // 0：标识成功，1标识失败
    qint32 id;
    InitPackage(int _header, int _sig, int _id):header(_header), sig(_sig), id(_id){}
};

// (对客户端群聊选择需求响应时，生成对应的群聊数据包）
struct GroupPackage
{
    qint32 header;
    qint32 channel;
    qint32 id;
    GroupPackage(int _header, int _channel, int _id):header(_header), channel(_channel), id(_id){}
};

//(对客户端转发消息时使用）
struct MessagePackage
{
    qint32 header;
    qint32 id;
    qint32 channel;
    char account[20];
    char message[100];
    MessagePackage(qint32 _header, qint32 _id, qint32 _channel): header(_header), id(_id), channel(_channel){
      memset(account, 0, 20);
      memset(message, 0, 100);
    }
};
#pragma pack()

#pragma pack(1)
//(登录注册时发送）
struct DataPackage
{
    qint32 header;
    char account[20];
    char passwd[20];
    DataPackage(){
      memset(account, 0, 20);
      memset(passwd, 0, 20);
    }
};
//（群聊选择时发送）
  struct GroupChatData
  {
      qint32 header;
      qint32 ApplyForChat;
      qint32 id;
  };

//  (退出群聊时发送）
  struct ExitChatData
  {
      qint32 header;
      qint32 channel;
      qint32 id;
  };

//  (退出登录时发送）
  struct LogOutData
  {
      qint32 header;
      qint32 id;
  };
//  (对客户端转发消息时使用）
  struct Message
  {
      qint32 header;
      qint32 id;
      qint32 channel;
      char txt[100];
      Message(){
        memset(txt, 0, 100);
      }
  };
#pragma pack()


#endif // HEADER_H
