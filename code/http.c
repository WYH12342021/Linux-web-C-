//http模块的具体实现
#include<stdio.h>
#include<sys/syscall.h>
#include<unistd.h>
#define __USE_GNU	//为了使用case版本字符串操作的函数
#include<string.h>
#include<time.h>
#include"http.h"

//解析请求
int parseRequest(const char* req,HTTP_REQUEST* hreq){
	/*
		GET /common/startup_scripts.js HTTP/1.1\r\n
		Host: 192.168.137.25\r\n
		User-Agent: Mozilla/5.0\r\n
		Accept: text/html\r\n
		Connection: keep-alive\r\n
		Referer: http://192/168.137.25/index.html\r\n\r\n
	*/
	sscanf(req,"%s%s%s",hreq->method,hreq->path,hreq->protocol);	//依据空格把大串拆成小串(方法，资源路径，协议版本)
	char* con = strcasestr(req,"connection");
	if(con){
	
		/*Connection: keep-alive\r\n
		Referer: http://192/168.137.25/index.html\r\n\r\n */
		sscanf(con,"%*s%s",hreq->connection);	//拆开 connection：，keep-alive  %*s是虚读并不保存
	}
	printf("%d.%ld > [%s][%s][%s][%s]\n",getpid(),syscall(SYS_gettid),
			hreq->method,hreq->path,hreq->protocol,hreq->connection);
	//判断是否为get方法
	if(strcasecmp(hreq->method,"get")){
		printf("%d.%ld > 无效的方法\n",getpid(),syscall(SYS_gettid));
		return -1;
	}
	//判断协议版本
	if(strcasecmp(hreq->protocol,"http/1.1") && strcasecmp(hreq->protocol,"http/1.0")){
		printf("%d.%ld > 无效的版本\n",getpid(),syscall(SYS_gettid));
		return -1;
	}
}

//构造响应头
int constructHead(const HTTP_RESPOND* hres, char* head){
	/*
	HTTP/1.1 200 OK\r\n
	Server: LaozhangWebserver 4.0\r\n
	Date: Mon 20 Mar 2023 11:40:41\r\n
	Content-Type: application/x-javascript\r\n
	Content-Length: 12311\r\n
	Connection: keep-alive\r\n
	*/

	char dateTime[32];
	time_t now = time(NULL);
	strftime(dateTime,sizeof(dateTime),"%a %d %b %Y %T",gmtime(&now));
	sprintf(head,"%s %d %s\r\n"
		     "Server: LAOZHANGZUISHUAI\r\n"
		     "Date: %s\r\n"
		     "Content-Type: %s\r\n"
		     "Content-Length: %ld\r\n"
		     "Connection: %s\r\n\r\n",
		     hres->protocol,hres->status,hres->desc,dateTime,
		     hres->type,hres->length,hres->connection);
	return 0;
}
