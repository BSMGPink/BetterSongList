#pragma once

#include "ISorter.hpp"
#include "GlobalNamespace/BeatmapLevel.hpp"
#include <map>

#include "songcore/shared/SongCore.hpp"
#include "songcore/shared/SongLoader/CustomBeatmapLevel.hpp"

namespace BetterSongList {
    class FolderDateSorter : public ISorterWithLegend, public ISorterPrimitive {
        public:
            FolderDateSorter();
            virtual bool get_isReady() const override;
            virtual std::future<void> Prepare() override;
            std::future<void> Prepare(bool fullReload);
            virtual std::optional<float> GetValueFor(GlobalNamespace::BeatmapLevel* level) const override;
            virtual Legend BuildLegend(ArrayW<GlobalNamespace::BeatmapLevel*> levels) const override;
        private:
            void OnSongsLoaded(const std::vector<SongCore::SongLoader::CustomBeatmapLevel*>& songs);
            void GatherFolderInfoThread(bool fullReload = false);
            static std::map<std::string, int> songTimes;
            static bool isLoading;
    };
}