#pragma once

#include "GlobalNamespace/LevelSearchViewController.hpp"
namespace BetterSongList::Hooks {
    class HookFilterClear {
        public:
            /// @brief prio int max value
            static void LevelSearchViewController_ResetFilter_Prefix();
    };
}