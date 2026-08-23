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

void FEditorLogSystem::Draw(const char* title, bool* p_open)
{
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
