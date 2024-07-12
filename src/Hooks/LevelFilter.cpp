#include "hooking.hpp"

#include "Patches/ImproveBasegameSearch.hpp"
#include "GlobalNamespace/LevelFilter.hpp"

// from ImproveBasegameSearch
MAKE_AUTO_HOOK_ORIG_MATCH(LevelFilter_FilterLevelByText, &GlobalNamespace::LevelFilter::FilterLevelByText, System::Collections::Generic::List_1<GlobalNamespace::BeatmapLevel*>*, System::Collections::Generic::List_1<GlobalNamespace::BeatmapLevel*>* levels, ArrayW<::StringW> searchTexts) {
    System::Collections::Generic::List_1<GlobalNamespace::BeatmapLevel*>* result = nullptr;
    if (!BetterSongList::Hooks::ImproveBasegameSearch::LevelFilter_FilterLevelByText_Prefix(levels, searchTexts, result)) {
        return result;
    }

    return LevelFilter_FilterLevelByText(levels, searchTexts);
}