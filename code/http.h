#ifndef __HTTP_H_
#define __HTTP_H_
#include<sys/types.h>
#include<limits.h>
//http请求中的关键数据
typedef struct httpRequest{
	char method[32];	//请求方法 GET/POST
	char path[PATH_MAX + 1];//请求的资源路径 linux允许的最大路径长度
	char protocol[32];	//协议版本	http 1.0/1.1
	char connection[32];	//连接状态 keep-live/close
}HTTP_REQUEST;

//解析http请求   参数：客户端请求的数据串，解析出的内容
int parseRequest(const char* req, HTTP_REQUEST* hreq);

//http响应头的关键数据
typedef struct httpRespond{
	char protocol[32];	//协议版本
	int status;		//响应码
	char desc[64];		//状态描述
	char type[32];		//类型
	off_t length;		//长度
	char connection[32];	//连接状态
}HTTP_RESPOND;
//构造http响应  参数 响应头数据，拼接后的响应头串
int constructHead(const HTTP_RESPOND* hres,char* head);

#endif
