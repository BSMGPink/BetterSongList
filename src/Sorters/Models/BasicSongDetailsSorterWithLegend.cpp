#include "Sorters/Models/BasicSongDetailsSorterWithLegend.hpp"
#include "logging.hpp"

#include "Utils/SongDetails.hpp"
#include "Utils/BeatmapUtils.hpp"
#include "Utils/SongListLegendBuilder.hpp"

namespace BetterSongList {
    BasicSongDetailsSorterWithLegend::BasicSongDetailsSorterWithLegend(
        BasicSongDetailsSorterWithLegend::ValueGetterFunc sortFunc
    ) : 
        BasicSongDetailsSorterWithLegend(sortFunc, std::bind(&BasicSongDetailsSorterWithLegend::DefaultLegendGetter, this, std::placeholders::_1))
    {}

    BasicSongDetailsSorterWithLegend::BasicSongDetailsSorterWithLegend(
        BasicSongDetailsSorterWithLegend::ValueGetterFunc sortFunc, 
        BasicSongDetailsSorterWithLegend::LegendGetterFunc legendFunc
    ) : 
        ISorterWithLegend(),
        ISorterPrimitive(),
        IAvailabilityCheck(),
        sortValueGetter(sortFunc), 
        legendValueGetter(legendFunc) {}

    std::string BasicSongDetailsSorterWithLegend::DefaultLegendGetter(const SongDetailsCache::Song* song) const {
        auto v = sortValueGetter(song);
        return v.has_value() ? fmt::format("{}", v.value()) : "";
    }

    bool BasicSongDetailsSorterWithLegend::get_isReady() const {
        return SongDetails::get_finishedInitAttempt();
    }

    std::future<void> BasicSongDetailsSorterWithLegend::Prepare() {
        return std::async(std::launch::async, []{
            SongDetails::Init();

            while(!SongDetails::get_finishedInitAttempt()) {
                std::this_thread::yield();
            }
        });
    }

    ISorterWithLegend::Legend BasicSongDetailsSorterWithLegend::BuildLegend(ArrayW<GlobalNamespace::BeatmapLevel*> levels) const {
        if (
            !SongDetails::get_songDetails()->songs.get_isDataAvailable() ||
            SongDetails::get_songDetails()->songs.size() == 0
        ) return {};

        return SongListLegendBuilder::BuildFor(levels, [legendValueGetter = legendValueGetter](GlobalNamespace::BeatmapLevel* level) -> std::string {
            auto h = BeatmapUtils::GetHashOfPreview(level);
            if (h.empty()) return "N/A";

            auto &song = SongDetails::get_songDetails()->songs.FindByHash(h);
            if (song == SongDetailsCache::Song::none ) return "N/A";

            return legendValueGetter(&song);
        });
    }

    std::optional<float> BasicSongDetailsSorterWithLegend::GetValueFor(GlobalNamespace::BeatmapLevel* level) const {
        if (
            !SongDetails::get_songDetails()->songs.get_isDataAvailable() ||
            SongDetails::get_songDetails()->songs.size() == 0
        ) return std::nullopt;

        std::string h = BeatmapUtils::GetHashOfPreview(level);
        if (h.empty()) return std::nullopt;

        auto &song = SongDetails::get_songDetails()->songs.FindByHash(h);
        if (song == SongDetailsCache::Song::none ) return std::nullopt;

        return sortValueGetter(&song);
    }

    std::string BasicSongDetailsSorterWithLegend::GetUnavailableReason() const {
        return SongDetails::GetUnavailabilityReason();
    }
}