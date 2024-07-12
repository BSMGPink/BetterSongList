#pragma once

#include "IFilter.hpp"

namespace BetterSongList {
    class PlayedFilter : public IFilter {
        public:
            PlayedFilter(bool unplayed = false);

            virtual bool get_isReady() const override;
            virtual std::future<void> Prepare() override;
            virtual bool GetValueFor(GlobalNamespace::BeatmapLevel* level) override;
        private:
            bool intendedPlayedState;
    };
}