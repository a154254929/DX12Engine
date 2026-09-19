#include "EditorLogSystem.h"

#include "../../../Engine/Debug/Log/SimpleLog.h"

FEditorLogSystem* FEditorLogSystem::logSystemInstance = nullptr;

FEditorLogSystem::FEditorLogSystem()
{
    bAutoScroll = true;
    Clear();
}

FEditorLogSystem::~FEditorLogSystem()
{
}

void FEditorLogSystem::Clear()
{
    textBuffer.clear();
    lineOffsets.clear();
    lineOffsets.push_back(0);
    preLineColor.clear();
}

void FEditorLogSystem::HandleBackstageLog(e_error InColorID, int InOldSize)
{
    char error_str[64] = { 0 };
    get_error_str(InColorID, error_str);

    textBuffer.append(error_str);

    ResetLineOffsets(InColorID,InOldSize);
}

void FEditorLogSystem::AddLog(const char* fmt, ...)
{
    const int oldSize = textBuffer.size();

    va_list args;
    va_start(args, fmt);
    textBuffer.appendfv(fmt, args);
    va_end(args);
    
    HandleBackstageLog(SIMPLE_C_LOG, oldSize);
}

void FEditorLogSystem::AddError(const char* fmt, ...)
{
    const int oldSize = textBuffer.size();

    va_list args;
    va_start(args, fmt);
    textBuffer.appendfv(fmt, args);
    va_end(args);
    
    HandleBackstageLog(SIMPLE_C_ERROR, oldSize);
}

void FEditorLogSystem::AddWarning(const char* fmt, ...)
{
    const int oldSize = textBuffer.size();

    va_list args;
    va_start(args, fmt);
    textBuffer.appendfv(fmt, args);
    va_end(args);
    
    HandleBackstageLog(SIMPLE_C_WARNING, oldSize);
}

void FEditorLogSystem::AddSuccess(const char* fmt, ...)
{
    const int oldSize = textBuffer.size();

    va_list args;
    va_start(args, fmt);
    textBuffer.appendfv(fmt, args);
    va_end(args);
    
    HandleBackstageLog(SIMPLE_C_SUCCESS, oldSize);
}

void FEditorLogSystem::Draw(float deltaTime)
{
    if (lineOffsets.Size != preLineColor.Size + 1)
    {
        return;
    }
    
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Log", nullptr))
    {
        ImGui::End();
        return;
    }
    
    if (ImGui::Button("Clear"))
    {
        Clear();
    }
    
    ImGui::SameLine();
    if (ImGui::Button("CopyAll"))
    {
        ImGui::LogToClipboard();
    }
    
    ImGui::SameLine();
    ImGui::Checkbox("AutoScroll", &bAutoScroll);
    
    ImGui::SameLine();
    textFilter.Draw("LogFilter", 180);
    
    ImGui::Separator();
    
    ImGui::BeginChild("LogScrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
    
    const char* textBufferStat = textBuffer.begin();
    const int lineCount = preLineColor.Size;
    
    if (textFilter.IsActive())
    {
        for (int lineIndex = 0; lineIndex < lineCount; lineIndex++)
        {
            const char* lineStart = textBufferStat + lineOffsets[lineIndex];
            const char* lineEnd = textBufferStat + lineOffsets[lineIndex + 1] - 1;
            
            if (textFilter.PassFilter(lineStart, lineEnd))
            {
                ImGui::PushStyleColor(ImGuiCol_Text, GetColor(preLineColor[lineIndex]));
                ImGui::TextUnformatted(lineStart, lineEnd);
                ImGui::PopStyleColor();
            }
        }
    }
    else
    {
        ImGuiListClipper clipper;
        clipper.Begin(lineCount);
        
        while (clipper.Step())
        {
            for (int lineIndex = clipper.DisplayStart; lineIndex < clipper.DisplayEnd; lineIndex++)
            {
                const char* lineStart = textBufferStat + lineOffsets[lineIndex];
                const char* lineEnd = textBufferStat + lineOffsets[lineIndex + 1] - 1;
                ImGui::PushStyleColor(ImGuiCol_Text, GetColor(preLineColor[lineIndex]));
                ImGui::TextUnformatted(lineStart, lineEnd);
                ImGui::PopStyleColor();
            }
        }
        
        clipper.End();
    }
    
    if (bAutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
    {
        ImGui::SetScrollHereY(1.0f);
    }
    
    ImGui::EndChild();
    ImGui::End();
}

void FEditorLogSystem::AddLineColor(e_error inColor)
{
    preLineColor.push_back(inColor);
}

void FEditorLogSystem::ResetLineOffsets(e_error inColor, int inOldSize)
{
    
    textBuffer.append("\n");
    
    for (int i = inOldSize; i < textBuffer.size(); i++)
    {
        if (textBuffer[i] == '\n')
        {
            lineOffsets.push_back(i + 1);
            
            AddLineColor(inColor);
        }
    }
}

FEditorLogSystem* FEditorLogSystem::GetInstance()
{
    if (!logSystemInstance)
    {
        logSystemInstance = new FEditorLogSystem();
    }
    return logSystemInstance;
}

void FEditorLogSystem::Destroy()
{
    if (logSystemInstance)
    {
        delete logSystemInstance;
        logSystemInstance = nullptr;
    }
}

ImVec4 FEditorLogSystem::GetColor(e_error inColorID)
{
    switch (inColorID)
    {
    case SIMPLE_C_LOG:
        return ImVec4(1.f, 1.f, 1.f, 1.f);
    case SIMPLE_C_ERROR:
        return ImVec4(1.f, 0.f, 0.f, 1.f);
    case SIMPLE_C_WARNING:
        return ImVec4(1.f, 1.f, 0.f, 1.f);
    case SIMPLE_C_SUCCESS:
        return ImVec4(0.f, 1.f, 0.f, 1.f);
    }

    return ImVec4(0.4f, 0.4f, 0.4f, 1.f);
}
