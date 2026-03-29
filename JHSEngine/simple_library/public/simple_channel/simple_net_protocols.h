// Copyright (C) RenZhai.2022.All Rights Reserved.
#pragma once
#include "simple_protocols_definition.h"

DEFINITION_SIMPLE_PROTOCOLS(Hello, 0);        //�ͻ��������������hello
DEFINITION_SIMPLE_PROTOCOLS(Challenge, 1)    //��������Ҫ��֤�ͻ���
DEFINITION_SIMPLE_PROTOCOLS(Login, 2)        //�ͻ��˷��͵�½����
DEFINITION_SIMPLE_PROTOCOLS(Welcom, 3)        //����˷��ͻ�ӭ
DEFINITION_SIMPLE_PROTOCOLS(Join, 4)        //�ɹ�

DEFINITION_SIMPLE_PROTOCOLS(Debug,5)        //��Ҫ���ڵ�����Ϣ
DEFINITION_SIMPLE_PROTOCOLS(Failure,6)        //����
DEFINITION_SIMPLE_PROTOCOLS(Upgrade, 7)        //�汾��һ��

DEFINITION_SIMPLE_PROTOCOLS(HeartBeat,8)    //����
DEFINITION_SIMPLE_PROTOCOLS(Close,9)        //�ر����ӵ� Э��