#include "Sorters/Models/FolderDateSorter.hpp"
#include "logging.hpp"

#include "Utils/SongListLegendBuilder.hpp"

#include "GlobalNamespace/BeatmapLevelPack.hpp"
#include "GlobalNamespace/BeatmapLevel.hpp"

#include "bsml/shared/BSML/MainThreadScheduler.hpp"

#include <sys/stat.h>
#include <time.h>

static const float MONTH_SECS = 1.0f / (60 * 60 * 24 * 30.4f);

namespace BetterSongList {
    std::map<std::string, int> FolderDateSorter::songTimes;

    bool FolderDateSorter::isLoading = false;

    FolderDateSorter::FolderDateSorter() : ISorterWithLegend(), ISorterPrimitive() {}

    bool FolderDateSorter::get_isReady() const {
        return !songTimes.empty();
    }

    std::future<void> FolderDateSorter::Prepare() {
        return Prepare(false);
    }

    void FolderDateSorter::OnSongsLoaded(const std::vector<SongCore::SongLoader::CustomBeatmapLevel*>& songs) {
        Prepare(false);
    }

    void FolderDateSorter::GatherFolderInfoThread(bool fullReload) {
        
        auto levels = SongCore::API::Loading::GetAllLevels();

        for (auto level : levels) {
            std::string levelID = level->levelID;
            auto itr = songTimes.find(levelID);
            if (itr != songTimes.end() && !fullReload) continue;

            struct stat fileStat = {0};
            std::string filePath = std::string(level->get_customLevelPath());
            if (stat(filePath.c_str(), &fileStat) == 0) {
                songTimes[levelID] = fileStat.st_ctim.tv_sec;
            }
        }

        isLoading = false;
    }

    std::future<void> FolderDateSorter::Prepare(bool fullReload) {
        if (isLoading) {
            return std::async(std::launch::deferred, []{});
        }
        isLoading = true;
        return std::async(std::launch::async, [fullReload, this](){
            auto hasLoaded = SongCore::API::Loading::AreSongsLoaded();
            while(!hasLoaded) std::this_thread::yield();
            
            this->FolderDateSorter::GatherFolderInfoThread(fullReload);
        });
    }

    std::optional<float> FolderDateSorter::GetValueFor(GlobalNamespace::BeatmapLevel* level) const {
		std::string levelId = level ? level->levelID : "";
        if (levelId.empty()) return std::nullopt;
        auto itr = songTimes.find(levelId);
        if (itr != songTimes.end()) return itr->second;
        return std::nullopt;
    }

    ISorterWithLegend::Legend FolderDateSorter::BuildLegend(ArrayW<GlobalNamespace::BeatmapLevel*> levels) const {
        time_t now = time(NULL);
        return SongListLegendBuilder::BuildFor(levels, [now](GlobalNamespace::BeatmapLevel* level) -> std::string {
            std::string levelId = level ? level->levelID : "";
            if (levelId.empty()) return "";
            auto itr = songTimes.find(levelId);
            if (itr == songTimes.end()) return "";
            auto months = (now - itr->second) * MONTH_SECS;

            if (months < 1.0f) return "<1 M";

            return fmt::format("{} M", (int)months);
        });
    }
}