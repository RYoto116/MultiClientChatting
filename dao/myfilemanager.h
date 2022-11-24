#ifndef MYFILEMANAGER_H
#define MYFILEMANAGER_H

#include <QObject>
#include <QDebug>
#include<fstream>
#include<iostream>
#include<string>
#include "header.h"

class MyFileManager : public QObject
{
  Q_OBJECT
public:
  explicit MyFileManager(QObject *parent = nullptr);
  std::string filepath;

//  int checkRegister(const QString& userID, const QString& passwd);
  int checkRegister(const char *account, const char *passwd);
  int checkLogIn(const char *userID, const char *passwd);
  int UserMatch(const char *account, const char *passwd);
  int RegisterAck(const char *account);
  int AddUserFile(const char *account, const char *passwd);

signals:

};

#endif // MYFILEMANAGER_H
