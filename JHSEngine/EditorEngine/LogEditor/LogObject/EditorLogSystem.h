#pragma once
#include "../../../imgui/imgui.h"

class FEditorLogSystem
{
    friend class FLogEditor;
public:
    FEditorLogSystem();
    ~FEditorLogSystem();
    
    static FEditorLogSystem* GetInstance();
    static void Destroy();
public:
    void Clear();
    void AddLog(const char* fmt, ...);
    void AddError(const char* fmt, ...);
    void AddWarning(const char* fmt, ...);
    void AddSuccess(const char* fmt, ...);
    
protected:
    void Draw(float deltaTime);
    
protected:
    ImGuiTextBuffer textBuffer;
    ImGuiTextFilter textFilter;
    ImVector<int> lineOffsets; 
    bool bAutoScroll;
    
protected:
    static FEditorLogSystem* logSystemInstance;
};
