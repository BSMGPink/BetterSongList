#pragma once

#include "GlobalNamespace/BeatmapLevelPack.hpp"
#include "GlobalNamespace/BeatmapLevel.hpp"
#include "System/Collections/Generic/Dictionary_2.hpp"

namespace BetterSongList::PlaylistUtils {
    template<typename T, typename U>
    using Dictionary = System::Collections::Generic::Dictionary_2<T, U>;
    
    bool get_hasPlaylistLib();
    Dictionary<StringW, GlobalNamespace::BeatmapLevelPack*>* get_builtinPacks();

    void Init();
    GlobalNamespace::BeatmapLevelPack* GetPack(StringW packID);
    ArrayW<GlobalNamespace::BeatmapLevel*> GetLevelsForLevelCollection(GlobalNamespace::BeatmapLevelPack* levelCollection);
}