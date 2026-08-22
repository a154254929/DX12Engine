#pragma once
#include "../Engine/Core/Engine.h"
#include "Core/EditorBase.h"

class CEditorEngine
    : public CEngine
    , public FEditorBase
{
public:
    CEditorEngine();

    virtual int PreInit(
#if defined(_WIN32)
        FWinMainCommandParameters
#endif
    );
    virtual int Init(
#if defined(_WIN32)
        FWinMainCommandParameters
#endif
    );
    virtual int PostInit();

    virtual int PreExit();
    virtual int Exit();
    virtual int PostExit();
    
    virtual void BuildEditor();
    virtual void DrawEditor(float deltaTime);
    virtual void ExitEditor();
    
    class FToolbarEditor* toolbarEditor;
    class FLogEditor* logEditor;
};