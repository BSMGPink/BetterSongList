#pragma once

#include "custom-types/shared/coroutine.hpp"
#include "UnityEngine/GameObject.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "HMUI/HoverHintController.hpp"
#include "GlobalNamespace/StandardLevelDetailView.hpp"
#include "GlobalNamespace/BeatmapLevel.hpp"
#include "GlobalNamespace/BeatmapKey.hpp"
#include "GlobalNamespace/LevelParamsPanel.hpp"

namespace BetterSongList::Hooks {
    class ExtraLevelParams {
        public:
            /// @brief no prio
            static void StandardLevelDetailView_RefreshContent_Postfix(
                    GlobalNamespace::StandardLevelDetailView* self, 
                    GlobalNamespace::BeatmapLevel* level, 
                    GlobalNamespace::BeatmapKey selectedDifficultyBeatmap,
                    GlobalNamespace::LevelParamsPanel* levelParamsPanel 
                );
            static void UpdateState();
            static void ModifyValue(TMPro::TextMeshProUGUI* text, std::string_view hoverHint, std::string_view icon);
            static GlobalNamespace::StandardLevelDetailView* get_lastInstance();
            static UnityEngine::GameObject* get_extraUI();
            static ArrayW<TMPro::TextMeshProUGUI*> get_fields();
            static HMUI::HoverHintController* get_hoverHintController();

        private:
            static custom_types::Helpers::Coroutine ProcessFields();
            static SafePtrUnity<GlobalNamespace::StandardLevelDetailView> lastInstance;
            static SafePtrUnity<UnityEngine::GameObject> extraUI;
            static SafePtr<Array<TMPro::TextMeshProUGUI*>> fields;
            static SafePtrUnity<HMUI::HoverHintController> hoverHintController;
    };
}