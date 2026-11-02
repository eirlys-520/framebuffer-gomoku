#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include"game.h"
#include"framebuffer.h"

#define RED   0x00FF0000
#define GREEN 0x0000FF00
#define BLUE  0x000000FF
#define BLACK 0x00000000
#define WHITE 0x00FFFFFF
#define YELLOW 0x00FFFF00

//棋盘横向起始位置的偏移量
#define BROAD_X_START 163
//棋盘纵向起始位置的偏移量
#define BROAD_Y_START 63

//棋盘横向位置个数
#define BROAD_X_CNT 20
//棋盘纵向位置个数
#define BROAD_Y_CNT 20
//棋盘单个格子的宽度
#define BROAD_W_ONE 25

#define WHITE_CHESS 1
#define BLACK_CHESS -1

int posx_g = BROAD_X_START;
int posy_g = BROAD_Y_START;
int chess_g[BROAD_Y_CNT][BROAD_X_CNT] = {0};
int flag = BLACK_CHESS;


void draw_bmp(int x, int y, char *bmpname)
{
#pragma pack(1)
//bmp文件相关信息
typedef struct tagBITMAPFILEHEADER {
    short    bfType;         // 文件类型标志
    int      bfSize;         // 文件大小，单位为字节
    short    bfReserved1;    // 保留字节
    short    bfReserved2;    // 保留字节
    int      bfOffBits;      // 数据偏移量，即实际图像数据开始的位置
}Bmp_file_head_t;
//bmp图像信息
typedef struct tagBITMAPINFOHEADER {
    int    biSize;         // BITMAPINFOHEADER的大小，单位为字节
    int    biWidth;        // 位图的宽度，单位为像素
    int    biHeight;       // 位图的高度，单位为像素
    short  biPlanes;       // 目标设备的位平面数，必须为1
    short  biBitCount;     // 每像素位数（颜色深度）
    int    biCompression;  // 图像压缩类型
    int    biSizeImage;    // 图像大小，单位为字节
    int    biXPelsPerMeter;// 水平分辨率，单位为像素/米
    int    biYPelsPerMeter;// 垂直分辨率，单位为像素/米
    int    biClrUsed;      // 实际使用颜色数
    int    biClrImportant; // 重要颜色数
}Bmp_info_t;
#pragma pack()

    Bmp_file_head_t headinfo;
	Bmp_info_t bmpinfo;

    int fd = open(bmpname, O_RDONLY);
    if (-1 == fd)
    {
        perror("open bmp error");
        return ;
    }

	read(fd, &headinfo, sizeof(headinfo));
	read(fd, &bmpinfo,  sizeof(bmpinfo));
    unsigned char *buff = malloc(bmpinfo.biHeight*bmpinfo.biWidth*bmpinfo.biBitCount/8);
    read(fd, buff, bmpinfo.biHeight*bmpinfo.biWidth*bmpinfo.biBitCount/8);
    close(fd);

    unsigned char *p = buff;
    unsigned char r, g, b;

    for (int j = bmpinfo.biHeight-1; j >= 0; j--)
    {
        for (int i = 0; i < bmpinfo.biWidth; i++)
        {
            b = *p;++p;
            g = *p;++p;
            r = *p;++p;
            unsigned int col = (r << 16) | (g << 8) | (b << 0);
			draw_point(i+x, j+y, col);

        }

    }

    free(buff);
}

void show_chess_broad()
{
    draw_clear(0x00FFF8DC);   

    int i;
    //绘制横向网格线
    for(i = 0; i < BROAD_Y_CNT; i++)
    {
        draw_h_line(BROAD_X_START,BROAD_Y_START + i*BROAD_W_ONE, (BROAD_X_CNT - 1)*BROAD_W_ONE,BLACK);
    }
    //绘制纵向网格线
    for(i = 0; i < BROAD_X_CNT; i++)
    {
        draw_s_line(BROAD_X_START + i*BROAD_W_ONE,BROAD_Y_START,(BROAD_Y_CNT - 1)*BROAD_W_ONE,BLACK);
    }
    //绘制光标小方块
    draw_square(posx_g-5, posy_g-5, 10, 10, RED);

    //遍历绘制棋盘上所有棋子
    
    int x,y;
    for(y = 0; y < BROAD_Y_CNT; ++y)
    {
        for(x = 0; x < BROAD_X_CNT; ++x)
        {
            if(WHITE_CHESS == chess_g[y][x])
            {
                draw_circle(BROAD_X_START + x *BROAD_W_ONE,BROAD_Y_START + y *BROAD_W_ONE,10,WHITE);
            }
            else if(BLACK_CHESS == chess_g[y][x])
            {
                draw_circle(BROAD_X_START + x *BROAD_W_ONE,BROAD_Y_START + y *BROAD_W_ONE,10,BLACK);
            }
        }
    }

}



void begin_game()
{
    char choose = 0;
    int x,y;
    while(1)
    {
        choose = getchar();
        while(getchar() != '\n' );
        switch(choose)
        {
            case 'w':   
                if(posy_g > BROAD_Y_START)
                {
                    posy_g -= BROAD_W_ONE;
                }
                break;
            case 's':   
                if(posy_g < BROAD_Y_START + (BROAD_Y_CNT - 1)*BROAD_W_ONE)
                {
                    posy_g += BROAD_W_ONE;
                }
                break;
            case 'a':   
                if(posx_g > BROAD_X_START)
                {
                    posx_g -= BROAD_W_ONE;
                }
                break;
            case 'd':
                if(posx_g < BROAD_X_START + (BROAD_X_CNT - 1)*BROAD_W_ONE)
                {
                    posx_g += BROAD_W_ONE;
                }
                break;

            case ' ':   //空格键落子
                //换算成棋盘数组下标
                x = (posx_g - BROAD_X_START)/BROAD_W_ONE; //列 x
                y = (posy_g - BROAD_Y_START)/BROAD_W_ONE; //行 y

                if(chess_g[y][x] == 0)
                {
                    chess_g[y][x] = flag;
                    show_chess_broad();
                    int res = is_win(y,x,chess_g); //x列、y行，顺序和函数匹配
                    if(res != 0)
                    {
                        show_chess_broad();
                        sleep(1);
                        if(res == BLACK_CHESS)
                        {
                            draw_bmp(0, 0, "black.bmp");
                        }
                        else if(res == WHITE_CHESS)
                        {
                            draw_bmp(0, 0, "white.bmp");
                        }
                        while(1);
                    }
                    flag = -flag; //切换黑白棋子
                }
                break;
        }
        show_chess_broad();
    }
}



int is_win(int i, int j, int chess_g[BROAD_Y_CNT][BROAD_X_CNT])
{
    int color = chess_g[j][i];
    if(color == 0)
    {
        return 0;
    }
    int count;

    //1、横向左右
    count = 1;
    for(int k = i - 1; k >= 0 && chess_g[j][k]==color; k--)
        count++;
    for(int k = i + 1; k < BROAD_X_CNT && chess_g[j][k]==color; k++)
        count++;
    if(count >= 5)
        return color;

    //2、纵向上下
    count = 1;
    for(int k = j - 1; k >= 0 && chess_g[k][i]==color; k--)
        count++;
    for(int k = j + 1; k < BROAD_Y_CNT && chess_g[k][i]==color; k++)
        count++;
    if(count >= 5)
        return color;

    //3、左上‑右下斜线
    count = 1;
    for(int ki = i-1, kj = j-1; ki >= 0 && kj >= 0 && chess_g[kj][ki]==color; ki--,kj--)
        count++;
    for(int ki = i+1, kj = j+1; ki < BROAD_X_CNT && kj < BROAD_Y_CNT && chess_g[kj][ki]==color; ki++,kj++)
        count++;
    if(count >=5)
        return color;

    //4、右上‑左下斜线
    count = 1;
    for(int ki = i+1, kj = j-1; ki < BROAD_X_CNT && kj >= 0 && chess_g[kj][ki]==color; ki++,kj--)
        count++;
    for(int ki = i-1, kj = j+1; ki >= 0 && kj < BROAD_Y_CNT && chess_g[kj][ki]==color; ki--,kj++)
        count++;
    if(count >=5)
        return color;

    return 0;
}

