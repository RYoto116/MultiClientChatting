#include "myfilemanager.h"

MyFileManager::MyFileManager(QObject *parent)
  : QObject{parent}
{
  filepath = std::string("usersource.txt");
}

/*
  检查账号密码是否合法
  检查账号是否存在
  参数：账号 密码
  返回值：int
*/
int MyFileManager::checkRegister(const char* account, const char* passwd)
{
  qDebug() << "账号: " << account << "\t" << "密码: " << passwd;
  qDebug() << "dao层注册检查";

  if(RegisterAck(account) == SUCCESS){
      return AddUserFile(account, passwd);
    }
  qDebug() << this << "用户已存在，拒绝注册";
  return FAIL;
}

int MyFileManager::checkLogIn(const char *account, const char *passwd)
{
  qDebug() << this << "dao层登录检查";
  qDebug() << "账号: " << account << " " << "密码: " << passwd;
  return UserMatch(account, passwd);
}

int MyFileManager::UserMatch(const char* account, const char* passwd) {            //查找账号密码是否匹配
  std::string temp1, temp2;
  std::ifstream file(filepath, std::ios::in);
  while (getline (file, temp1, ' ')) {
      getline(file, temp2);
      if (!strcmp(temp1.c_str(), account)) {
          if (!strcmp(temp2.c_str(), passwd))
            {
              file.close();
              qDebug() << this << "账号密码匹配";
              return SUCCESS;             //真，账号密码匹配
            }
          else {
              qDebug() << "用户输入: " << account << " " << passwd;
              qDebug() << strlen(account) << " " << strlen(passwd);
              qDebug() << "数据记录: " << temp1.c_str() << " " << temp2.c_str();
              qDebug() << temp1.size() << " " << temp2.size();
              qDebug() << "比较结果: " << strcmp(temp1.c_str(), account) << " " << strcmp(temp2.c_str(), passwd);
              qDebug() << this << "账号密码不匹配";
              file.close();
              return FAIL;                //假，不匹配
            }
        }
    }
  file.close();
  qDebug() << "账号不存在";
  return FAIL;                                   //假，账号不存在
}

int MyFileManager::RegisterAck(const char* account) {              //判断账号是否被注册
  qDebug() << this << "查询用户注册记录...";
  std::string temp1,temp2;
  std::ifstream file(filepath, std::ios::in);//文件地址双斜杠
  while (getline(file, temp1,' ')) {             //读入账号
      getline(file, temp2);                      //读出密码，使得下次循环直接读取账号
      if (!strcmp(account, temp1.c_str())){
          qDebug() << this << "用户账号重复，不允许注册";
          file.close();
          return FAIL;
        }                    //真，账号被注册
    }
  file.close();
  qDebug() << this << "新账号，允许注册";
  return SUCCESS;                                 //假，账号没有被注册
}

int MyFileManager::AddUserFile(const char* account, const char* passwd) {  /*向文件中写入账号密码*/
  qDebug() << this << "将用户加入数据库....";
  std::ofstream file(filepath, std::ios::app);
  if (file.is_open()) {
      file << account<<' '<< passwd << "\n";
      file.close();
      return SUCCESS;
    }
  return FAIL;
}
