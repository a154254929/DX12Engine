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

//���������� �Թ���Ա����
//mshta vbscript:CreateObject("Shell.Application").ShellExecute("%~dp0git_install.exe","1 F:\c\c\20\20.exe F:\c\c\git_install\git_install\icon1.ico test_git1","","runas",1)(window.close)&&exit
//�����ǰ���
/*
#define REG_SHELL_DIRECTORY                    "*\\Shell\\"                        //�ڶ���������Ҽ�
#define REG_DIRECTORY                        "Directory\\Background\\shell\\"    //�ڿհ׵ĵ�������Ҽ� ��Ӱ�ť
#define REG_DESKTOPBACKGROUND                "DesktopBackground\\shell\\"        //�ڿհ׵ĵ�������Ҽ� ��Ӱ�ť

#define REG_PATH_TEST REG_DIRECTORY
int main(int argc, char *argv[])
{
    const char *commit_type = argv[1];
    const char *path_exe = argv[2];
    const char *path_icon = argv[3];
    const char *name = argv[4];

    char buf_reg_key[MAX_PATH] = { 0 };
    char buf_reg_sub_key[MAX_PATH] = { 0 };
    strcpy(buf_reg_key, REG_PATH_TEST);
    strcat(buf_reg_key, name);

    strcpy(buf_reg_sub_key, REG_PATH_TEST);
    strcat(buf_reg_sub_key, name);
    strcat(buf_reg_sub_key, "\\command");

    if (strcmp(commit_type ,"1") == 0) //��װ
    {
        //key
        freg_info info;
        init_feg_info(&info);

        strcpy(info.filename, buf_reg_key);
        info.hkey = HKEY_CLASSES_ROOT;
        {
            info.value[info.size_value].type = REG_SZ;
            strcpy(info.value[info.size_value++].buf, name);

            info.value[info.size_value].type = REG_SZ;
            strcpy(info.value[info.size_value].name, "icon");
            strcpy(info.value[info.size_value++].buf, path_icon);
        }

        if (!register_info(&info))
        {
            printf("\n 1 ��Ҫ����ԱȨ��~~ \n");
            system("pause");

            return 1;
        }

        //sub key
        init_feg_info(&info);
        strcpy(info.filename, buf_reg_sub_key);
        info.hkey = HKEY_CLASSES_ROOT;
        {
            info.value[info.size_value].type = REG_SZ;
            strcpy(info.value[info.size_value++].buf, path_exe);
        }

        if (!register_info(&info))
        {
            printf("\n 2 ��Ҫ����ԱȨ��~~ \n");
            system("pause");

            return 1;
        }
    }
    else if (strcmp(commit_type, "2") == 0) //ж��
    {
        if (delete_register_info(HKEY_CLASSES_ROOT, buf_reg_key))
        {
            delete_reg_key(HKEY_CLASSES_ROOT, REG_PATH_TEST, name);
        }
    }

    return 0;
}
*/

_CRT_BEGIN_C_HEADER
typedef struct
{
    char name[MAX_PATH];
    DWORD type;
    char buf[1024];
}freg_value;

typedef struct
{
    HKEY hkey;
    char filename[MAX_PATH];

    int size_value;
    freg_value value[1024];
}freg_info;

void init_feg_info(freg_info *info);

//ע�� ����Ϣ
bool register_info(const freg_info *info);

//������д������� ����ɾ��ʧ��
bool delete_register_info(HKEY hkey, const char *filename);

//������д������� ����ɾ��ʧ��
bool delete_reg_key(HKEY hkey, const char *filename, const char *subkey_name);
_CRT_END_C_HEADER