#include "hooking.hpp"
#include "Patches/HookSelectedCollection.hpp"

#include "GlobalNamespace/AnnotatedBeatmapLevelCollectionsViewController.hpp"

// from HookSelectedCollection
MAKE_AUTO_HOOK_MATCH(AnnotatedBeatmapLevelCollectionsViewController_SetData, &GlobalNamespace::AnnotatedBeatmapLevelCollectionsViewController::SetData, void, GlobalNamespace::AnnotatedBeatmapLevelCollectionsViewController* self, ::System::Collections::Generic::IReadOnlyList_1<::GlobalNamespace::BeatmapLevelPack*>* annotatedBeatmapLevelCollections, int selectedItemIndex, bool hideIfOneOrNoPacks) {
    AnnotatedBeatmapLevelCollectionsViewController_SetData(self, annotatedBeatmapLevelCollections, selectedItemIndex, hideIfOneOrNoPacks);
    BetterSongList::Hooks::HookSelectedCollection::AnnotatedBeatmapLevelCollectionsViewController_SetData_PostFix(self);
}

// from HookSelectedCollection
MAKE_AUTO_HOOK_MATCH(AnnotatedBeatmapLevelCollectionsViewController_HandleDidSelectAnnotatedBeatmapLevelCollection, &GlobalNamespace::AnnotatedBeatmapLevelCollectionsViewController::HandleDidSelectAnnotatedBeatmapLevelCollection, void, GlobalNamespace::AnnotatedBeatmapLevelCollectionsViewController* self, ::GlobalNamespace::BeatmapLevelPack* beatmapLevelCollection) {
    BetterSongList::Hooks::HookSelectedCollection::AnnotatedBeatmapLevelCollectionsViewController_HandleDidSelectAnnotatedBeatmapLevelCollection_Prefix(beatmapLevelCollection);
    AnnotatedBeatmapLevelCollectionsViewController_HandleDidSelectAnnotatedBeatmapLevelCollection(self, beatmapLevelCollection);
}