#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h> 
#include <unistd.h>

#define PORT 8080
#define HOST "127.0.0.1"
#define BUFF_SIZE 1024
#define REQUEST_QUEUE_LENGTH 10

int server_fd;
int client_fd;
struct sockaddr_in client_addr;
socklen_t len_client_addr = sizeof(client_addr);
char http_request[BUFF_SIZE], response_header[BUFF_SIZE], response_content[BUFF_SIZE], http_response[BUFF_SIZE];
char *header_tmpl = "HTTP/1.1 200 OK\r\n"
        "Server: ZBS's Server V1.0\r\n"
        "Accept-Ranges: bytes\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n"
        "Content-Type: text/html\r\n\r\n";

int startServer();
void dealRequest(char *http_request);
void dealResponse();
char * execPHP();

/**
 * 创建一个server socket
 */
int startServer() {
    int sock_fd;
    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET; // 协议族是IPV4协议
    server_addr.sin_port = htons(PORT); // 将主机的无符号短整形数转换成网络字节顺序
    server_addr.sin_addr.s_addr = inet_addr(HOST); // 用inet_addr函数将字符串host转为int型

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (bind(sock_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        perror("bind_error");
        exit(1);
    }

    if (listen(sock_fd, REQUEST_QUEUE_LENGTH) == -1) {
        perror("listen error");
        exit(1);
    }

    return (sock_fd);
}

/**
 * 处理响应内容
 */
void dealResponse() {
    sprintf(response_header, header_tmpl, strlen(response_content));
    sprintf(http_response, "%s%s", response_header, response_content);
}

/**
 * 处理请求参数
 *
 * @param request_content
 */
void dealRequest(char *request_content) {
    char *result;
    char *first_line = strtok(request_content, "\n"); // 获取第一行 结构类似 "GET /path VERSION"
    char *method = strtok(first_line, " ");  // 获取到请求方法
    if(strcmp(method, "GET") != 0){
        result = "只支持HTTP GET 方法！";
    }else{
        char *param = strtok(NULL, " ");  // 获取到参数
        result = execPHP(param);
    }
    strcpy(response_content, result);
}

/**
 * 执行PHP脚本以返回执行结果
 *
 * @param args
 * @return
 */
char * execPHP(char *args){
    // 这里不能用变长数组，需要给command留下足够长的空间，以存储args参数，不然拼接参数时会栈溢出
    char command[BUFF_SIZE] = "php /Users/mfhj-dz-001-441/CLionProjects/cproject/tinyServer/index.php ";
    FILE *fp;
    static char buff[BUFF_SIZE]; // 声明静态变量以返回变量指针地址
    char line[BUFF_SIZE];
    strcat(command, args);
    memset(buff, 0, BUFF_SIZE); // 静态变量会一直保留，这里初始化一下
    if((fp = popen(command, "r")) == NULL){
        strcpy(buff, "服务器内部错误");
    }else{
        // fgets会在获取到换行时停止，这里将每一行拼接起来
        while (fgets(line, BUFF_SIZE, fp) != NULL){
            strcat(buff, line);
        };
    }

    return buff;
}

int main() {
    server_fd = startServer();
    while (1) {
        client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &len_client_addr);

        memset(http_request, 0, BUFF_SIZE); // 由于是全局变量，防止上次连接的变量污染，初始化缓冲区内容
        recv(client_fd, http_request, BUFF_SIZE, 0);

        dealRequest(http_request);
        dealResponse();

        send(client_fd, http_response, sizeof(http_response), 0);
        close(client_fd);
    }
}