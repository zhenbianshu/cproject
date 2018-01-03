#include <stdio.h>
#include <string.h>

#define BLK " "

// todo 查看 strtok strcat  strcpy 函数的用法

int main() {
    char ori_str[2048]; // 输入的字符串
    char longest_word[256]; // 最长的单词
    char *tmp_word; // 解析词
    int word_length; // 当前最长单词的长度

    printf("请输入:\n");
    scanf("%[^\n]", ori_str); // 注意：输入字符串，注意不能用 %s, 会使用空格作为分隔符

    tmp_word = strtok(ori_str, BLK);

    strcpy(longest_word, tmp_word); // 先将第一个单词初始化为当前最长单词
    word_length = strlen(longest_word);

    while ((tmp_word = strtok(NULL, BLK))) {
        // 两单词同长时，将换行符和后一个单词添加到最长单词结果中
        if (word_length == strlen(tmp_word)) {
            strcat(longest_word, "\n");
            strcat(longest_word, tmp_word);
            // 刚解析出的单词长时，将新单词替换原最长单词，并修改当前最长单词长度值
        } else if (word_length < strlen(tmp_word)) {
            memset(longest_word, 0, strlen(longest_word)); // 注意：将结果初始化，防止当前结果为两个单词同长，直接复制无法完全覆盖的情况
            strcpy(longest_word, tmp_word);
            word_length = strlen(tmp_word);
        }
    }

    printf("输出:\n%s\n", longest_word);
}