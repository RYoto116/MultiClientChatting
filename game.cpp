///*
//	需求:
//	登录界面：
//		登录：用户输入账号密码，进入用户界面
//		注册：进入注册界面
//		退出
//		忘记密码：进入重置密码界面
//	注册界面：
//		用户输入账号密码，点击注册
//		取消注册：回到登录界面
//	用户界面：
//		展示用户的群聊列表和好友列表
//		进入群聊：进入通信界面
//		进入私聊：进入私聊界面
//		添加群聊/好友：进入添加界面
//		退出登录：回到登录界面
//	通信界面：
//		消息显示框显示历史消息、消息时间和用户账号；用户可以在消息输入框输入消息并发送，消息显示框更新新的消息
//		用户下线：回到用户界面
//		退出群组：在群组人员中删除用户，回到用户界面，在群聊列表中删除群组
//	添加界面：
//		根据账号或群号添加好友或群组，更新到用户界面
//	重置密码界面：
//		用户重新设置密码并确认
//	私聊界面：
//		用户与指定好友进行实时通信
//*/

//#pragma once
//#include <QString>
//#include <QTcpSocket>

//struct Header{};


///*
//服务器流程：listen->newConnection->nextPendingConnection->readAll/write
//客户端流程：connectToHost->waitForConnected->write/readAll
//*/


//// 服务器
////---------------------------Service---------------------------
///*
//	负责人: 擅长逻辑的李四
//	功能:
//	    初始化服务器数据:
//		1. 创建线程池
//		2. 初始化服务器监听端口: int m_port 监听套接字 QTcpServer* m_server
//		3. 已连接客户端列表: QTcpSocket* clientSockets[MAX_USER]; -->后续优化为红黑树（set或map）
//		4. 当前客户端socket数量 static int client_num = 0;
//	槽：
//	    onNewClientConnected处理用户新的连接
//	    onRecvData 接收用户数据
//	    onClientDisconnected 处理用户断开连接
//	信号：
//	参数: void
//	返回值: void
//*/
//void ServerInit();


///*
//    对读取到的数据进行解析header,根据不同的header类型，进入不同的线程类型

//*/
//void DataParse();


///*
//对数据进行进一步的解析，如果为登录，则匹配用户数据库，
//如果为注册，查找是否有相同账号，如果有，失败，无，成功。
//进行按InitPackage格式进行数据组包，回传

//*/

//void General_control_thread();


///*对数据进行进一步的解析,向用户队列、消息队列里增删用户、增加消息*/
//void Chat_control_thread();


///*循环读取消息，转发消息*/
//void Chat_thread();


///*
//  套接字的复用操作：需要重写incomingConnection函数（这也是之后我们实现客户端与客户端通信的基础）

//  clientSockets[client_num++] = m_server->nextPendingConnection();

//*/

//void onNewClientConnected();


///*

//  解析消息

//*/
//void onRecvData();

///*



//*/

//void onClientDisconnected();



///*
//	负责人: 擅长逻辑的李四
//	功能:
//	    给客户端分配一个登录注册线程
//	    判断用户账号是否合法：
//		1. 8位数字
//		dao: 2. 判断账号在user表中是否存在
//	    判断用户密码是否合法：
//		1. 6~12位数字或字母
//		dao: 2. 判断密码是否正确
//	       view: 3. 登录成功，进入用户界面；否则提示登录失败
//	参数: QString id, QString passwd
//	返回值: bool: 登录成功 返回true    登录失败 返回false
//*/
//bool userLogin(QString& id, QString& passwd);


///*
//	负责人: 擅长逻辑的李四
//	功能:
//	    给客户端分配一个登录注册线程
//	    判断用户账号是否合法：
//		1. 8位数字
//		dao: 2. 判断账号在user数据库中是否存在
//	    判断用户密码是否合法：
//		1. 6~12位数字或字母
//		dao: 2. 若合法，将用户加入user表，
//		view: 返回登录界面

//	参数: QString id, QString passwd
//	返回值: bool 注册成功 返回true    注册失败 返回false
//*/
//bool userRegister(QString& id, QString& passwd);



///*
//	负责人: 擅长逻辑的李四
//	功能:
//	    给客户端分配一个会话线程
//	    将sender发送的信息转发给指定用户：
//		群聊：receiver=None
//		私聊：groupID=-1

//	参数: int groupID, QTcpSocket* sender, QTcpSocket* receiver, QByteArray msg
//	返回值: int 发送成功返回1  发送失败返回0
//*/
//int messageSend(int groupID, QTcpSocket* sender, QTcpSocket* receiver, QString msg);


///*
//	负责人: 擅长逻辑的李四
//	功能:
//	    分析数据包，提取消息头和消息体
//	    根据消息类型启动相关的线程

//	参数: QString package
//	返回值: int 返回消息类型
//*/
//int dataParse(QString package);

///*
//	负责人: 擅长逻辑的李四
//	功能:
//	    将消息头和消息体组成数据包

//	参数: QByteArray package
//	返回值: int 返回消息类型
//*/

//QString dataPack(Header header, QByteArray data);


///*查找账号密码是否匹配*/
//void UserMatch();

///*判断账号是否被注册*/
//void RegisterACK();
