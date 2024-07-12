#include "Utils/LocalScoresUtils.hpp"

#include "GlobalNamespace/PlayerDataModel.hpp"
#include "GlobalNamespace/PlayerData.hpp"
#include "GlobalNamespace/PlayerLevelStatsData.hpp"
#include "bsml/shared/BSML/SharedCoroutineStarter.hpp"
#include "System/Collections/Generic/List_1.hpp"
#include "System/Collections/Generic/Dictionary_2.hpp"

#include "logging.hpp"

#include "custom-types/shared/coroutine.hpp"

#define COROUTINE(coroutine) BSML::SharedCoroutineStarter::get_instance()->StartCoroutine(custom_types::Helpers::CoroutineHelper::New(coroutine));

namespace BetterSongList::LocalScoresUtils {
    SafePtrUnity<GlobalNamespace::PlayerDataModel> playerDataModel;

    struct PlayedMaps : std::vector<std::string> {
        PlayedMaps(int count) : std::vector<std::string>() { reserve(count); }
        auto find(const std::string& str) { return std::find(begin(), end(), str); }
        const auto find(const std::string& str) const { return std::find(begin(), end(), str); }
    };

    PlayedMaps playedMaps(500);

    GlobalNamespace::PlayerDataModel* get_playerDataModel() {
        if (!playerDataModel || !playerDataModel.ptr()) {
            playerDataModel = UnityEngine::Object::FindObjectOfType<GlobalNamespace::PlayerDataModel*>();
        }
        return playerDataModel.ptr();
    }

    bool get_hasScores() {
        return get_playerDataModel();
    }

    bool HasLocalScore(std::string levelId) {
        if (playedMaps.find(levelId) != playedMaps.end()) {
            return true;
        }

        auto playerDataModel = get_playerDataModel();

        auto playerData = playerDataModel ? playerDataModel->playerData : nullptr;
        auto levelData = ListW<GlobalNamespace::PlayerLevelStatsData*>::New();
        auto stats = playerData ? playerData->get_levelsStatsData()->get_Values()->i___System__Collections__Generic__IEnumerable_1_TValue_() : nullptr;
        INFO("{}", fmt::ptr(stats));
        levelData->AddRange(stats);
        if (!levelData) {
            return false;
        }

        // only iterate the new entries
        for (int i = levelData.size(); i-- > playedMaps.size();) {
            auto x = levelData[i];
            if (x->validScore && x->levelID == levelId) {
                playedMaps.push_back(levelId);
                return true;
            }
        }
        return false;
    }

    bool HasLocalScore(GlobalNamespace::BeatmapLevel* level) {
        if (!level) return false;
        auto levelId = level->levelID;
        return levelId ? HasLocalScore(levelId) : false;
    }

    static bool isLoadingScores = false;

    void Load() {
        if (isLoadingScores || get_hasScores()) return;
        isLoadingScores = true;
        auto playerDataModel = get_playerDataModel();
        INFO("{}", fmt::ptr(playerDataModel));
        auto playerData = playerDataModel ? playerDataModel->get_playerData() : nullptr;
        INFO("{}", fmt::ptr(playerData));
        auto levelData = ListW<GlobalNamespace::PlayerLevelStatsData*>::New();
        auto stats = playerData ? playerData->get_levelsStatsData()->get_Values()->i___System__Collections__Generic__IEnumerable_1_TValue_() : nullptr;
        INFO("{}", fmt::ptr(stats));
        levelData->AddRange(stats);
        
        if (levelData) {
            std::thread([](ListW<GlobalNamespace::PlayerLevelStatsData*> levelData){
                INFO("x1");
                for (auto x : levelData) {
                    INFO("{}", fmt::ptr(x));
                    if (!x->validScore) continue;
                    auto levelId = static_cast<std::string>(x->levelID);
                    if (playedMaps.find(levelId) == playedMaps.end()) {
                        playedMaps.emplace_back(levelId);
                    }
                }

                isLoadingScores = false;
            }, levelData).detach();
        }
    }
}