#include "OperationHandleSelectManager.h"
#if EDITOR_ENGINE

#include "EngineVariableTable.h"
#include "EditorEngine/SelectEditor/OperationHandle/MoveArrow.h"
#include "EditorEngine/SelectEditor/OperationHandle/RotateArrow.h"
#include "EditorEngine/SelectEditor/OperationHandle/ScaleArrow.h"

extern class GMoveArrow* moveArrow;
extern class GScaleArrow* scaleArrow;
extern class GRotateArrow* rotateArrow;
extern class GActorObject* selectedObject;

FOperationHandleSelectManager* FOperationHandleSelectManager::instance = nullptr;

FOperationHandleSelectManager::FOperationHandleSelectManager()
{
}

FOperationHandleSelectManager* FOperationHandleSelectManager::GetInstance()
{
    if (!instance)
    {
        instance = new FOperationHandleSelectManager();
    }
    return instance;
}

void FOperationHandleSelectManager::DestroyInstance()
{
    if (instance)
    {
        delete instance;
        instance = nullptr;
    }
}

void FOperationHandleSelectManager::AllOperationHandleHide()
{
    if (moveArrow)
    {
        moveArrow->SetVisible(false);
    }
    if (scaleArrow)
    {
        scaleArrow->SetVisible(false);
    }
    if (rotateArrow)
    {
        rotateArrow->SetVisible(false);
    }
}

void FOperationHandleSelectManager::DisplaySelectOperationHandle(GActorObject* inSelectOperationHandle)
{
    if (selectedObject)
    {
        AllOperationHandleHide();
        
        if (GOperationHandleBase* inHandleBase = dynamic_cast<GOperationHandleBase*>(inSelectOperationHandle))
        {
            inHandleBase->SetPosition(selectedObject->GetPosition());
            //inHandleBase->SetRotation(selectedObject->GetRotation());
            inHandleBase->SetVisible(true);
        }
            SetNewSelectOperationHandle(inSelectOperationHandle);
    }
}


void FOperationHandleSelectManager::DisplaySelectOperationHandle()
{            
    if (selectedObject)
    {
        if (!selectOperationHandle)
        {
            if (moveArrow)
            {
                moveArrow->SetPosition(selectedObject->GetPosition());
                //inHandleBase->SetRotation(selectedObject->GetRotation());
                moveArrow->SetVisible(true);
                
                SetNewSelectOperationHandle(moveArrow);
            }
        }
        else
        {
            if (GOperationHandleBase* inHandleBase = dynamic_cast<GOperationHandleBase*>(selectOperationHandle))
            {
                inHandleBase->SetPosition(selectedObject->GetPosition());
                //inHandleBase->SetRotation(selectedObject->GetRotation());
                inHandleBase->SetVisible(true);
            }
        }
    }
}

void FOperationHandleSelectManager::HideSelectOperationHandle()
{
    if (GOperationHandleBase* inHandleBase = dynamic_cast<GOperationHandleBase*>(selectOperationHandle))
    {
        inHandleBase->SetVisible(false);
    }
}

void FOperationHandleSelectManager::SetNewSelectOperationHandle(GActorObject* inSelectOperationHandle)
{
    selectOperationHandle = inSelectOperationHandle;
}

void FOperationHandleSelectManager::SetNewSelectObject(GActorObject* inSelectObject)
{
    selectedObject = inSelectObject;
}

#endif
