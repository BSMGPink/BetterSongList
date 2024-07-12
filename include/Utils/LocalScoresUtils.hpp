#pragma once

#include "GlobalNamespace/BeatmapLevel.hpp"
namespace BetterSongList::LocalScoresUtils {
    bool get_hasScores();
    bool HasLocalScore(GlobalNamespace::BeatmapLevel* level);
    void Load();
}