#pragma once

#include "IFilter.hpp"
#include "Utils/IAvailabilityCheck.hpp"
#include "song-details/shared/SongDetails.hpp"
#include "song-details/shared/Data/Song.hpp"

namespace BetterSongList {
    class BasicSongDetailsFilter : public IFilter, public IAvailabilityCheck {
        public:
            BasicSongDetailsFilter(const std::function<bool(const SongDetailsCache::Song*)>& func);

            virtual bool get_isReady() const override;
            virtual std::future<void> Prepare() override;
            virtual bool GetValueFor(GlobalNamespace::BeatmapLevel* level) override;

            virtual std::string GetUnavailableReason() const override;
        private:
            std::function<bool(const SongDetailsCache::Song*)> filterValueTransformer;
    };
}