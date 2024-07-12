#pragma once

#include <string>
#include <vector>
#include "custom-types/shared/coroutine.hpp"
#include "song-details/shared/SongDetails.hpp"
#include "song-details/shared/Data/Song.hpp"
#include "GlobalNamespace/BeatmapCharacteristicSO.hpp"

namespace BetterSongList::SongDetails {
    bool get_isAvailable();
    bool get_finishedInitAttempt();
    bool get_attemptedToInit();
    
    bool CheckAvailable();
    std::string GetUnavailabilityReason();
    SongDetailsCache::SongDetails * get_songDetails();
    void Init();

    SongDetailsCache::MapCharacteristic StringToBeatStarCharacteristics(std::string_view serializedName);
    std::string BeatmapCharacteristicToString(GlobalNamespace::BeatmapCharacteristicSO* char_);
    SongDetailsCache::MapCharacteristic BeatmapCharacteristicToBeatStarCharacteristic(GlobalNamespace::BeatmapCharacteristicSO* char_);
}