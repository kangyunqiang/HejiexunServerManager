#pragma once

#ifdef MESSAGEPIPE_EXPORTS
#define MESSAGEPIPE_API __declspec(dllexport)
#else
#define MESSAGEPIPE_API __declspec(dllimport)
#endif

typedef void* MsgPipeHandle;		//��Ϣ�ܵ����

//��Ϣ�ṹ
struct MsgInfo
{
	unsigned int type;					//��Ϣ����
	unsigned int id;						//��ϢID��ÿ����Ϣ��1
	unsigned int length;				//�û����ݳ���
	void * address;						//�û�����
};

#define MSG_PIPE_BUFFER_LENGTH (1024 * 1024 * 10)
#define MSG_PIPE_MEMORY_CACHE_CAPACITY (1024 * 1024 * 5)
#define MSG_PIPE_FILE_CACHE_CAPACITY (1024 * 1024 * 100)

//���µķ�����serverǰ׺��ֻ����server���ã���clientǰ׺��ֻ����client���ã���������˫�����ɵ��á�

//����������Ϣ�ܵ��� �����½�����Ϣ�ܵ����
MESSAGEPIPE_API MsgPipeHandle serverCreateMsgPipe(
	const char * name,									//��Ϣ�ܵ�����
	unsigned int buffer_length  = MSG_PIPE_BUFFER_LENGTH,										//�����ڴ���Ϣ����������
	unsigned int memory_cache_capacity = MSG_PIPE_MEMORY_CACHE_CAPACITY,	//��������Ϣ�����ڴ滺�泤��
	unsigned int file_cache_capacity = MSG_PIPE_FILE_CACHE_CAPACITY,					//��������Ϣ������滺���ļ�����
	bool two_side = true);											//�Ƿ�˫���շ�

//��ȡserver��������Ϣ�ܵ��� ������Ϣ�ܵ����
MESSAGEPIPE_API MsgPipeHandle clientGetMsgPipe(const char * name);						//��Ϣ�ܵ�����

//����һ����Ϣ�� ����ʵ�ʷ�������Ϣ����
MESSAGEPIPE_API unsigned int serverSend(
	MsgPipeHandle pipe,					//��Ϣ�ܵ����
	MsgInfo* msg_pos,						//��������Ϣ����
	unsigned int msg_count);				//��������Ϣ����

//����һ����Ϣ�� ����ʵ�ʷ�������Ϣ����
MESSAGEPIPE_API unsigned int clientSend(
	MsgPipeHandle pipe,					//��Ϣ�ܵ����
	MsgInfo* msg_pos,						//��������Ϣ����
	unsigned int msg_count);				//��������Ϣ����

//���;ɵı�������������Ϣ�� ����ʵ�ʷ����Ļ�����Ϣ����
MESSAGEPIPE_API unsigned int serverSendOldMsg(MsgPipeHandle pipe);					//��Ϣ�ܵ����

//���;ɵı�������������Ϣ�� ����ʵ�ʷ����Ļ�����Ϣ����
MESSAGEPIPE_API unsigned int clientSendOldMsg(MsgPipeHandle pipe);					//��Ϣ�ܵ����

//������Ϣ�� ����ʵ���յ�����Ϣ����
MESSAGEPIPE_API unsigned int serverRecv(
	MsgPipeHandle pipe, 					//��Ϣ�ܵ����
	MsgInfo** msg_pos);					//���ر���������յ�����Ϣ�����ַ

//������Ϣ�� ����ʵ���յ�����Ϣ����
MESSAGEPIPE_API unsigned int clientRecv(
	MsgPipeHandle pipe, 					//��Ϣ�ܵ����
	MsgInfo** msg_pos);					//���ر���������յ�����Ϣ�����ַ

//��ѯ�Ƿ�������Ϣ����
MESSAGEPIPE_API bool serverHasNewMsg(MsgPipeHandle pipe);					//��Ϣ�ܵ����

//��ѯ�Ƿ�������Ϣ����
MESSAGEPIPE_API bool clientHasNewMsg(MsgPipeHandle pipe);						//��Ϣ�ܵ����

//�ر���Ϣ�ܵ�
MESSAGEPIPE_API void serverClose(MsgPipeHandle pipe);//��Ϣ�ܵ����

//�ر���Ϣ�ܵ�
MESSAGEPIPE_API void clientClose(MsgPipeHandle pipe);//��Ϣ�ܵ����

//��ȡ�����ڴ���Ϣ����������
MESSAGEPIPE_API unsigned int getBufferLength(MsgPipeHandle pipe);