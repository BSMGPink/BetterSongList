#include "Patches/HookFilterClear.hpp"

#include "UI/FilterUI.hpp"

namespace BetterSongList::Hooks {
    void HookFilterClear::LevelSearchViewController_ResetFilter_Prefix() {
        FilterUI::SetFilter("", false, false);
    }
}