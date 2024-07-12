#include "hooking.hpp"

#include "Patches/HookBeatmapDataLoader.hpp"

#include "BeatmapDataLoaderVersion3/BeatmapDataLoader.hpp"
#include "BeatmapSaveDataVersion3/BeatmapSaveData.hpp"
#include "BeatmapSaveDataVersion3/BeatmapSaveDataItem.hpp"
#include "BeatmapSaveDataVersion3/ObstacleData.hpp"
#include "GlobalNamespace/BeatmapDataBasicInfo.hpp"
#include "UnityEngine/JsonUtility.hpp"

// from UI/SongDeleteButton
// from UI/ExtraLevelParams
MAKE_AUTO_HOOK_MATCH(BeatmapDataLoaderVersion3_BeatmapDataLoader_GetBeatmapDataBasicInfoFromSaveDataJson, 
                    &BeatmapDataLoaderVersion3::BeatmapDataLoader::GetBeatmapDataBasicInfoFromSaveDataJson, 
                    GlobalNamespace::BeatmapDataBasicInfo*, StringW beatmapJson)
{
    if (!beatmapJson)
    {
        return nullptr;
    }
    BeatmapSaveDataVersion3::BeatmapSaveData* beatmapSaveData = UnityEngine::JsonUtility::FromJson<BeatmapSaveDataVersion3::BeatmapSaveData*>(beatmapJson);
    if (beatmapSaveData == nullptr)
    {
        return nullptr;
    }
    int count = beatmapSaveData->___colorNotes->get_Count();
    int count2 = beatmapSaveData->___obstacles->get_Count();
    int count3 = beatmapSaveData->___bombNotes->get_Count();

    std::vector<BetterSongList::Hooks::SimpleObstacle> obstacles;
    for (int i = 0; i < count2; i++)
    {
        auto obstacle = beatmapSaveData->___obstacles->get_Item(i);
        obstacles.push_back(BetterSongList::Hooks::SimpleObstacle(obstacle->x, obstacle->y, obstacle->width, obstacle->height, obstacle->duration, obstacle->beat));
    }
    auto mark = BetterSongList::Hooks::HookBeatmapDataLoader::GetMarkStatus(obstacles);
    auto beatmapDataBasicInfo = GlobalNamespace::BeatmapDataBasicInfo::New_ctor(4, count, count2 * (mark ? -1 : 1), count3);

    return beatmapDataBasicInfo;
}