#include "hooking.hpp"
#include "logging.hpp"

#include "Patches/RestoreTableScroll.hpp"
#include "Patches/HookSelectedInTable.hpp"
#include "Patches/HookLevelCollectionTableSet.hpp"
#include "Patches/UI/ScrollEnhancement.hpp"
#include "GlobalNamespace/LevelCollectionTableView.hpp"

// from RestoreTableScroll
// from UI/ScrollEnhancement
MAKE_AUTO_HOOK_MATCH(LevelCollectionTableView_Init_0, static_cast<void (GlobalNamespace::LevelCollectionTableView::*)()>(&GlobalNamespace::LevelCollectionTableView::Init), void, GlobalNamespace::LevelCollectionTableView* self) {
    BetterSongList::Hooks::RestoreTableScroll::LevelCollectionTableView_Init_Prefix(self);
    BetterSongList::Hooks::ScrollEnhancement::LevelCollectionTableView_Init_Prefix(self, self->_isInitialized, self->_tableView);
    LevelCollectionTableView_Init_0(self);
}

// from RestoreTableScroll
MAKE_AUTO_HOOK_MATCH(LevelCollectionTableView_Init_2, static_cast<void (GlobalNamespace::LevelCollectionTableView::*)(::StringW, ::UnityEngine::Sprite*)>(&GlobalNamespace::LevelCollectionTableView::Init), void, GlobalNamespace::LevelCollectionTableView* self, StringW headerText, UnityEngine::Sprite* headerSprite) {
    BetterSongList::Hooks::RestoreTableScroll::LevelCollectionTableView_Init_Prefix(self);
    LevelCollectionTableView_Init_2(self, headerText, headerSprite);
}

// from RestoreTableScroll
// from HookLevelCollectionTableSet
MAKE_AUTO_HOOK_MATCH(LevelCollectionTableView_SetData, &GlobalNamespace::LevelCollectionTableView::SetData, void, GlobalNamespace::LevelCollectionTableView* self, System::Collections::Generic::IReadOnlyList_1<GlobalNamespace::BeatmapLevel*>* beatmapLevels, System::Collections::Generic::HashSet_1<StringW>* favoriteLevelIds, bool beatmapLevelsAreSorted, bool sortBeatmapLevels) {
    ArrayW<GlobalNamespace::BeatmapLevel*> arr{(void*)beatmapLevels};

    BetterSongList::Hooks::HookLevelCollectionTableSet::LevelCollectionTableView_SetData_Prefix(self, arr, favoriteLevelIds, beatmapLevelsAreSorted);
    LevelCollectionTableView_SetData(self, reinterpret_cast<System::Collections::Generic::IReadOnlyList_1<GlobalNamespace::BeatmapLevel*>*>(arr.convert()), favoriteLevelIds, beatmapLevelsAreSorted, sortBeatmapLevels);
    BetterSongList::Hooks::RestoreTableScroll::LevelCollectionTableView_SetData_PostFix(self);
    BetterSongList::Hooks::HookLevelCollectionTableSet::LevelCollectionTableView_SetData_PostFix(self, arr);
}
// from HookSelectedInTable
MAKE_AUTO_HOOK_MATCH(LevelCollectionTableView_HandleDidSelectRowEvent, &GlobalNamespace::LevelCollectionTableView::HandleDidSelectRowEvent, void, GlobalNamespace::LevelCollectionTableView* self, ::HMUI::TableView* tableView, int row) {
    LevelCollectionTableView_HandleDidSelectRowEvent(self, tableView, row);
    BetterSongList::Hooks::HookSelectedInTable::LevelCollectionTableView_HandleDidSelectRowEvent_Postfix(self->_selectedBeatmapLevel);
}