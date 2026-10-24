#include "TimelineComponent.h"

FTimeline::FTimeline()
    : timeLength(0.f)
    , currentLeftTime(0.f)
{
    
}

void FTimeline::Tick(float deltaTime)
{
    currentLeftTime -= deltaTime;
    if (currentLeftTime > 0.f)
    {
        if (timelineDelegate.IsBound())
        {
            timelineDelegate.Execute(timeLength, deltaTime);
        }
    }
    else
    {
        currentLeftTime = 0;
        timelineDelegate.ReleaseDelegate();
    }
}

void FTimeline::BindTimelineEvent(float inTime, FTimelineDelegate& inTimelineDelegate)
{
    timeLength = inTime;
    currentLeftTime = inTime;
    timelineDelegate = inTimelineDelegate;
}
