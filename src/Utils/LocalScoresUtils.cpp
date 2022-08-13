#include "Utils/LocalScoresUtils.hpp"

#include "GlobalNamespace/PlayerDataModel.hpp"
#include "GlobalNamespace/PlayerData.hpp"
#include "GlobalNamespace/PlayerLevelStatsData.hpp"
#include "GlobalNamespace/SharedCoroutineStarter.hpp"

#include "custom-types/shared/coroutine.hpp"

#define COROUTINE(coroutine) GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(custom_types::Helpers::CoroutineHelper::New(coroutine));

namespace BetterSongList::LocalScoresUtils {
    SafePtrUnity<GlobalNamespace::PlayerDataModel> playerDataModel;
    struct PlayedMaps : std::vector<std::string> {
        PlayedMaps(int count) : std::vector<std::string>() { reserve(count); }
        auto find(const std::string& str) { return std::find(begin(), end(), str); }
        const auto find(const std::string& str) const { return std::find(begin(), end(), str); }
    };

    PlayedMaps playedMaps(500);

    GlobalNamespace::PlayerDataModel* get_playerDataModel() {
        if (!playerDataModel) {
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
        auto playerData = playerDataModel ? playerDataModel->get_playerData() : nullptr;
        ListWrapper<GlobalNamespace::PlayerLevelStatsData*> levelData{playerData ? playerData->get_levelsStatsData() : nullptr};
        if (!levelData) {
            return false;
        }

        // only iterate the new entries
        for (int i = levelData->get_Count(); i-- > playedMaps.size();) {
            auto x = levelData[i];
            if (x->get_validScore() && x->get_levelID() == levelId) {
                playedMaps.push_back(levelId);
                return true;
            }
        }
        return false;
    }

    bool HasLocalScore(GlobalNamespace::IPreviewBeatmapLevel* level) {
        if (!level) return false;
        auto levelId = level->get_levelID();
        return levelId ? HasLocalScore(levelId) : false;
    }

    static bool isLoadingScores = false;

    custom_types::Helpers::Coroutine LoadScoresCoroutine() {
        auto playerDataModel = get_playerDataModel();
        auto playerData = playerDataModel ? playerDataModel->get_playerData() : nullptr;
        ListWrapper<GlobalNamespace::PlayerLevelStatsData*> levelData{playerData ? playerData->get_levelsStatsData() : nullptr};
        if (!levelData) {
            isLoadingScores = false;
            co_return;
        }

        for (int count = 0; auto x : levelData) {
            if ((count++ % 50) == 0) co_yield nullptr;
            if (!x->get_validScore()) continue;
            auto levelId = static_cast<std::string>(x->get_levelID());
            if (playedMaps.find(levelId) == playedMaps.end()) {
                playedMaps.emplace_back(levelId);
            }
        }

        isLoadingScores = false;
        co_return;
    }
    
    void Load() {
        // TODO: main thread scheduler thing?
        if (isLoadingScores || get_hasScores()) return;
        isLoadingScores = true;
        COROUTINE(LoadScoresCoroutine());
    }
}