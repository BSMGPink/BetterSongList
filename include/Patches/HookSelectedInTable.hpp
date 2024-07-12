#pragma once

#include "GlobalNamespace/BeatmapLevel.hpp"

namespace BetterSongList::Hooks {
    class HookSelectedInTable {
        public:
            /// @brief prio int min value
            static void LevelCollectionTableView_HandleDidSelectRowEvent_Postfix(GlobalNamespace::BeatmapLevel* selectedPreviewBeatmapLevel);
    };
}