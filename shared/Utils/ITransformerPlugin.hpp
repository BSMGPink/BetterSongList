#pragma once

#include "ISorter.hpp"
#include "GlobalNamespace/SelectLevelCategoryViewController.hpp"
#include "GlobalNamespace/BeatmapLevelPack.hpp"

namespace BetterSongList {
    class ITransformerPlugin : public ISorter {
        public:
            ITransformerPlugin() : ISorter() {}
            virtual std::string get_name() const = 0;
            virtual bool get_visible() const = 0;
            virtual void ContextSwitch(GlobalNamespace::SelectLevelCategoryViewController::LevelCategory levelCategory, GlobalNamespace::BeatmapLevelPack* playlist) = 0;
    };
}