#include "LogEditor.h"

#include "LogObject/EditorLogSystem.h"

void FLogEditor::BuildEditor()
{
    FEditorLogSystem::GetInstance();
}

void FLogEditor::DrawEditor(float deltaTime)
{
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);;
    
    bool bOpenLog = true;
    ImGui::Begin("Log", &bOpenLog);
    
    //ImGui::Text("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
    
    ImGui::End();
    
    //绘制字体
    FEditorLogSystem::GetInstance()->Draw(deltaTime);
}

void FLogEditor::ExitEditor()
{
    FEditorLogSystem::Destroy();
}