﻿#pragma once
#include "../../EngineMinimal.h"

enum EPressState
{
	Press,		//按下
	Release,	//松开
};

struct FInputKey
{
	FInputKey();

	string keyName;
	EPressState pressState;

};