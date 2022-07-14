// IMPORTANT: LCDWIKI_SPI LIBRARY AND LCDWIKI_TOUCH LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.

//This program is a demo of drawing

//when using the BREAKOUT BOARD only and using these hardware spi lines to the LCD,
//the SDA pin and SCK pin is defined by the system and can't be modified.
//if you don't need to control the LED pin,you can set it to 3.3V and set the pin definition to -1.
//other pins can be defined by youself,for example
//pin usage as follow:
//             CS  DC/RS  RESET  SDI/MOSI  SDO/MISO  SCK  LED    VCC     GND
//Arduino Uno  15   4     2      13        12        14   25   5V/3.3V   GND
//            T_IRQ  T_DO  T_DIN  T_CS  T_CLK
//Arduino Uno  34     19      23     5    18

//Remember to set the pins to suit your display module!

/*********************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE 
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**********************************************************************************/
#include <Arduino.h>
#include "main.h"

void setup(void)
{
    my_lcd.Init_LCD();
    my_lcd.Set_Rotation(3);
    my_touch.TP_Set_Rotation(3);
    my_touch.TP_Init(my_lcd.Get_Rotation(), my_lcd.Get_Display_Width(), my_lcd.Get_Display_Height());
    my_lcd.Fill_Screen(BLACK);
    show_main_menu();
    current_color = RED;
    current_pen = 0;
}

void loop()
{
comme:
    my_touch.TP_Scan(0); //这个函数会改变touch_statue的状态
    if (my_touch.TP_Get_State() & TP_PRES_DOWN)
    {
        if (my_touch.y < COLORBOXSIZE / 2 + 20) //进入菜单的选择
        {
            if (((my_touch.x > 5) && (my_touch.x < ((my_lcd.Get_Display_Width() - 20) / 3 + 5))) && !flag_pen) //select color
            {
                flag_colour = 1; //进入选择颜色的菜单
                if (show_flag)
                {
                    my_lcd.Set_Draw_color(BLACK);
                    my_lcd.Fill_Rectangle(0, 0, my_lcd.Get_Display_Width() - 1, COLORBOXSIZE / 2 + 20);
                    show_color_select_menu();
                }
                show_flag = false;
                switch (current_color)
                {
                case RED:
                {
                    my_lcd.Set_Draw_color(WHITE);
                    my_lcd.Draw_Rectangle(0, 0, COLORBOXSIZE - 1, COLORBOXSIZE / 2 - 1);
                    break;
                }
                case YELLOW:
                {
                    my_lcd.Set_Draw_color(WHITE);
                    my_lcd.Draw_Rectangle(COLORBOXSIZE, 0, 2 * COLORBOXSIZE - 1, COLORBOXSIZE / 2 - 1);
                    break;
                }
                case GREEN:
                {
                    my_lcd.Set_Draw_color(WHITE);
                    my_lcd.Draw_Rectangle(2 * COLORBOXSIZE, 0, 3 * COLORBOXSIZE - 1, COLORBOXSIZE / 2 - 1);
                    break;
                }
                case CYAN:
                {
                    my_lcd.Set_Draw_color(WHITE);
                    my_lcd.Draw_Rectangle(3 * COLORBOXSIZE, 0, 4 * COLORBOXSIZE - 1, COLORBOXSIZE / 2 - 1);
                    break;
                }
                case BLUE:
                {
                    my_lcd.Set_Draw_color(WHITE);
                    my_lcd.Draw_Rectangle(4 * COLORBOXSIZE, 0, 5 * COLORBOXSIZE - 1, COLORBOXSIZE / 2 - 1);
                    break;
                }
                case MAGENTA:
                {
                    my_lcd.Set_Draw_color(WHITE);
                    my_lcd.Draw_Rectangle(5 * COLORBOXSIZE, 0, 6 * COLORBOXSIZE - 1, COLORBOXSIZE / 2 - 1);
                    break;
                }
                default:
                    break;
                }
                //画选中的框框
            }

            if (flag_colour)
            {
                if (my_touch.y < COLORBOXSIZE / 2) //
                {
                    old_color = current_color;
                    if (my_touch.x < COLORBOXSIZE)
                    {
                        current_color = RED;
                        my_lcd.Set_Draw_color(WHITE);
                        my_lcd.Draw_Rectangle(0, 0, COLORBOXSIZE - 1, COLORBOXSIZE / 2 - 1);
                    }
                    else if (my_touch.x < COLORBOXSIZE * 2)
                    {
                        current_color = YELLOW;
                        my_lcd.Set_Draw_color(WHITE);
                        my_lcd.Draw_Rectangle(COLORBOXSIZE, 0, 2 * COLORBOXSIZE - 1, COLORBOXSIZE / 2 - 1);
                    }
                    else if (my_touch.x < COLORBOXSIZE * 3)
                    {
                        current_color = GREEN;
                        my_lcd.Set_Draw_color(WHITE);
                        my_lcd.Draw_Rectangle(2 * COLORBOXSIZE, 0, 3 * COLORBOXSIZE - 1, COLORBOXSIZE / 2 - 1);
                    }
                    else if (my_touch.x < COLORBOXSIZE * 4)
                    {
                        current_color = CYAN;
                        my_lcd.Set_Draw_color(WHITE);
                        my_lcd.Draw_Rectangle(3 * COLORBOXSIZE, 0, 4 * COLORBOXSIZE - 1, COLORBOXSIZE / 2 - 1);
                    }
                    else if (my_touch.x < COLORBOXSIZE * 5)
                    {
                        current_color = BLUE;
                        my_lcd.Set_Draw_color(WHITE);
                        my_lcd.Draw_Rectangle(4 * COLORBOXSIZE, 0, 5 * COLORBOXSIZE - 1, COLORBOXSIZE / 2 - 1);
                    }
                    else if (my_touch.x < COLORBOXSIZE * 6)
                    {
                        current_color = MAGENTA;
                        my_lcd.Set_Draw_color(WHITE);
                        my_lcd.Draw_Rectangle(5 * COLORBOXSIZE, 0, 6 * COLORBOXSIZE - 1, COLORBOXSIZE / 2 - 1);
                    }

                    if (old_color != current_color) //原来选中的框框消失
                    {
                        switch (old_color)
                        {
                        case RED:
                        {
                            my_lcd.Set_Draw_color(RED);
                            my_lcd.Draw_Rectangle(0, 0, COLORBOXSIZE - 1, COLORBOXSIZE / 2 - 1);
                            break;
                        }
                        case YELLOW:
                        {
                            my_lcd.Set_Draw_color(YELLOW);
                            my_lcd.Draw_Rectangle(COLORBOXSIZE, 0, 2 * COLORBOXSIZE - 1, COLORBOXSIZE / 2 - 1);
                            break;
                        }
                        case GREEN:
                        {
                            my_lcd.Set_Draw_color(GREEN);
                            my_lcd.Draw_Rectangle(2 * COLORBOXSIZE, 0, 3 * COLORBOXSIZE - 1, COLORBOXSIZE / 2 - 1);
                            break;
                        }
                        case CYAN:
                        {
                            my_lcd.Set_Draw_color(CYAN);
                            my_lcd.Draw_Rectangle(3 * COLORBOXSIZE, 0, 4 * COLORBOXSIZE - 1, COLORBOXSIZE / 2 - 1);
                            break;
                        }
                        case BLUE:
                        {
                            my_lcd.Set_Draw_color(BLUE);
                            my_lcd.Draw_Rectangle(4 * COLORBOXSIZE, 0, 5 * COLORBOXSIZE - 1, COLORBOXSIZE / 2 - 1);
                            break;
                        }
                        case MAGENTA:
                        {
                            my_lcd.Set_Draw_color(MAGENTA);
                            my_lcd.Draw_Rectangle(5 * COLORBOXSIZE, 0, 6 * COLORBOXSIZE - 1, COLORBOXSIZE / 2 - 1);
                            break;
                        }
                        default:
                            break;
                        }
                    }
                }
                else if (my_touch.y < COLORBOXSIZE / 2 + 20) //选择完成 开始画画
                {
                    if ((my_touch.x > (my_lcd.Get_Display_Width() - 20) / 3 + 10) && (my_touch.x < (my_lcd.Get_Display_Width() - 20) / 3 * 2 + 10))
                    {
                        my_lcd.Set_Draw_color(BLACK);
                        my_lcd.Fill_Rectangle(0, 0, my_lcd.Get_Display_Width() - 1, COLORBOXSIZE / 2 + 20);
                        show_main_menu();
                        flag_colour = 0;
                        show_flag = true;
                        goto comme;
                    }
                }
            }

            if ((my_touch.x > ((my_lcd.Get_Display_Width() - 20) / 3 * 2 + 15)) && (my_touch.x < (((my_lcd.Get_Display_Width() - 20) / 3 * 3 + 15))) && !flag_colour) //select pen size
            {
                flag_pen = 1;
                if (show_flag)
                {
                    my_lcd.Set_Draw_color(BLACK);
                    my_lcd.Fill_Rectangle(0, 0, my_lcd.Get_Display_Width() - 1, COLORBOXSIZE / 2 + 20);
                    show_pen_size_select_menu();
                }
                show_flag = false;
                switch (current_pen)
                {
                case 0:
                {
                    my_lcd.Set_Draw_color(WHITE);
                    my_lcd.Draw_Rectangle(0, 0, PENBOXSIZE - 1, COLORBOXSIZE / 2 - 1);
                    break;
                }
                case 1:
                {
                    my_lcd.Set_Draw_color(WHITE);
                    my_lcd.Draw_Rectangle(PENBOXSIZE, 0, 2 * PENBOXSIZE - 1, COLORBOXSIZE / 2 - 1);
                    break;
                }
                case 2:
                {
                    my_lcd.Set_Draw_color(WHITE);
                    my_lcd.Draw_Rectangle(2 * PENBOXSIZE, 0, 3 * PENBOXSIZE - 1, COLORBOXSIZE / 2 - 1);
                    break;
                }
                case 3:
                {
                    my_lcd.Set_Draw_color(WHITE);
                    my_lcd.Draw_Rectangle(3 * PENBOXSIZE, 0, 4 * PENBOXSIZE - 1, COLORBOXSIZE / 2 - 1);
                    break;
                }
                default:
                    break;
                }
            }

            if (flag_pen)
            {
                if (my_touch.y < COLORBOXSIZE / 2)
                {
                    old_pen = current_pen;
                    if (my_touch.x < PENBOXSIZE)
                    {
                        current_pen = 0;
                        my_lcd.Set_Draw_color(WHITE);
                        my_lcd.Draw_Rectangle(0, 0, PENBOXSIZE - 1, COLORBOXSIZE / 2 - 1);
                    }
                    else if (my_touch.x < 2 * PENBOXSIZE)
                    {
                        current_pen = 1;
                        my_lcd.Set_Draw_color(WHITE);
                        my_lcd.Draw_Rectangle(PENBOXSIZE, 0, 2 * PENBOXSIZE - 1, COLORBOXSIZE / 2 - 1);
                    }
                    else if (my_touch.x < 3 * PENBOXSIZE)
                    {
                        current_pen = 2;
                        my_lcd.Set_Draw_color(WHITE);
                        my_lcd.Draw_Rectangle(2 * PENBOXSIZE, 0, 3 * PENBOXSIZE - 1, COLORBOXSIZE / 2 - 1);
                    }
                    else if (my_touch.x < 4 * PENBOXSIZE)
                    {
                        current_pen = 3;
                        my_lcd.Set_Draw_color(WHITE);
                        my_lcd.Draw_Rectangle(3 * PENBOXSIZE, 0, 4 * PENBOXSIZE - 1, COLORBOXSIZE / 2 - 1);
                    }

                    if (old_pen != current_pen)
                    {
                        switch (old_pen)
                        {
                        case 0:
                        {
                            my_lcd.Set_Draw_color(BLACK);
                            my_lcd.Draw_Rectangle(0, 0, PENBOXSIZE - 1, COLORBOXSIZE / 2 - 1);
                            break;
                        }
                        case 1:
                        {
                            my_lcd.Set_Draw_color(BLACK);
                            my_lcd.Draw_Rectangle(PENBOXSIZE, 0, 2 * PENBOXSIZE - 1, COLORBOXSIZE / 2 - 1);
                            break;
                        }
                        case 2:
                        {
                            my_lcd.Set_Draw_color(BLACK);
                            my_lcd.Draw_Rectangle(2 * PENBOXSIZE, 0, 3 * PENBOXSIZE - 1, COLORBOXSIZE / 2 - 1);
                            break;
                        }
                        case 3:
                        {
                            my_lcd.Set_Draw_color(BLACK);
                            my_lcd.Draw_Rectangle(3 * PENBOXSIZE, 0, 4 * PENBOXSIZE - 1, COLORBOXSIZE / 2 - 1);
                            break;
                        }
                        default:
                            break;
                        }
                    }
                }
                else if (my_touch.y < COLORBOXSIZE / 2 + 20)
                {
                    if ((my_touch.x > (my_lcd.Get_Display_Width() - 20) / 3 + 10) && (my_touch.x < (my_lcd.Get_Display_Width() - 20) / 3 * 2 + 10))
                    {
                        my_lcd.Set_Draw_color(BLACK);
                        my_lcd.Fill_Rectangle(0, 0, my_lcd.Get_Display_Width() - 1, COLORBOXSIZE / 2 + 20);
                        show_main_menu();
                        flag_pen = 0;
                        show_flag = true;
                        goto comme;
                    }
                }
            }

            if (((my_touch.x > ((my_lcd.Get_Display_Width() - 20) / 3 + 10)) && (my_touch.x < ((my_lcd.Get_Display_Width() - 20) / 3 * 2 + 10))) && !flag_colour && !flag_pen)
            {
                my_lcd.Set_Draw_color(BLACK);
                my_lcd.Fill_Rectangle(0, COLORBOXSIZE / 2 + 21, my_lcd.Get_Display_Width() - 1, my_lcd.Get_Display_Height() - 1);
            }
        }

        if (((my_touch.y - current_pen) > COLORBOXSIZE / 2 + 20) && ((my_touch.y + current_pen) < my_lcd.Get_Display_Height())) //drawing
        {
            my_lcd.Set_Draw_color(current_color);
            my_lcd.Fill_Circle(my_touch.x, my_touch.y, current_pen);
        }
    }
}