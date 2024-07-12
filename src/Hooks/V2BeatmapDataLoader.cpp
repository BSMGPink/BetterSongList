#include "hooking.hpp"

#include "Patches/HookBeatmapDataLoader.hpp"

#include "BeatmapDataLoaderVersion2_6_0AndEarlier/BeatmapDataLoader.hpp"
#include "BeatmapSaveDataVersion2_6_0AndEarlier/BeatmapSaveData.hpp"
#include "BeatmapSaveDataVersion2_6_0AndEarlier/BeatmapSaveDataItem.hpp"
#include "BeatmapSaveDataVersion2_6_0AndEarlier/ObstacleData.hpp"
#include "BeatmapSaveDataVersion2_6_0AndEarlier/NoteData.hpp"
#include "BeatmapSaveDataVersion2_6_0AndEarlier/NoteType.hpp"
#include "GlobalNamespace/BeatmapDataBasicInfo.hpp"
#include "UnityEngine/JsonUtility.hpp"

// from UI/SongDeleteButton
// from UI/ExtraLevelParams
MAKE_AUTO_HOOK_MATCH(BeatmapDataLoaderVersion2_6_0AndEarlier_BeatmapDataLoader_GetBeatmapDataBasicInfoFromSaveDataJson, 
                    &BeatmapDataLoaderVersion2_6_0AndEarlier::BeatmapDataLoader::GetBeatmapDataBasicInfoFromSaveDataJson, 
                    GlobalNamespace::BeatmapDataBasicInfo*, StringW beatmapJson)
{
    if (!beatmapJson)
    {
        return nullptr;
    }
    BeatmapSaveDataVersion2_6_0AndEarlier::BeatmapSaveData* beatmapSaveData = UnityEngine::JsonUtility::FromJson<BeatmapSaveDataVersion2_6_0AndEarlier::BeatmapSaveData*>(beatmapJson);
    if (beatmapSaveData == nullptr)
    {
        return nullptr;
    }
    int count = 0;
    int count2 = beatmapSaveData->____obstacles->get_Count();
    int count3 = 0;

    for (size_t i = 0; i < beatmapSaveData->____notes->get_Count(); i++)
    {
        auto note = beatmapSaveData->____notes->get_Item(i);
        if(note->get_type() == BeatmapSaveDataVersion2_6_0AndEarlier::NoteType::Bomb)
        {
            count3++;
        }
        else
        {
            count++;
        }
    }

    std::vector<BetterSongList::Hooks::SimpleObstacle> obstacles;
    for (int i = 0; i < count2; i++)
    {
        auto obstacle = beatmapSaveData->____obstacles->get_Item(i);
        auto layer = BeatmapDataLoaderVersion2_6_0AndEarlier::BeatmapDataLoader::ObstacleConverter::GetLayerForObstacleType(obstacle->____type);
        auto height = BeatmapDataLoaderVersion2_6_0AndEarlier::BeatmapDataLoader::ObstacleConverter::GetHeightForObstacleType(obstacle->____type);
        obstacles.push_back(BetterSongList::Hooks::SimpleObstacle(obstacle->_lineIndex, layer, obstacle->_width, height, obstacle->_duration, obstacle->_time));
    }

    auto mark = BetterSongList::Hooks::HookBeatmapDataLoader::GetMarkStatus(obstacles);
    auto beatmapDataBasicInfo = GlobalNamespace::BeatmapDataBasicInfo::New_ctor(4, count, count2 * (mark ? -1 : 1), count3);

    return beatmapDataBasicInfo;
}