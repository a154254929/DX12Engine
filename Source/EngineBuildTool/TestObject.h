//Copyright (C) RenZhai.2022.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "../Common/Core/GamePlay/PlayerStateBase.h"
#include "../../MMOARPGGameType.h"
#include "MMOARPGType.h"
#include "MMOARPGPlayerState.generated.h"

/**
 *
 */
UCLASS()
class MMOARPG_API AMMOARPGPlayerState : public APlayerStateBase
{
	GENERATED_BT_BODY()

	JPROPERTY(CodeType = Resources)
	int Num;

	JPROPERTY(CodeType = Resources)
	bool PlayerState;

public:
	JFUNCTION(CodeType = Event)
	void Hello(int c,float b);

	JFUNCTION(CodeType = Describe)
	void Hello1();
};
