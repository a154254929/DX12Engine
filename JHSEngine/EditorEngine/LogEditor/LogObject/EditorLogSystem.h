#pragma once
#include "../../../imgui/imgui.h"
#include "../../../simple_library/public/simple_library.h"

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
    void HandleBackstageLog(e_error InColorID,int InOldSize);
    void AddLog(const char* fmt, ...);
    void AddError(const char* fmt, ...);
    void AddWarning(const char* fmt, ...);
    void AddSuccess(const char* fmt, ...);
    
protected:
    void Draw(float deltaTime);
    
    void AddLineColor(e_error inColor);
    void ResetLineOffsets(e_error inColor,int inOldSize);
    ImVec4 GetColor(e_error inColorID);
    
protected:
    ImGuiTextBuffer textBuffer;
    ImGuiTextFilter textFilter;
    ImVector<int> lineOffsets; 
    ImVector<e_error>	preLineColor;
    bool bAutoScroll;
    
protected:
    static FEditorLogSystem* logSystemInstance;
};
