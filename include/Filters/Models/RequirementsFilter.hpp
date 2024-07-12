#pragma once

#include "IFilter.hpp"
#include "GlobalNamespace/BeatmapLevel.hpp"
#include <vector>

namespace BetterSongList {
    class RequirementsFilter : public IFilter {
        public:
            /// @brief a filter for requirements
            /// @param invert if false, keeps maps WITH requirements, if true, keeps maps WITHOUT requirements
            RequirementsFilter(bool invert = false);

            virtual bool get_isReady() const override;
            virtual std::future<void> Prepare() override;
            virtual bool GetValueFor(GlobalNamespace::BeatmapLevel* level) override;
        private:
            static bool inited;
            bool invert;
    };
}