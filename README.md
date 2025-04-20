# Linux-web服务器（C语言）
主要练习Linux环境下的系统编程和网路编程

## 1.模块介绍

主模块:负责启动服务器模块；

服务器模块：负责接收客户端的连接请求；开启线程模块；

线程模块：负责完成和客户端通信；

通信模块：创建套接字---》组织地址结构---》绑定地址结构---》开启侦听---》接收客户端连接请求---》接收http请求---》发送http响应

http模块：解析http请求；构造http响应

资源模块：判断文件是否存在；判断文件类型

信号模块：忽略大部分无用信号



## 2.模块功能描述

### 主模块: main.c

`初始化服务器：initServer(端口号)      服务器端口号，默认80	`

`if(initServer(argc < 2 ? 80 : atoi(argv[1])) == -1){
		return -1;
	}`

`运行服务器：runServer(文件路径)      客户端可访问的文件路径	`

`if(runServer(argc < 3 ? "../home" : argv[2]) == -1){
		return -1;		
	}`

`终结服务器：deinitServer()`

### 服务器模块：server.c

`//初始化服务器 `

`忽略大部分无用信号 定义在Signals.c里的initSignals()`

`初始化侦听套接字 注意是侦听套接字，不是TCP连接的套接字  定义在socket.c里的initSocket(port)`

`	//初始化信号
	if(initSignals()==-1){
		return -1;
	}
	//初始化套接字
	if(initSocket(port) == -1){
		return -1;
	}`

`//运行服务器  `

`接收客户端TCP连接请求`

`	//接连接请求
		int conn = acceptClient();`

`创建线程，调用线程过程函数client，定义在client.c里`

 ` //开线程
     pthread_t tid;  //用来输出线程的ID
     pthread_attr_t attr;  //线程属性
     pthread_attr_init(&attr);  //初始化
     //采用分离线程,线程结束后，内核会自动回收
     pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);` 

`CA* ca = malloc(sizeof(CA));
		ca->conn = conn;
		ca->home = home;
		int error = pthread_create(&tid,&attr,client,ca);	//client为线程过程函数，定义在client文件里`

`//终结服务器`

`void deinitServer(void){
	deinitSocket();关闭侦听套接字，定义在socket.c里
}`



### 线程模块：client.c

`client(void* arg)`

`//接收请求，调用socket.c里的recvRequest(port)`

`//解析http请求中的路径，调用http.c里的parseRequest函数`

`//处理请求路径，与本地路径拼接`

`//搜索本地路径资源，验证类型，调用resource.c里的searchResource(path)`

`//构造http响应，调用http.c里的constructHead函数`

`//构造响应`

`//发送响应头，调用socket.c里的sendHead函数`

`//发送响应体，调用socket.c里的sendBody函数`

`//上述步骤循环执行，此为一个线程所作的事情`

### 通信模块： socket.c

//初始化侦听套接字

`创建套接字 s_sock = socket(AF_INET,SOCK_STREAM,0); `

`设置端口复用 setsockopt(s_sock,SOL_SOCKET, SO_REUSEADDR,&on,sizeof(on)`

`组织地址结构  `

`struct sockaddr_in ser;
	ser.sin_family = AF_INET;
	ser.sin_port = htons(port);
	ser.sin_addr.s_addr = INADDR_ANY;`

`绑定端口bind()`

`bind(s_sock,(struct sockaddr*)&ser,sizeof(ser)`

`启动侦听listen()listen(s_sock,1024)`

//接收客户端连接请求 

`  int conn = accept(s_sock,(struct sockaddr*)&cli,&len);`

`//发送响应头 send(conn,head,strlen(head),0)`

`//发送响应体 	`

`while((len = read(fd,buf,sizeof(buf)))>0){
		if(send(conn,buf,len,0) == -1){
			perror("send");
			return -1;
		}
	}`

`//接受客户端的http请求 `

`调用recv函数 ssize_t byte = recv(conn,buf,sizeof(buf)-1,0); `

`//终结化套接字，侦听套接字`

`	close(s_sock);`

### http模块：http.c

`//解析请求数据	
	GET /common/startup_scripts.js HTTP/1.1\r\n
	Host: 192.168.137.25\r\n
	User-Agent: Mozilla/5.0\r\n
	Accept: text/html\r\n
	Connection: keep-alive\r\n
	Referer: http://192/168.137.25/index.html\r\n\r\n
	`

`//构造响应	  
   HTTP/1.1 200 OK\r\n
   Server: LaozhangWebserver 4.0\r\n
   Date: Mon 20 Mar 2023 11:40:41\r\n
   Content-Type: application/x-javascript\r\n
   Content-Length: 12311\r\n
   Connection: keep-alive\r\n
   	`

### 资源模块： resource.c

`//搜索资源searchResource`

`int searchResource(const char* path){
    return access(path,R_OK);
}`

`//判断类型identifytype`

`int identifyType(const char* path,char* type)`

### 信号模块：signals.c

`//忽略大部分信号    `

 `for(int signum = SIGHUP;signum <= SIGRTMAX;signum++){
        if(signum != SIGINT && signum != SIGTERM){
            signal(signum,SIG_IGN);
        }
    } `

## 3. 运行项目

### 编译所有项目

`gcc -c client.c`

`gcc -c http.c`

`gcc -c resource.c`

`gcc -c server.c`

`gcc -c signals.c`

`gcc -c socket.c`

编译所有.c文件为.o文件

gcc *.o -o webserver -lpthread			//编译时要链接库文件pthread，星.o表示所有.o文件

运行项目

sudo ./webserver [端口号] [文件路径]		//后两个参数可不写，默认为80端口 上级目录的home文件夹

访问服务器

首先确保客户端与服务器在同一局域网下

服务器新建命令行窗口 输入 ifconfig  //查看ip地址

然后，在客户端浏览器输入查到的IP地址即可访问
