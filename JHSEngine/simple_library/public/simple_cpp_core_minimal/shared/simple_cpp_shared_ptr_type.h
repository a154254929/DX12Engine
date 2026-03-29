// Copyright (C) RenZhai.2022.All Rights Reserved.
#pragma once

//���ü���
class FRefCounter
{
public:
    FRefCounter()
        :SharedCount(0)
        ,WeakCount(0)
    {

    }

    bool operator>(const int &InCount)
    {
        return SharedCount > InCount;
    }

    bool operator<(const int& InCount)
    {
        return SharedCount < InCount;
    }

    int SharedCount;
    int WeakCount;
};