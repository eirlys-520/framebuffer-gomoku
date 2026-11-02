#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <math.h>
#include "framebuffer.h"

void *pmem = NULL;
int fb;
struct fb_var_screeninfo vinfo;

int fbx_g = 0;
int fby_g = 0;

int init_fb(char *devname)
{
    //1. 打开显示设备(/dev/fb0)
    fb = open(devname, O_RDWR);
    if (-1 == fb)
    {
        perror("open fb error");
        return -1;
    }

    //2. 获取显示设备相关参数（分辨率，像素格式）
   
    int ret = ioctl(fb, FBIOGET_VSCREENINFO, &vinfo);
    if (ret < 0)
    {
        perror("ioctl error");
        return -1;
    }
	
    printf("xres = %d, yres = %d\n", vinfo.xres, vinfo.yres);
  //  printf("xres_virtual = %d, yres_virtual = %d\n", vinfo.xres_virtual, vinfo.yres_virtual);
    printf("bits_per_pixel = %d\n", vinfo.bits_per_pixel);
	
	fbx_g = vinfo.xres;
	fby_g = vinfo.yres;

    //3. 建立显存空间和用户空间的内存映射
    size_t len = vinfo.xres_virtual * vinfo.yres_virtual * vinfo.bits_per_pixel/8;
    pmem = mmap(NULL, len, PROT_READ|PROT_WRITE, MAP_SHARED, fb, 0);
    if (pmem == MAP_FAILED)
    {
        perror("mmap error");
        return -1;
    }

    return 0;
}



void draw_point(int x, int y, unsigned int col)
{
    if (x >= vinfo.xres || y >= vinfo.yres)
    {
        return ;
    }
    if (vinfo.bits_per_pixel == RGB_FMT_888)
    {
        unsigned int *p = pmem;
        *(p+vinfo.xres_virtual*y+x) = col;
    }
    else if (vinfo.bits_per_pixel == RGB_FMT_565)
    {
        unsigned short *p = pmem;
        *(p+vinfo.xres_virtual*y+x) = col;
    }
}
void draw_clear(unsigned int col)
{
    for(int i = 0;i < fby_g;++i)
    {
        draw_h_line(0,i,fbx_g,col);
    }
}


void draw_h_line(int x, int y, int len, unsigned int col)
{
	for (int i = x; i < x+len; ++i)
	{
		draw_point(i, y, col);
	}
}


void draw_s_line(int x,int y,int len,unsigned int col)
{
    for(int i = y;i < y+len;++i)
    {
        draw_point(x,i,col);
    }
}


void draw_square(int x,int y,int h,int w,unsigned int col)
{
    for(int j = y;j < y+h;++j)
    {
        for(int i = x;i < x+w;++i)
        {
            draw_point(i,j,col);
        }
    }
}

void draw_circle(int x0,int y0,int r,unsigned int col)
{
    int x = 0,y = 0,r0 = 0;
    for(r0  = 0; r0 <= r; ++r0)
    {
        for(float si = 0;si <= 360;si+=1)
        {
            x = x0 + r0 * cos(2*3.1416/360 * si);
            y = y0 + r0 * sin(2*3.1416/360 * si);
            draw_point(x,y,col);
        }
    }
}

