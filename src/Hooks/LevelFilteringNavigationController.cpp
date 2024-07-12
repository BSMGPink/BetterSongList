#include "hooking.hpp"
#include "Patches/HookSelectedCollection.hpp"
#include "Patches/RestoreLevelSelection.hpp"

#include "GlobalNamespace/LevelFilteringNavigationController.hpp"

// from HookSelectedCollection
MAKE_AUTO_HOOK_MATCH(LevelFilteringNavigationController_UpdateSecondChildControllerContent, &GlobalNamespace::LevelFilteringNavigationController::UpdateSecondChildControllerContent, void, GlobalNamespace::LevelFilteringNavigationController* self, GlobalNamespace::SelectLevelCategoryViewController::LevelCategory levelCategory) {
    BetterSongList::Hooks::HookSelectedCollection::LevelFilteringNavigationController_UpdateSecondChildControllerContent_Prefix(levelCategory);
    LevelFilteringNavigationController_UpdateSecondChildControllerContent(self, levelCategory);
}

// from RestoreLevelSelection
MAKE_AUTO_HOOK_MATCH(LevelFilteringNavigationController_ShowPacksInSecondChildController, &GlobalNamespace::LevelFilteringNavigationController::ShowPacksInSecondChildController, void, GlobalNamespace::LevelFilteringNavigationController* self, System::Collections::Generic::IReadOnlyList_1<::GlobalNamespace::BeatmapLevelPack*>* beatmapLevelPacks) {
    BetterSongList::Hooks::RestoreLevelSelection::LevelFilteringNavigationController_ShowPacksInSecondChildController_Prefix(self, self->_levelPackIdToBeSelectedAfterPresent);
    LevelFilteringNavigationController_ShowPacksInSecondChildController(self, beatmapLevelPacks);
}
