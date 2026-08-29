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
typedef struct
{
    unsigned int a;
    unsigned int b;
    unsigned int c;
    unsigned int d;
}simple_c_guid;

bool is_guid_valid(simple_c_guid* c_guid);

bool is_guid_valid_str(const char* c_guid);

void normalization_guid(simple_c_guid* c_guid);

void create_guid(simple_c_guid* c_guid);

void create_guid_str(char* c_guid);

void guid_to_string(char *buf, const simple_c_guid * c_guid);

void string_to_guid(const char* buf, simple_c_guid* c_guid);

bool guid_equal(const simple_c_guid * c_guid_a, const simple_c_guid * c_guid_b);

bool guid_equal_str(const char *guid_string, const simple_c_guid * c_guid);
 _CRT_END_C_HEADER
