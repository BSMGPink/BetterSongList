#include "Patches/RestoreLevelSelection.hpp"
#include "Patches/RestoreTableScroll.hpp"
#include "Patches/HookLevelCollectionTableSet.hpp"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Sprite.hpp"
#include "GlobalNamespace/BeatmapLevelPack.hpp"
#include "GlobalNamespace/BeatmapLevelPack.hpp"
#include "GlobalNamespace/BeatmapLevel.hpp"
#include "GlobalNamespace/BeatmapKey.hpp"
#include "GlobalNamespace/BeatmapDifficulty.hpp"
#include "GlobalNamespace/LevelSelectionNavigationController.hpp"
#include "GlobalNamespace/LevelFilteringNavigationController.hpp"
#include "GlobalNamespace/BeatmapLevelsModel.hpp"
#include "Utils/PlaylistUtils.hpp"

#include "songcore/shared/SongCore.hpp"

#include "logging.hpp"
#include "config.hpp"

namespace BetterSongList::Hooks {
    std::string RestoreLevelSelection::restoredPackId;
    SafePtr<GlobalNamespace::BeatmapLevelPack> RestoreLevelSelection::restoredPack;

    GlobalNamespace::BeatmapLevelPack* RestoreLevelSelection::get_restoredPack() {
        if (!restoredPack) {
            return nullptr;
        }
        return restoredPack.ptr();
    }
    void RestoreLevelSelection::LevelSelectionFlowCoordinator_DidActivate_Prefix(GlobalNamespace::LevelSelectionFlowCoordinator* self, GlobalNamespace::LevelSelectionFlowCoordinator::State*& startState, bool firstActivation) {
        auto startPack = startState ? startState->beatmapLevelPack : nullptr;
        auto startPackId = startPack ? startPack->packID : nullptr;
        restoredPackId = startPackId ? static_cast<std::string>(startPackId) : "";
        if (startState) {
            return;
        }

        INFO("restored Pack Id: {}", restoredPackId);
        auto restoreCategory = config.get_lastCategory();
        auto& restoreLevel = config.get_lastSong();
        GlobalNamespace::BeatmapLevel* m = nullptr;
        
        GlobalNamespace::BeatmapLevelsModel* levelsModel = self->levelSelectionNavigationController->_levelFilteringNavigationController->_beatmapLevelsModel;
        if (!restoreLevel.empty()) {
            m = levelsModel->GetBeatmapLevel(levelsModel);
        }

        LoadPackFromCollectionName(levelsModel);
        if (m) {

            auto levelCategory = System::Nullable_1<GlobalNamespace::SelectLevelCategoryViewController::LevelCategory>();
            levelCategory.value = restoreCategory;
            levelCategory.hasValue = true;

            auto state = GlobalNamespace::LevelSelectionFlowCoordinator::State::New_ctor(
                    get_restoredPack(),
                    static_cast<GlobalNamespace::BeatmapLevel *>(m)
            );

            state->___levelCategory = levelCategory;
            startState = state;
        }     
    }

    void RestoreLevelSelection::LoadPackFromCollectionName(GlobalNamespace::BeatmapLevelsModel* levelsModel) {
        INFO("Loading pack from name");
        if (get_restoredPack()) {
            auto packID = get_restoredPack()->packID;
            if (packID && packID == config.get_lastPack()) {
                return;
            }
        }

        if (config.get_lastPack().empty()) {
            restoredPack.emplace(nullptr);
            INFO("Config lastpack was empty");
            return;
        }

        restoredPack.emplace(levelsModel->GetLevelPackForLevelId(config.get_lastPack()));
    }

    void RestoreLevelSelection::LevelFilteringNavigationController_ShowPacksInSecondChildController_Prefix(GlobalNamespace::LevelFilteringNavigationController* self, StringW& levelPackIdToBeSelectedAfterPresent) {
        if (levelPackIdToBeSelectedAfterPresent) return;
        auto levelsModel = self->_beatmapLevelsModel;
        LoadPackFromCollectionName(levelsModel);

        levelPackIdToBeSelectedAfterPresent = get_restoredPack() ? get_restoredPack()->packID : nullptr;
    }
}