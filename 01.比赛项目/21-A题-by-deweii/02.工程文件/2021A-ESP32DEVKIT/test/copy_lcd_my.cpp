
#include "main.h"

void setup(void)
{
    Screen_Init();
    Init();

    show_main_menu_start();
}

void loop()
{
    // again:
    touch.TP_Scan(0);
    if (touch.TP_Get_State() & TP_PRES_DOWN)
    {
        //按键检测在菜单栏
        if (touch.x < MENU_WIDTH)
        {
            //
            if (touch.y < MENU_BEGIN_Y + MENU_HEIGHT)
            {
                flag_menu = MENU_MAIN;
            }
            else if (touch.y < MENU_BEGIN_Y + MENU_HEIGHT * 2)
            {
                flag_menu = MENU_1;
            }
            else if (touch.y < MENU_BEGIN_Y + MENU_HEIGHT * 3)
            {
                flag_menu = MENU_2;
            }
            else if (touch.y < MENU_BEGIN_Y + MENU_HEIGHT * 4)
            {
                flag_menu = MENU_3;
            }
            else if (touch.y < MENU_BEGIN_Y + MENU_HEIGHT * 5)
            {
                flag_menu = MENU_4;
            }
            else
            {
                flag_menu = MENU_SET;
            }
            //
            if (flag_old_menu != flag_menu)
            {
                show_main_menu();
                flag_old_menu = flag_menu;

                if (flag_menu == MENU_MAIN)
                {
                    show_deweii();
                }

                if (flag_menu == MENU_1) //car mode
                {
                    show_menu_1();
                }

                if (flag_menu == MENU_2) //cam mode
                {
                    show_menu_2();
                }

                if (flag_menu == MENU_3)
                {
                    show_menu_3();
                }

                if (flag_menu == MENU_4)
                {
                    show_menu_4();
                }

                if (flag_menu == MENU_SET)
                {
                    show_menu_set();
                }
            }
        }
        //
        else
        {
            //menu_1
            if (flag_menu == MENU_1)
            {
                //select
                if (touch.x > LCD_WIDTH - 2 * MENU_WIDTH)
                {
                    //mode 0
                    if (touch.y < MENU_HEIGHT)
                    {
                        show_menu_1_mode(0);
                    }
                    else if (touch.y < MENU_HEIGHT * 2)
                    {
                        flag_menu_1_mode1++;
                        if (flag_menu_1_mode1 == flag_menu_1_mode1_num)
                            flag_menu_1_mode1 = 0;

                        show_menu_1_mode(1);
                    }
                    else if (touch.y < MENU_HEIGHT * 3)
                    {
                        flag_menu_1_mode2++;
                        if (flag_menu_1_mode2 == flag_menu_1_mode2_num)
                            flag_menu_1_mode2 = 0;

                        show_menu_1_mode(2);
                    }
                    else if (touch.y < MENU_HEIGHT * 4)
                    {
                        flag_menu_1_mode3++;
                        if (flag_menu_1_mode3 == flag_menu_1_mode3_num)
                            flag_menu_1_mode3 = 0;

                        show_menu_1_mode(3);
                    }
                    else if (touch.y < MENU_HEIGHT * 5)
                    {
                    }
                    else if (touch.y < MENU_HEIGHT * 6)
                    {
                    }
                    else if (touch.y < MENU_HEIGHT * 7)
                    {
                    }
                    else
                    {
                    }
                }
            }
            //touch
            if (flag_menu == MENU_2)
            {
            }
            //touch adc mode
            if (flag_menu == MENU_3)
            {
                if (touch.x > LCD_WIDTH - 2 * MENU_WIDTH)
                {
                    if (touch.y < MENU_HEIGHT)
                    {
                        flag_adc_rolling = !flag_adc_rolling;
                        avoid_touch();
                        if (flag_adc_rolling)
                        {
                            lcd.Set_Draw_color(BLACK);
                            lcd.Fill_Rectangle(S_ADC_BEGIN_X + S_ADC_S, S_ADC_BEGIN_Y,
                                               S_ADC_BEGIN_X + S_ADC_S, S_ADC_BEGIN_Y + S_ADC_BEGIN_Y_I * 4);
                            show_menu_3();
                        }
                    }
                }
            }
            //touch dac mode
            if (flag_menu == MENU_4)
            {
                if (touch.y < MENU_HEIGHT)
                {
                }
                else if (touch.y < MENU_HEIGHT * 2)
                {
                    if (touch.x < LCD_WIDTH - 2 * MENU_WIDTH + MENU_INTERVAL / 2)
                    {
                        show_menu_4_mode(--flag_menu_4_mdoe1);
                    }
                    else
                    {
                        show_menu_4_mode(++flag_menu_4_mdoe1);
                    }
                }
                else
                {
                }
            }

            if (flag_menu == MENU_SET)
            {
            }

            avoid_touch();
        }
    }

    if (flag_menu == MENU_SET)
    {
        show_menu_set_mode(); //变化的部分
    }

    if (flag_menu == MENU_3)
    {
        show_menu_3_mode();
    }
}