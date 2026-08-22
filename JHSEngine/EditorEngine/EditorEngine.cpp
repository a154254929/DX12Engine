#include "EditorEngine.h"

#include "ToolbarEditor/ToolbarEditor.h"
#include "LogEditor/LogEditor.h"

CEditorEngine::CEditorEngine()
{
    toolbarEditor = new FToolbarEditor();
    logEditor = new FLogEditor();
}

int CEditorEngine::PreInit(
#if defined(_WIN32)
    FWinMainCommandParameters inParameters
#endif
)
{
    return 0;
}

int CEditorEngine::Init(
#if defined(_WIN32)
    FWinMainCommandParameters inParameters
#endif
)
{
    return 0;
}

int CEditorEngine::PostInit()
{
    return 0;
}

int CEditorEngine::PreExit()
{
    return 0;
}

int CEditorEngine::Exit()
{
    return 0;
}

int CEditorEngine::PostExit()
{
    return 0;
}

void CEditorEngine::BuildEditor()
{
    toolbarEditor->BuildEditor();
    logEditor->BuildEditor();
}

void CEditorEngine::DrawEditor(float deltaTime)
{
    /*
    bool show_demo_window = true;
    ImGui::ShowDemoWindow(&show_demo_window);
    */
    
    toolbarEditor->DrawEditor(deltaTime);
    logEditor->DrawEditor(deltaTime);
}

void CEditorEngine::ExitEditor()
{
    toolbarEditor->ExitEditor();
    logEditor->ExitEditor();
}
