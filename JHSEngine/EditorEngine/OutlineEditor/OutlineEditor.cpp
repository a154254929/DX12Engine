#include "OutlineEditor.h"
#include "../../Engine/Core/World.h"
#include "../../Engine/Actor/Core/ActorObject.h"
#include "Engine/Component/Mesh/Core/MeshComponent.h"
#include "Engine/Component/Mesh/Core/MeshComponentType.h"
#include "Engine/Mesh/Core/Mesh.h"
#include "../../Common/EngineVariableTable.h"
#include "Engine/Rendering/Core/DirectX/RenderingPipeline/RenderingPipeline.h"
#include "Engine/Rendering/Core/DirectX/RenderingPipeline/RenderLayer/RenderLayerManager.h"


void FOutlineEditor::BuildEditor()
{
}

extern int actorSelectId;
void FOutlineEditor::DrawEditor(float deltaTime)
{
    ImGui::Begin("Outline Editor");
    if (CWorld* inWorld = GetWorld())
    {
        ImGui::BeginChild("leftPanel", ImVec2(200, 0), true);
        {
            for (int i = 0; i < inWorld->GetAllActors().size(); ++i)
            {
                GActorObject* actor = inWorld->GetAllActors()[i];
                char objectName[128] = { 0 };
                sprintf(objectName, "%s", actor->GetName().c_str());
                if (ImGui::Selectable(objectName, i == actorSelectId))
                {
                    actorSelectId = i;
                    HighlightDisplayObject(actor);
                }
            }
        }
        ImGui::EndChild();
    }
    ImGui::End();
}

void FOutlineEditor::ExitEditor()
{
}

void FOutlineEditor::HighlightDisplayObject(GActorObject* inActorObject)
{
    if (FRenderLayerManager* layerManager = GetRenderingLayerManager())
    {
        layerManager->HighlightDisplayObject(inActorObject);
    }
}
