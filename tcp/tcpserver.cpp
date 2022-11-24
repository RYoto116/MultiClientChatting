#include "tcpserver.h"
#include "forwardthread.h"

TcpServer::TcpServer(QObject *parent)
  : QTcpServer{parent}
{
  qDebug() << this << "connectinos started on " << QThread::currentThread();
  pool = new QThreadPool(this);
  pool->setMaxThreadCount(15);
}

TcpServer::~TcpServer()
{

}

bool TcpServer::listen(const QHostAddress &address, qint16 port)
{
  if(!QTcpServer::listen(address, port)) return false;
  m_thread = new QThread(this);
  m_connections = new TcpConnections;

  // 将所有tcp连接移动到子线程中，cross-threaded 信号与槽
  connect(m_thread, &QThread::started, m_connections, &TcpConnections::start, Qt::QueuedConnection);
  connect(this, &TcpServer::accepting, m_connections, &TcpConnections::accept, Qt::QueuedConnection);
  connect(this, &TcpServer::finished, m_connections, &TcpConnections::quit, Qt::QueuedConnection);
  // m_connections->quit()，触发finished()信号，退出子线程
  connect(m_connections, &TcpConnections::finished, this, &TcpServer::complete, Qt::QueuedConnection);

  connect(this, &TcpServer::forward, m_connections, &TcpConnections::forward, Qt::QueuedConnection);
  m_connections->moveToThread(m_thread);
  m_thread->start();

  return true;
}
void TcpServer::close()
{
  qDebug() << this << "closing server";
  emit finished();
  QTcpServer::close();
}

qint16 TcpServer::port()
{
  if(isListening()){
      return this->serverPort();
    }
  else return 9999;
}

void TcpServer::incomingConnection(qintptr descriptor)
{
  TcpConnection* connection = new TcpConnection;
  connect(connection, &TcpConnection::textMessage, this, &TcpServer::forward, Qt::QueuedConnection);
  qDebug() << this << "new connection" << connection;
  accept(descriptor, connection);
}

void TcpServer::accept(qintptr descriptor, TcpConnection *connection)
{
  qDebug() << this << "attempting to accept connection" << descriptor;
  connection->moveToThread(m_thread);
  emit accepting(descriptor, connection); // 不能跨线程调用函数，因此采用信号机制
}

void TcpServer::complete()
{
  if(!m_thread){
      qWarning() << this << "exiting complete but no thread";
      return;
    }

  qDebug() << this << "Complete called, destroying thread";
  delete m_connections;

  qDebug() << this << "Quitting thread";
  m_thread->quit();
  m_thread->wait();

  delete m_thread;

  qDebug() << this << "complete";
}

