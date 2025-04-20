# Linux-web服务器（C语言）
主要练习Linux环境下的系统编程和网路编程
### 编译项目

`gcc -c client.c`

`gcc -c http.c`

`gcc -c resource.c`

`gcc -c server.c`

`gcc -c signals.c`

`gcc -c socket.c`

编译所有.c文件为.o文件

gcc *.o -o webserver -lpthread			//编译时要链接库文件pthread

运行项目

sudo ./webserver [端口号] [文件路径]		//后两个参数可不写，默认为80端口 上级目录的home文件夹

访问服务器

首先确保客户端与服务器在同一局域网下

服务器新建命令行窗口 输入 ifconfig  //查看ip地址

然后，在客户端浏览器输入查到的IP地址即可访问
