#include "EditorEngine.h"

#include "ToolbarEditor/ToolbarEditor.h"
#include "LogEditor/LogEditor.h"
#include "OutlineEditor/OutlineEditor.h"

CEditorEngine::CEditorEngine()
{
    toolbarEditor = new FToolbarEditor();
    logEditor = new FLogEditor();
    outlineEditor = new FOutlineEditor();
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
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    
    toolbarEditor->BuildEditor();
    logEditor->BuildEditor();
    outlineEditor->BuildEditor();
}

void CEditorEngine::DrawEditor(float deltaTime)
{
    DrawLayer(deltaTime);
    
    bool show_demo_window = true;
    ImGui::ShowDemoWindow(&show_demo_window);
    
    toolbarEditor->DrawEditor(deltaTime);
    logEditor->DrawEditor(deltaTime);
    outlineEditor->DrawEditor(deltaTime);
}

void CEditorEngine::ExitEditor()
{
    toolbarEditor->ExitEditor();
    logEditor->ExitEditor();
    outlineEditor->ExitEditor();
}

void CEditorEngine::DrawLayer(float deltaTime)
{
    
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGuiWindowFlags windowsFlags = ImGuiWindowFlags_NoTitleBar
                                    | ImGuiWindowFlags_NoCollapse
                                    | ImGuiWindowFlags_NoResize
                                    | ImGuiWindowFlags_NoMove
                                    | ImGuiWindowFlags_NoBringToFrontOnFocus
                                    | ImGuiWindowFlags_NoNavFocus
                                    | ImGuiWindowFlags_NoBackground;
    
    bool bOpenWindows = true;
    ImGui::Begin("Hello", &bOpenWindows, windowsFlags);
    
    ImGui::PopStyleVar();
    ImGui::PopStyleVar(2);
    
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockSpaceId = ImGui::GetID("MainDockSpace");
        ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
    }
    
    ImGui::End();
}
