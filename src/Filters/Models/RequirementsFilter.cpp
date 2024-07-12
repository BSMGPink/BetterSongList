#include "Filters/Models/RequirementsFilter.hpp"
#include "logging.hpp"

#include "songcore/shared/SongCore.hpp"
#include "songcore/shared/CustomJSONData.hpp"

#include "System/Threading/Tasks/Task_1.hpp"
#include "Utils/BeatmapUtils.hpp"

#include "songcore/shared/SongLoader/CustomBeatmapLevel.hpp"


namespace BetterSongList {
    bool RequirementsFilter::inited = false;

    RequirementsFilter::RequirementsFilter(bool invert) 
        : IFilter(), invert(invert) {}

    bool RequirementsFilter::get_isReady() const {
        return SongCore::API::Loading::AreSongsLoaded();
    }

    std::future<void> RequirementsFilter::Prepare() {
        return std::async(std::launch::async, [this](){
            auto hasLoaded = SongCore::API::Loading::AreSongsLoaded();
            while (!hasLoaded) std::this_thread::yield();
        });
    }

    bool RequirementsFilter::GetValueFor(GlobalNamespace::BeatmapLevel* level) {
        auto customLevel = il2cpp_utils::try_cast<SongCore::SongLoader::CustomBeatmapLevel>(level).value_or(nullptr);
        if (!customLevel) {
            DEBUG("Level was not custom level!");
            return invert;
        }

        auto saveData = customLevel->get_standardLevelInfoSaveDataV2().value_or(nullptr);
        if (!saveData) {
            DEBUG("Level had no save data!");
            return invert;
        }

        auto sets = saveData->____difficultyBeatmapSets;

        for (auto set : sets) {
            auto chara = set->____beatmapCharacteristicName;
            auto diffs = set->____difficultyBeatmaps;
            for (auto diff : diffs) {
                auto customData = saveData->get_CustomSaveDataInfo();
                if (!customData.has_value()) {
                    continue;
                }

                auto detailsOpt = customData.value().get().TryGetCharacteristicAndDifficulty(chara, diff->____difficultyRank);
                if(detailsOpt) {
                    auto details = detailsOpt.value();
                    if (details.get().requirements.size() > 0) return !invert;
                }
            }
        }
        
        DEBUG("Custom Data contained 0 requirements!");
        return invert;
    }
}