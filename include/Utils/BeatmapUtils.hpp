#pragma once

#include "GlobalNamespace/BeatmapKey.hpp"
#include "GlobalNamespace/BeatmapLevel.hpp"
#include <string>

namespace BetterSongList::BeatmapUtils {
    std::string GetHashOfPreview(GlobalNamespace::BeatmapLevel* level);
    int GetCharacteristicFromDifficulty(GlobalNamespace::BeatmapKey diff);
}