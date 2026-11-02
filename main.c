#include <stdio.h>
#include "framebuffer.h"
#include "game.h"

#define RED   0x00FF0000
#define GREEN 0x0000FF00
#define BLUE  0x000000FF
#define BLACK 0x00000000
#define WHITE 0x00FFFFFF
#define YELLOW 0x00FFFF00

int main(void)
{
	init_fb("/dev/fb0");
#if 0
    draw_background(0,0,WHITE);	
	draw_point(0, 0, RED);
	draw_h_line(0,0,799,BLACK);
    draw_s_line(0,0,599,BLACK);
    draw_square(0,0,5,5,RED);
    draw_circle(400,300,15,YELLOW);
#endif
    show_chess_broad();
    begin_game();

	return 0;
}
