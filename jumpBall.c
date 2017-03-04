#include <stdio.h>
#include <stdlib.h>

// 判断是windows VC还是Linux GCC环境，分别加载不同的头文件
#if defined __GNUC__
#include <unistd.h>
#elif defined _MSC_VER
#include <windows.h>
#endif

// 定义宏用以实现相同的睡眠等待函数
#if defined __GNUC__
#include <unistd.h>
#define Sleep(x) usleep(x##000)
#elif defined _MSC_VER
#include <windows.h>
#endif

void clear();

 /* VC 和 GCC 下分别使用不同的清屏函数 */
void clear() {
    #ifdef __GNUC__
    system("clear");
    #elif defined _MSC_VER
    system("cls");
    #endif
}

int main(){
    int i,j;
    int vt_x = 1;
    int vt_y = 1;
    
    int loc_x = 3;
    int loc_y = 4;
    
    int left = 0;
    int top = 0;
    
    int right = 40;
    int bottom = 10;
    
    int times = 200;
    while(times > 0){
        times--;
        clear();
        loc_x = loc_x + vt_x;
        loc_y = loc_y + vt_y;
        
        if((loc_x == left)||(loc_x == right)){
            vt_x = - vt_x;
        }
        if((loc_y == top)||(loc_y == bottom)){
            vt_y = - vt_y;
        }

        for(i=0;i<loc_y;i++){
            printf("\n");
        }
        for(j=0;j<loc_x;j++){
            printf(" ");
        }
        printf("o");
        printf("\n");

        Sleep(500);
    }
    
}