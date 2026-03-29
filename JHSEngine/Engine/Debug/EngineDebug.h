#pragma once
#include <stdio.h>

#define ANALYSIS_HRESULT(inValue)\
{\
    HRESULT result = inValue;\
    if (FAILED(result))\
    {\
        char _errorBuf[512];\
        sprintf_s(_errorBuf, 512, "[%s:%d]\nHRESULT Failed: %s\nCode: 0x%08X (%i)", __FILE__, __LINE__, #inValue, (unsigned int)result, (int)result);\
        Engine_Log_Error("%s", _errorBuf);\
        MessageBoxA(NULL, _errorBuf, "ANALYSIS_HRESULT Error", MB_OK | MB_ICONERROR);\
        assert(0);\
    }\
}