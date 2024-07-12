#pragma once

#include "ISorter.hpp"
#include "IFilter.hpp"

#include "GlobalNamespace/LevelCollectionTableView.hpp"
#include "System/Collections/Generic/HashSet_1.hpp"
#include <thread>

namespace BetterSongList::Hooks {
    template<typename T>
    using HashSet = System::Collections::Generic::HashSet_1<T>;
    class HookLevelCollectionTableSet {
        public:
            static ISorter* sorter;
            static IFilter* filter;

            static ArrayW<GlobalNamespace::BeatmapLevel*> get_lastInMapList();
            static ArrayW<GlobalNamespace::BeatmapLevel*> get_lastOutMapList();

            static void Refresh(bool processAsync = false, bool clearAsyncResult = true);

            /// @brief prio int max value
            static void LevelCollectionTableView_SetData_Prefix(GlobalNamespace::LevelCollectionTableView* self, ArrayW<GlobalNamespace::BeatmapLevel*>& previewBeatmapLevels, HashSet<StringW>* favoriteLevelIds, bool& beatmapLevelsAreSorted);
            /// @brief no prio
            static void LevelCollectionTableView_SetData_PostFix(GlobalNamespace::LevelCollectionTableView* self, ArrayW<GlobalNamespace::BeatmapLevel*> previewBeatmapLevels);
        private:
            static void FilterWrapper(ArrayW<GlobalNamespace::BeatmapLevel*>& previewBeatmapLevels);
            static bool PrepareStuffIfNecessary(std::function<void()> cb = nullptr, bool cbOnAlreadyPrepared = false);
            static std::function<void(ArrayW<GlobalNamespace::BeatmapLevel*>)> recallLast;
            static SafePtr<Array<GlobalNamespace::BeatmapLevel*>> lastInMapList;
            static SafePtr<Array<GlobalNamespace::BeatmapLevel*>> lastOutMapList;
            static SafePtr<Array<GlobalNamespace::BeatmapLevel*>> asyncPreProcessed;
            static ISorterWithLegend::Legend customLegend;
            static bool prepareThreadCurrentlyRunning;
    };
}