#pragma once
//Copyright (C) RenZhai.2019.All Rights Reserved.
//���� ��լ
//��c�����ϸԴ�뽲����AboutCG ���ְ�������Ӳ��c���ԡ���Ƶ�������棺
//https://www.aboutcg.org/courseDetails/902/introduce
//ϣ��ѧϰ�������漼�� ������UE4��Ϸ, ���Է���������ַ��
//https://zhuanlan.zhihu.com/p/60117613
//
//bibi���Կ���������լϵ�н̳̽��ܣ�
//https://space.bilibili.com/29544409
//
//���˲�����վ
//http://renzhai.net
//
//���ڱ��װ�������ϸ���� :
//���ְ汾(��ϸ)��
//https://zhuanlan.zhihu.com/p/144558934
//��Ƶ�汾��
//https://www.bilibili.com/video/BV1x5411s7s3
#include "../simple_c_core/simple_core_minimal.h"

_CRT_BEGIN_C_HEADER
typedef enum 
{
    SIMPLE_BLACK = 0,                // ��ɫ        
    SIMPLE_BLUE,                    // ��ɫ        
    SIMPLE_GREEN,                    // ��ɫ        
    SIMPLE_LIGHT_BLUE,                // ǳ��ɫ    
    SIMPLE_RED,                        // ��ɫ        
    SIMPLE_VIOLET,                    // ��ɫ        
    SIMPLE_YELLOW,                    // ��ɫ        
    SIMPLE_WHITE,                    // ��ɫ        
    SIMPLE_GREY,                    // ��ɫ
    SIMPLE_PALE_BLUE,                // ����ɫ
    SIMPLE_PALE_GREEN,                // ����ɫ
    SIMPLE_LIGHT_GREEN,                // ��ǳ��ɫ
    SIMPLE_PALE_RED,                // ����ɫ
    SIMPLE_LAVENDER,                // ����ɫ
    SIMPLE_CANARY_YELLOW,            // ����ɫ
    SIMPLE_BRIGHT_WHITE,            // ����ɫ
}simple_console_w_color;

//set_console_windows_color
void set_console_w_color(simple_console_w_color font_color, simple_console_w_color background_color);
_CRT_END_C_HEADER