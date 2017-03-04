#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h> 
#include <unistd.h>

#define PORT 8080
#define HOST "127.0.0.1"
#define BUFF_SIZE 4096
#define REQUEST_QUEUE_LENGTH 10

int startServer();

/**
 * 创建一个server socket
 */
int startServer() {
    int sock_fd;
    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET; // 协议族是IPV4协议
    server_addr.sin_port = htons(PORT); // 将主机的无符号短整形数转换成网络字节顺序
    server_addr.sin_addr.s_addr = inet_addr(HOST); // 用inet_addr函数将字符串host转为int型

    /**
     * int socket(int af, int type, int protocol);
     * af (adress faimly), 协议族， 有AF_INET(IPV4) AF_INET6(IPV6)等，也可用 PF(protocal family) 为前缀
     * type 数据传输方式，有 SOCK_STREAM（流套接字）、SOCK_DGRAM（数据报套接字）等
     * protocal 协议类型， 有 IPPROTO_TCP（TCP协议）、IPPROTO_UDP(UDP协议), 将此位置0，系统自己选择使用哪种协议
     */
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    /**
     * int bind(int sock, struct sockaddr *addr, socklen_t addrlen)
     * sock 即一开始创建的socket
     * sockaddr_in类型的结构：
     * struct sockaddr_in{
            sa_family_t     sin_family;   //地址族（Address Family），也就是地址类型
            uint16_t        sin_port;     //16位的端口号
            struct in_addr  sin_addr;     //32位IP地址
            char            sin_zero[8];  //不使用，一般用0填充
        };
     * sockaddr类型的地址结构：
     * struct sockaddr{
            sa_family_t  sin_family;   //地址族（Address Family），也就是地址类型
            char         sa_data[14];  //IP地址和端口号
       }
     * sockaddr 和 sockaddr_in 的长度相同，都是16字节，只是将IP地址和端口号合并到一起，用一个成员 sa_data 表示。
     * 要想给 sa_data 赋值，必须同时指明IP地址和端口号，例如”127.0.0.1:80“，
     * 没有相关函数将这个字符串转换成需要的形式，也就很难给 sockaddr 类型的变量赋值，所以使用 sockaddr_in 来代替。
     * 这两个结构体的长度相同，强制转换类型时不会丢失字节，也没有多余的字节。
     */
    if (bind(sock_fd, (struct sockaddr *) &server_addr, sizeof (server_addr)) == -1) {
        perror("bind_error");
        exit(1);
    }

    /**
     * 开始监听服务器端口
     * 当套接字正在处理客户端请求时，如果有新的请求进来，套接字是没法处理的，只能把它放进缓冲区，待当前请求处理完毕后，再从缓冲区中读取出来处理。
     * 如果不断有新的请求进来，它们就按照先后顺序在缓冲区中排队，直到缓冲区满。这个缓冲区，就称为请求队列（Request Queue）
     */
    if (listen(sock_fd, REQUEST_QUEUE_LENGTH) == -1) {
        perror("listen error");
        exit(1);
    }

    return (sock_fd);
}

int main() {
    int server_fd;
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t len_client_addr = sizeof (client_addr);
    char http_request[BUFF_SIZE], http_header[BUFF_SIZE], http_response[BUFF_SIZE];
    char *http_res_tmpl = "HTTP/1.1 200 OK\r\n"
            "Server: ZBS's Server V1.0\r\n"
            "Accept-Ranges: bytes\r\n"
            "Content-Length: %d\r\n"
            "Connection: close\r\n"
            "Content-Type: %s\r\n\r\n";
    char content[] = "hello boy!";
    int len = sizeof (content);

    server_fd = startServer();
    while (1) {
        /**
         * 受理客户端的请求
         * int accept(int sock, struct sockaddr *addr, socklen_t *addrlen);
         * sock 服务器套接字
         * accept() 会阻塞程序执行
         */
        client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &len_client_addr);

        memset(http_request, 0, BUFF_SIZE); // 初始化缓冲区内容

        recv(client_fd, http_request, BUFF_SIZE, 0); // 从客户端socket中读取信息

        // int sprintf(char *str, char * format [, argument, ...]) 以format将参数格式化，返回str的长度
        sprintf(http_header, http_res_tmpl, len, "text/html");
        sprintf(http_response, "%s%s", http_header, content);

        // 向socket客户端写入响应结果
        send(client_fd, http_response, sizeof (http_response), 0);
        close(client_fd);
    }
    close(server_fd);
    return 0;
}