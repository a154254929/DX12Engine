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
#define _number_of_successful_conversions(a) a
#define _out_pram(a) a
#define _in_pram(a) a

typedef struct
{
    int index;
    char paths[2048][512];//MAX_PATH
}def_c_paths;

typedef struct
{
    int index;
    wchar_t paths[2048][512];//MAX_PATH
}def_c_paths_w;

void init_def_c_paths(def_c_paths *c_paths);
void init_def_c_paths_w(def_c_paths_w* c_paths);

int copy_file(char *Src, char *Dest);

void find_files(char const *in_path, def_c_paths *str,bool b_recursion);

bool is_file_exists(char const* filename);

bool create_file(char const *filename);
 
//����·��
bool create_file_directory(char const *in_path);

//�򿪵�ַ
bool open_url(const char* url);

//ͨ��������url
bool open_url_by_param(const char* url, const char* param);

//ͨ��������ĳ������
bool open_by_operation(const char* in_operation, const char* url, const char* param);

//��һ���ļ���
bool open_explore(const char* url);

//ʹ�øýӿ� һ��Ҫ��ʼ��buf
bool get_file_buf(const char *path,char *buf);

bool save_file_buff(const char* path, char* buf);

bool add_file_buf(const char *path, char *buf);

//������������ַ����ķ�ʽ�洢�������������0 �Զ��ضϣ������ö����ƴ洢
bool add_new_file_buf(const char *path, char *buf);

//������Զ����Ʒ�ʽ��ȡ
bool load_data_from_disk(const char* path, char* buf);

unsigned int get_file_size_by_filename(const char *filename);

unsigned int get_file_size(FILE *file_handle);

//������Զ����Ʒ�ʽ�洢������������0�Զ��ضϵ����
bool save_data_to_disk(const char* path, char* buf, int buf_size);
//���ַ���խ�ַ�
// 
//���ַ�תխ�ַ�
_number_of_successful_conversions(size_t) wchar_t_to_char(
    _out_pram(char*) dst_char,
    size_t char_size,
    _in_pram(wchar_t const*) _Src);

//խ�ַ�ת���ַ�
_number_of_successful_conversions(size_t) char_to_wchar_t(
    _out_pram(wchar_t*) dst_wchar_t,
    size_t wchar_t_size,
    _in_pram(char const*) _Src);

//���ַ�
//////////////////////////////////////////////
//������������ַ����ķ�ʽ�洢�������������0 �Զ��ضϣ������ö����ƴ洢
bool add_new_file_buf_w(const wchar_t* path, char* buf);

bool get_file_buf_w(const wchar_t* path, char* buf);

//������Զ����Ʒ�ʽ�洢������������0�Զ��ضϵ����
bool save_data_to_disk_w(const wchar_t* path, char* buf,int buf_size);

//������Զ����Ʒ�ʽ��ȡ buf�Ĵ�СҪ��ʵ�ʴ�С+1 ��Ϊ���һλ����/0
bool load_data_from_disk_w(const wchar_t* path, char* buf);

bool is_file_exists_w(const wchar_t *filename);

//�򿪵�ַ
bool open_url_w(const wchar_t* url);

bool open_url_by_param_w(const wchar_t* url,const wchar_t *param);

bool open_by_operation_w(const wchar_t *in_operation, const wchar_t* url, const wchar_t* param);

//��һ���ļ���
bool open_explore_w(const wchar_t* url);

unsigned int get_file_size_by_filename_w(const wchar_t* filename);
_CRT_END_C_HEADER