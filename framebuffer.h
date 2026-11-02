#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#define RGB_FMT_888 32
#define RGB_FMT_565 16

//当前显示设备可视化界面的分辨率
extern int fbx_g;   //800   横向分辨率
extern int fby_g;   //600   纵向分辨率

//初始化显示设备:/dev/fb0
extern int init_fb(char *devname);
//绘制一个像素点
//x ： 横向坐标
//y ： 纵向坐标
//col ：RGB888颜色值
extern void draw_point(int x, int y, unsigned int col);

extern void draw_background(int x,int y,unsigned int col);

extern void draw_h_line(int x, int y, int len, unsigned int col);

extern void draw_s_line(int x,int y,int len,unsigned int col);

extern void draw_square(int x,int y,int h,int w,unsigned int col);

extern void draw_circle(int x0,int y0,int r,unsigned int col);

extern void draw_clear(unsigned int col);

#endif
