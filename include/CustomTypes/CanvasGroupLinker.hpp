#pragma once

#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/CanvasGroup.hpp"

DECLARE_CLASS_CODEGEN(BetterSongList, CanvasGroupLinker, UnityEngine::MonoBehaviour,

    DECLARE_INSTANCE_METHOD(void, Update);

    public:
    bool triggered;
    float lastAlpha;
    UnityEngine::CanvasGroup* baseCanvasGroup;
    UnityEngine::CanvasGroup* targetCanvasGroup;
    std::function<void()> OnCanvasGroupChanged;
)