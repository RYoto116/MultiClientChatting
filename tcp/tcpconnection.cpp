#include "loginrunnable.h"
#include "tcpconnection.h"
#include "registerrunnable.h"

int TcpConnection::globalID  = 0;
TcpConnection::TcpConnection(QObject *parent)
  : QObject{parent}
{
  qDebug() << this <<"Created";
  requestType = 0;
  channel = 1; // 先写死
//  clientID = ++globalID;
  state = OFFLINE;

  memset(account, 0, 20);
  memset(passwd, 0, 20);
}

TcpConnection::~TcpConnection()
{
  qDebug() << this << "Destroyed";
}

void TcpConnection::setSocket(QTcpSocket *socket)
{
  m_socket = socket;
  connect(m_socket, &QTcpSocket::connected, this, &TcpConnection::connected);
  connect(m_socket, &QTcpSocket::disconnected, this, &TcpConnection::disconnected);
  connect(m_socket, &QTcpSocket::readyRead, this, &TcpConnection::readyRead);
  connect(m_socket, &QTcpSocket::bytesWritten, this, &TcpConnection::bytesWritten);
  connect(m_socket, &QTcpSocket::stateChanged, this, &TcpConnection::stateChanged);
  connect(m_socket, static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error), this, &TcpConnection::error);
}

QTcpSocket *TcpConnection::getSocket()
{
  // 系统会记录下emit出这个信号的对象，在对应的slot里可以通过 sender()得到当前是谁invoke了你的slot
//  if(!sender()) return m_socket;
//  return static_cast<QTcpSocket*>(sender());
    return m_socket;
}

int TcpConnection::getState()
{
  return state;
}

void TcpConnection::connected()
{
  if(!sender()) return;
  qDebug() << this << "connected " << getSocket();
}

void TcpConnection::disconnected()
{
  qDebug() << this << "disconnected " << getSocket();
}

void TcpConnection::readyRead()
{
  qDebug() << this << "readyRead " << getSocket();

  m_socket->read((char*)&requestType, 4); // 读取4B消息头
  qDebug() << this << "接收请求，解析消息类型：" << requestType;
  switch(requestType){
    case LOGIN:
      if(OFFLINE == state) loggingIn();
//      m_socket->flush();
      break;
    case REGISTER:
      if(OFFLINE == state) registering();
//      m_socket->flush();
      break;
    case EXIT_1: // 退出登录注册
      requestType = 0;
      state = OFFLINE;
      m_socket->readAll();
      break;
    case SELGROUP:
      emit selGroup(0);
      m_socket->readAll();
      break;
    case EXIT_2: // 退出群组
      requestType = 0;
      state = OFFLINE;
      m_socket->readAll();
      break;
    case GROUP:
        emit textMessage(0, this);
      break;
    default:
      m_socket->readAll();
      break;
    }
}

void TcpConnection::bytesWritten(qint64 bytes)
{
  qDebug() << this << " bytesWritten " << getSocket() << " number of bytes = " << bytes;
}

void TcpConnection::stateChanged(QAbstractSocket::SocketState socketState)
{
  qDebug() << this << " stateChanged " << getSocket() << " state = " << socketState;
}

void TcpConnection::error(QAbstractSocket::SocketError socketError)
{
  qDebug() << this << " error " << getSocket() << " error = " << socketError;
}

void TcpConnection::registering()
{
  qDebug() << this << "消息类型:" << requestType;

  RegisterRunnable* regTask = new RegisterRunnable();

  regTask->buffer = m_socket->readAll();
  regTask->connection = this;
  regTask->setAutoDelete(true);
  connect(regTask, &RegisterRunnable::result, this, &TcpConnection::initPackageResponse, Qt::QueuedConnection);
  QThreadPool::globalInstance()->start(regTask);
}

void TcpConnection::loggingIn()
{
  qDebug() << this << "消息类型:" << requestType;

  LoginRunnable* logInTask = new LoginRunnable();
  logInTask->buffer = m_socket->readAll();
  logInTask->connection = this;
  logInTask->setAutoDelete(true);
  connect(logInTask, &LoginRunnable::result, this, &TcpConnection::changeState, Qt::QueuedConnection);
  QThreadPool::globalInstance()->start(logInTask);
}

void TcpConnection::changeState(int res)
{
  if(requestType == 0) return;
  if(requestType == LOGIN && res == SUCCESS) {
      state = ONLINE;
      clientID = ++globalID;
    }
  initPackageResponse(res);
}

void TcpConnection::initPackageResponse(int res)
{
  InitPackage pack(requestType, res, clientID);

  QByteArray bytes;
  bytes.append(reinterpret_cast<char*>(&pack), sizeof (pack));

  requestType = 0;

  m_socket->write(bytes, sizeof(pack));
  m_socket->flush();
}
