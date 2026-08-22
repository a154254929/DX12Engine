#include "ToolbarEditor.h"

void FToolbarEditor::BuildEditor()
{
}

void FToolbarEditor::DrawEditor(float deltaTime)
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {

            if (ImGui::MenuItem("New Level"))
            {
            
            }
            
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "ctrl+z"))
            {
                // Handle Undo action
            }
            if (ImGui::MenuItem("Redo", "ctrl+y"))
            {
                // Handle Redo action
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void FToolbarEditor::ExitEditor()
{
}