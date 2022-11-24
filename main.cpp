#include "mainwindow.h"
#include "tcp/tcpserver.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
//  MainWindow w;
//  w.show();
  TcpServer server;
  if(server.listen(QHostAddress::Any, 20000)){
      qDebug() << "Start Server";
    }
  else {
      qCritical() << server.errorString();
    }
  return a.exec();
}
