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
}

void FEditorLogSystem::AddLog(const char* fmt, ...)
{
}

void FEditorLogSystem::AddError(const char* fmt, ...)
{
}

void FEditorLogSystem::AddWarning(const char* fmt, ...)
{
}

void FEditorLogSystem::AddSuccess(const char* fmt, ...)
{
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
