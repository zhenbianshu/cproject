#include <stdio.h>

/**
 * 简单的实现文件倒读
 * @return
 */
int main(){
    FILE *fp;
    char line[1024];
    fp = fopen("/tmp/test_reverse.txt", "r");

    fseek(fp, -14, SEEK_END);
    while (fgets(line, 1024, fp)){
        fseek(fp, -28, SEEK_CUR);
        sleep(1);
        printf("%s", line);
    }
}