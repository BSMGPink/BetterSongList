#include "Utils/BeatmapUtils.hpp"
#include "logging.hpp"

#include "GlobalNamespace/BeatmapCharacteristicSO.hpp"

namespace BetterSongList::BeatmapUtils {
    std::string GetHashOfPreview(GlobalNamespace::BeatmapLevel* level) {
        if (!level) return "";
        auto levelId = static_cast<std::string>(level->levelID);
        if (levelId.size() < 53) return "";
        if (levelId[12] != '_') return "";
        return levelId.substr(13, 40);
    }
    
    int GetCharacteristicFromDifficulty(GlobalNamespace::BeatmapKey diff) {
        auto chara = diff.beatmapCharacteristic;
        auto d = chara ? chara->get_sortingOrder() : 0;
        if (d == 0 || d > 4) return 0;

        if (d == 3)
            d = 4;
        else if (d == 4)
            d = 3;

        return d + 1;
    }
}