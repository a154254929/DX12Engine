#pragma once
#include "../EngineMinimal.h"

//总长度 间隔时间
DEFINITION_SIMPLE_SINGLE_DELEGATE(FTimelineDelegate, void, float, float);

struct FTimeline
{
    FTimeline();
    
    void Tick(float deltaTime);
    
    void BindTimelineEvent(float inTime, FTimelineDelegate& inTimelineDelegate);
private:
    float timeLength;
    float currentLeftTime;
    
    FTimelineDelegate timelineDelegate;
};