#pragma once

#include "GlobalNamespace/BeatmapLevel.hpp"
#include "System/Collections/Generic/List_1.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"

namespace BetterSongList::Hooks {
    class ImproveBasegameSearch {
        public:
            /// @brief prio int min value + 10
            static bool LevelFilter_FilterLevelByText_Prefix(System::Collections::Generic::List_1<GlobalNamespace::BeatmapLevel*>* levels, ArrayW<StringW>& searchTexts, System::Collections::Generic::List_1<GlobalNamespace::BeatmapLevel*>*& result);
    };
}