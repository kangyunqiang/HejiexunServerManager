#pragma once

#ifdef MESSAGEPIPE_EXPORTS
#define MESSAGEPIPE_API __declspec(dllexport)
#else
#define MESSAGEPIPE_API __declspec(dllimport)
#endif

typedef void* MsgPipeHandle;		//消息管道句柄

//消息结构
struct MsgInfo
{
	unsigned int type;					//消息类型
	unsigned int id;						//消息ID，每个消息加1
	unsigned int length;				//用户数据长度
	void * address;						//用户数据
};

#define MSG_PIPE_BUFFER_LENGTH (1024 * 1024 * 10)
#define MSG_PIPE_MEMORY_CACHE_CAPACITY (1024 * 1024 * 5)
#define MSG_PIPE_FILE_CACHE_CAPACITY (1024 * 1024 * 100)

//以下的方法带server前缀的只能由server调用，带client前缀的只能由client调用，其他方法双方都可调用。

//创建命名消息管道， 返回新建的消息管道句柄
MESSAGEPIPE_API MsgPipeHandle serverCreateMsgPipe(
	const char * name,									//消息管道名称
	unsigned int buffer_length  = MSG_PIPE_BUFFER_LENGTH,										//共享内存消息缓冲区长度
	unsigned int memory_cache_capacity = MSG_PIPE_MEMORY_CACHE_CAPACITY,	//待发送消息本地内存缓存长度
	unsigned int file_cache_capacity = MSG_PIPE_FILE_CACHE_CAPACITY,					//待发送消息本地外存缓存文件长度
	bool two_side = true);											//是否双向收发

//获取server创建的消息管道， 返回消息管道句柄
MESSAGEPIPE_API MsgPipeHandle clientGetMsgPipe(const char * name);						//消息管道名称

//发送一组消息， 返回实际发出的消息数量
MESSAGEPIPE_API unsigned int serverSend(
	MsgPipeHandle pipe,					//消息管道句柄
	MsgInfo* msg_pos,						//待发送消息数组
	unsigned int msg_count);				//待发送消息数量

//发送一组消息， 返回实际发出的消息数量
MESSAGEPIPE_API unsigned int clientSend(
	MsgPipeHandle pipe,					//消息管道句柄
	MsgInfo* msg_pos,						//待发送消息数组
	unsigned int msg_count);				//待发送消息数量

//发送旧的被缓存起来的消息， 返回实际发出的缓存消息数量
MESSAGEPIPE_API unsigned int serverSendOldMsg(MsgPipeHandle pipe);					//消息管道句柄

//发送旧的被缓存起来的消息， 返回实际发出的缓存消息数量
MESSAGEPIPE_API unsigned int clientSendOldMsg(MsgPipeHandle pipe);					//消息管道句柄

//接收消息， 返回实际收到的消息数量
MESSAGEPIPE_API unsigned int serverRecv(
	MsgPipeHandle pipe, 					//消息管道句柄
	MsgInfo** msg_pos);					//返回变量，存放收到的消息数组地址

//接收消息， 返回实际收到的消息数量
MESSAGEPIPE_API unsigned int clientRecv(
	MsgPipeHandle pipe, 					//消息管道句柄
	MsgInfo** msg_pos);					//返回变量，存放收到的消息数组地址

//查询是否有新消息到达
MESSAGEPIPE_API bool serverHasNewMsg(MsgPipeHandle pipe);					//消息管道句柄

//查询是否有新消息到达
MESSAGEPIPE_API bool clientHasNewMsg(MsgPipeHandle pipe);						//消息管道句柄

//关闭消息管道
MESSAGEPIPE_API void serverClose(MsgPipeHandle pipe);//消息管道句柄

//关闭消息管道
MESSAGEPIPE_API void clientClose(MsgPipeHandle pipe);//消息管道句柄

//获取共享内存消息缓冲区长度
MESSAGEPIPE_API unsigned int getBufferLength(MsgPipeHandle pipe);