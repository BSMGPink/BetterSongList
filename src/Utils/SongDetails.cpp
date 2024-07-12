#include "Utils/SongDetails.hpp"
#include "logging.hpp"

#include <thread>

namespace BetterSongList::SongDetails {
    static SongDetailsCache::SongDetails* songDetails = nullptr;
    SongDetailsCache::SongDetails* get_songDetails() {
        return songDetails;
    }

    bool get_isAvailable() {
        return CheckAvailable();
    }

    bool CheckAvailable() {
        if (songDetails == nullptr)
            return false;

        return  songDetails->songs.get_isDataAvailable();
    }

    static bool finishedInitAttempt = false;
    bool get_finishedInitAttempt() {
        return finishedInitAttempt;
    }
    
    static bool attemptedToInit = false;
    bool get_attemptedToInit() {
        return attemptedToInit;
    }

    std::string GetUnavailabilityReason() {
        if (finishedInitAttempt && !songDetails->songs.get_isDataAvailable() || songDetails->songs.size() == 0) {
            return "Initialization failed";
        }
        return "";
    }

    void Init() {
        if (attemptedToInit) return;
        attemptedToInit = true;
        std::thread([](){
            DEBUG("Getting songdetails");
            songDetails = SongDetailsCache::SongDetails::Init().get();
            DEBUG("Got songdetails");


            if (!songDetails->songs.get_isDataAvailable()) {
                finishedInitAttempt = true;
                DEBUG("BSL Failed");
            } else {
                DEBUG("BSL Not failed Songs size:{}", songDetails->songs.size());
                finishedInitAttempt = true;
            }
        }).detach();
    }

        /// @brief Gets the song_data_core::BeatStarCharacteristics from a passed serialized char name
        /// @param serializedName the name to check for
        /// @return song_data_core::BeatStarCharacteristic of the name, or Unknown for invalid
        SongDetailsCache::MapCharacteristic StringToBeatStarCharacteristics(std::string_view serializedName)
        {
            switch(serializedName.data()[0])
            {
                case 's': [[fallthrough]];
                case 'S': return SongDetailsCache::MapCharacteristic::Standard;
                case 'o': [[fallthrough]];
                case 'O': return SongDetailsCache::MapCharacteristic::OneSaber;
                case 'n': [[fallthrough]];
                case 'N': return SongDetailsCache::MapCharacteristic::NoArrows;
                case 'd': [[fallthrough]];
                case 'D':
                    if (serializedName.data()[6] == '9') return SongDetailsCache::MapCharacteristic::NinetyDegree;
                    else return SongDetailsCache::MapCharacteristic::ThreeSixtyDegree;
                case 'l': [[fallthrough]];
                case 'L': {
                    if (serializedName.data()[1] == 'a' || serializedName.data()[1] == 'A') return SongDetailsCache::MapCharacteristic::Lawless;
                    else return SongDetailsCache::MapCharacteristic::LightShow;
                }
                default: return SongDetailsCache::MapCharacteristic::Custom;
            }
        }

        /// @brief gets the serializedName from the game object
        /// @param char_ the characteristic SO to get thet name for
        /// @return string serialized name
        std::string BeatmapCharacteristicToString(GlobalNamespace::BeatmapCharacteristicSO* char_)
        {
            #if defined __has_include && __has_include("GlobalNamespace/BeatmapCharacteristicSO.hpp")
            return char_->get_serializedName();
            #else
            return CRASH_UNLESS(il2cpp_utils::RunMethod<StringW>(char_, "get_serializedName"));
            #endif
        }

        /// @brief gets the characteristic that belongs with this game SO
        /// @param  char_ characteristicSO to get the characteristic for
        /// @return song_data_core::BeatStarCharacteristics
        SongDetailsCache::MapCharacteristic BeatmapCharacteristicToBeatStarCharacteristic(GlobalNamespace::BeatmapCharacteristicSO* char_)
        {
            return StringToBeatStarCharacteristics(BeatmapCharacteristicToString(char_));
        }
}