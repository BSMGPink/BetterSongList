#include "hooking.hpp"
#include "config.hpp"

#include "Patches/HookFilterClear.hpp"
#include "GlobalNamespace/LevelSearchViewController.hpp"
#include "GlobalNamespace/LevelFilter.hpp"

// from HookFilterClear
MAKE_AUTO_HOOK_MATCH(LevelSearchViewController_Refresh, static_cast<void(GlobalNamespace::LevelSearchViewController::*)()>(&GlobalNamespace::LevelSearchViewController::RefreshAsync), void, GlobalNamespace::LevelSearchViewController* self) {
    BetterSongList::Hooks::HookFilterClear::LevelSearchViewController_ResetFilter_Prefix();
    if(config.get_autoFilterUnowned()) {
        self->____currentSearchFilter.songOwned = true;
        self->____currentSearchFilter.songNotOwned = false;
    }
    LevelSearchViewController_Refresh(self);
}
