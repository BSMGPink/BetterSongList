#pragma once

#include "beatsaber-hook/shared/utils/typedefs.h"
#include "GlobalNamespace/BeatmapLevel.hpp"
#include <unordered_map>
#include <optional>
#include <future>

namespace BetterSongList {
    class ISorter {
        public:
            /// @brief cast the sorter to a derived type
            /// @tparam T the derived type to cast to
            /// @return this as derived type or nullptr if invalid
            template<typename T>
            constexpr T as() const {
                return dynamic_cast<T>(this);
            }

            /// @brief cast the sorter to a derived type
            /// @tparam T the derived type to cast to
            /// @return this as derived type or nullptr if invalid
            template<typename T>
            constexpr T as() {
                return dynamic_cast<T>(this);
            }

            constexpr ISorter() {}
            virtual bool get_isReady() const = 0;
            virtual std::future<void> Prepare() = 0;
    };

    class ISorterWithLegend {
        public:
            struct LegendPair {
                LegendPair(const std::string& str, const int& num) : first(str), second(num) {}
                std::string first;
                int second;
            };
            
            using Legend = std::vector<LegendPair>;
            constexpr ISorterWithLegend() {};
            virtual Legend BuildLegend(ArrayW<GlobalNamespace::BeatmapLevel*> levels) const = 0;
    };

    class ISorterCustom : public ISorter {
        public:
            constexpr ISorterCustom() : ISorter() {}
            virtual void DoSort(ArrayW<GlobalNamespace::BeatmapLevel*>& levels, bool ascending) const = 0;
    };

    class ISorterPrimitive : public ISorter {
        public:
            constexpr ISorterPrimitive() : ISorter() {}
            virtual std::optional<float> GetValueFor(GlobalNamespace::BeatmapLevel* level) const = 0;
    };
}