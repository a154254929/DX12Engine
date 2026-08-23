#include "EditorLogSystem.h"

FEditorLogSystem* FEditorLogSystem::logSystemInstance = nullptr;

FEditorLogSystem::FEditorLogSystem()
{
    bAutoScroll = true;
}

FEditorLogSystem::~FEditorLogSystem()
{
}

void FEditorLogSystem::Clear()
{
    textBuffer.clear();
    lineOffsets.clear();
    lineOffsets.push_back(0);
}

void FEditorLogSystem::AddLog(const char* fmt, ...)
{
    int textSize = textBuffer.size();
    
    va_list args;
    va_start(args, fmt);
    textBuffer.appendfv(fmt, args);
    va_end(args);
    
    textBuffer.append("\n");
    
    for (int i = textSize; i < textBuffer.size(); i++)
    {
        if (textBuffer[i] == '\n')
        {
            lineOffsets.push_back(i + 1);
        }
    }
}

void FEditorLogSystem::AddError(const char* fmt, ...)
{
    int textSize = textBuffer.size();
    
    va_list args;
    va_start(args, fmt);
    textBuffer.appendfv(fmt, args);
    va_end(args);
    
    textBuffer.append("\n");
    
    for (int i = textSize; i < textBuffer.size(); i++)
    {
        if (textBuffer[i] == '\n')
        {
            lineOffsets.push_back(i + 1);
        }
    }
}

void FEditorLogSystem::AddWarning(const char* fmt, ...)
{
    int textSize = textBuffer.size();
    
    va_list args;
    va_start(args, fmt);
    textBuffer.appendfv(fmt, args);
    va_end(args);
    
    textBuffer.append("\n");
    
    for (int i = textSize; i < textBuffer.size(); i++)
    {
        if (textBuffer[i] == '\n')
        {
            lineOffsets.push_back(i + 1);
        }
    }
}

void FEditorLogSystem::AddSuccess(const char* fmt, ...)
{
    int textSize = textBuffer.size();
    
    va_list args;
    va_start(args, fmt);
    textBuffer.appendfv(fmt, args);
    va_end(args);
    
    textBuffer.append("\n");
    
    for (int i = textSize; i < textBuffer.size(); i++)
    {
        if (textBuffer[i] == '\n')
        {
            lineOffsets.push_back(i + 1);
        }
    }
}

void FEditorLogSystem::Draw(float deltaTime)
{
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Log", nullptr))
    {
        ImGui::End();
        return;
    }
    
    ImGui::BeginChild("LogScrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
    
    const char* textBufferStat = textBuffer.begin();
    const char* textBufferEnd = textBuffer.end();
    
    {
        ImGuiListClipper clipper;
        clipper.Begin(lineOffsets.Size);
        
        while (clipper.Step())
        {
            for (int lineIndex = clipper.DisplayStart; lineIndex < clipper.DisplayEnd; lineIndex++)
            {
                const char* lineStart = textBufferStat + lineOffsets[lineIndex];
                const char* lineEnd = (lineIndex + 1 < lineOffsets.Size) ? (textBufferStat + lineOffsets[lineIndex + 1] - 1) : textBufferEnd;
                ImGui::TextUnformatted(lineStart, lineEnd);
            }
        }
        
        clipper.End();
    }
    
    ImGui::EndChild();
    ImGui::End();
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
