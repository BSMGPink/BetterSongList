#include "hooking.hpp"

#include "GlobalNamespace/StandardLevelDetailView.hpp"
#include "GlobalNamespace/BeatmapLevel.hpp"
#include "Patches/UI/ExtraLevelParams.hpp"

// from UI/SongDeleteButton
// from UI/ExtraLevelParams
MAKE_AUTO_HOOK_MATCH(StandardLevelDetailView_RefreshContent, &GlobalNamespace::StandardLevelDetailView::RefreshContent, void, GlobalNamespace::StandardLevelDetailView* self)
{
    StandardLevelDetailView_RefreshContent(self);
    BetterSongList::Hooks::ExtraLevelParams::StandardLevelDetailView_RefreshContent_Postfix(self, self->_beatmapLevel, self->beatmapKey, self->_levelParamsPanel);
}