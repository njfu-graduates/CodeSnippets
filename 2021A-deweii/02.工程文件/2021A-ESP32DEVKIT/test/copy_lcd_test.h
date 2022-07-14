#include <LCDWIKI_GUI.h>   //Core graphics library
#include <LCDWIKI_SPI.h>   //Hardware-specific library
#include <LCDWIKI_TOUCH.h> //touch screen library

//paramters define
#define MODEL ILI9341
#define CS 15
#define CD 4
#define RST 2
#define MOSI 13
#define MISO 12
#define SCK 14
#define LED 25 //if you don't need to control the LED pin,you should set it to -1 and set it to 3.3V

//touch screen paramters define
#define TCS 5
#define TCLK 18
#define TDOUT 19
#define TDIN 23
#define TIRQ 34

#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
//the definiens of software spi mode as follow:
//if the IC model is known or the modules is unreadable,you can use this constructed function
LCDWIKI_SPI my_lcd(MODEL, CS, CD, MISO, MOSI, RST, SCK, LED); //model,cs,dc,miso,mosi,reset,sck,led
//if the IC model is not known and the modules is readable,you can use this constructed function
//LCDWIKI_SPI my_lcd(240,320,CS,CD,MISO,MOSI,RST,SCK,LED); //width,height,cs,dc,miso,mosi,reset,sck,led

//the definiens of touch mode as follow:
LCDWIKI_TOUCH my_touch(TCS, TCLK, TDOUT, TDIN, TIRQ); //tcs,tclk,tdout,tdin,tirq

uint16_t color_mask[] = {0xF800, 0xFFE0, 0x07E0, 0x07FF, 0x001F, 0xF81F}; //color select

#define COLORBOXSIZE my_lcd.Get_Display_Width() / 6
#define PENBOXSIZE my_lcd.Get_Display_Width() / 4

int32_t old_color, current_color;
boolean flag_colour; //是否还在颜色菜单
int16_t old_pen, current_pen;
boolean flag_pen;         //是否还在线条菜单
boolean show_flag = true; //是否可以在下面写

void show_string(char *str, int16_t x, int16_t y, uint8_t csize, uint16_t fc, uint16_t bc, boolean mode)
{
    my_lcd.Set_Text_Mode(mode);
    my_lcd.Set_Text_Size(csize);
    my_lcd.Set_Text_colour(fc);
    my_lcd.Set_Text_Back_colour(bc);
    my_lcd.Print_String(str, x, y);
}

//show color select menu
void show_color_select_menu(void)
{
    uint16_t i;
    for (i = 0; i < 6; i++)
    {
        my_lcd.Set_Draw_color(color_mask[i]);
        my_lcd.Fill_Rectangle(i * COLORBOXSIZE, 0, (i + 1) * COLORBOXSIZE - 1, COLORBOXSIZE / 2 - 1);
    }
    my_lcd.Set_Draw_color(GREEN);
    my_lcd.Fill_Round_Rectangle((my_lcd.Get_Display_Width() - 20) / 3 + 10, COLORBOXSIZE / 2 + 2, (my_lcd.Get_Display_Width() - 20) / 3 * 2 + 10, COLORBOXSIZE / 2 + 20, 5);
    show_string("OK", CENTER, COLORBOXSIZE / 2 + 4, 2, RED, BLACK, 1);
}

//show pen size select menu
void show_pen_size_select_menu(void)
{
    uint16_t i;
    my_lcd.Set_Text_Mode(1);
    my_lcd.Set_Text_Size(2);
    my_lcd.Set_Text_colour(GREEN);
    my_lcd.Set_Text_Back_colour(BLACK);
    for (i = 0; i < 4; i++)
    {
        my_lcd.Print_Number_Int(i + 1, 5 + PENBOXSIZE * i, (COLORBOXSIZE / 2 - 16) / 2, 0, ' ', 10);
        my_lcd.Set_Draw_color(RED);
        my_lcd.Fill_Rectangle(25 + PENBOXSIZE * i, COLORBOXSIZE / 2 / 2 - i, PENBOXSIZE * (i + 1) - 10, COLORBOXSIZE / 2 / 2 + i);
    }
    my_lcd.Set_Draw_color(GREEN);
    my_lcd.Fill_Round_Rectangle((my_lcd.Get_Display_Width() - 20) / 3 + 10, COLORBOXSIZE / 2 + 2, (my_lcd.Get_Display_Width() - 20) / 3 * 2 + 10, COLORBOXSIZE / 2 + 20, 5);
    show_string("OK", CENTER, COLORBOXSIZE / 2 + 4, 2, RED, BLACK, 1);
}

//show main menu
void show_main_menu(void)
{
    my_lcd.Set_Draw_color(YELLOW);
    my_lcd.Fill_Round_Rectangle(5, 0, (my_lcd.Get_Display_Width() - 20) / 3 + 5, COLORBOXSIZE / 2 + 20, 5);
    my_lcd.Fill_Round_Rectangle((my_lcd.Get_Display_Width() - 20) / 3 * 2 + 15, 0, (my_lcd.Get_Display_Width() - 20) / 3 * 3 + 15, COLORBOXSIZE / 2 + 20, 5);
    my_lcd.Set_Draw_color(MAGENTA);
    my_lcd.Fill_Round_Rectangle((my_lcd.Get_Display_Width() - 20) / 3 + 10, 0, (my_lcd.Get_Display_Width() - 20) / 3 * 2 + 10, COLORBOXSIZE / 2 + 20, 5);
    show_string("COLOUR", 8 + ((my_lcd.Get_Display_Width() - 20) / 3 - 72) / 2 - 1, ((COLORBOXSIZE / 2 + 20) - 16) / 2, 2, BLUE, BLACK, 1);
    show_string("CLEAR", (my_lcd.Get_Display_Width() - 20) / 3 + 10 + ((my_lcd.Get_Display_Width() - 20) / 3 - 60) / 2 - 1, ((COLORBOXSIZE / 2 + 20) - 16) / 2, 2, WHITE, BLACK, 1);
    show_string("PEN", (my_lcd.Get_Display_Width() - 20) / 3 * 2 + 15 + ((my_lcd.Get_Display_Width() - 20) / 3 - 36) / 2 - 1, ((COLORBOXSIZE / 2 + 20) - 16) / 2, 2, BLUE, BLACK, 1);
}