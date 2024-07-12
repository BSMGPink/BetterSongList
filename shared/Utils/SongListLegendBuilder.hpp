#pragma once


#include "beatsaber-hook/shared/utils/typedefs.h"
#include "GlobalNamespace/BeatmapLevel.hpp"
#include "ISorter.hpp"
#include <unordered_map>
#include <algorithm>
#include <string>

namespace BetterSongList::SongListLegendBuilder {
    /// @brief just a way to add information to a vector. you shouldn't really use this yourself probably
    struct GroupedLegendPairs : std::vector<BetterSongList::ISorterWithLegend::LegendPair> {
        GroupedLegendPairs(const std::string& group) : std::vector<BetterSongList::ISorterWithLegend::LegendPair>(), group(group) {}
        GroupedLegendPairs(const std::string& group, const BetterSongList::ISorterWithLegend::LegendPair& initialPair) : GroupedLegendPairs(group) { emplace_back(initialPair); }
        bool operator>(const std::string& a) const { return group < a; }
        bool operator<(const std::string& a) const { return group < a; }
        bool operator==(const std::string& a) const { return group == a; }
        std::string group;
    };
    
    // this method is public for your convenience because building a legend is hell if you can't access this method, and just copying it can be annoying
    /// @param levels the sorted level array to build the legend for
    /// @param builder the method that makes the legend string from the BeatmapLevel
    /// @param entryLengthLimit the limit of the length for the legend entries
    /// @param valueLimit the limit for the amount of entries in the final legend
    /// @return ISorter::Legend the legend for the passed levels
    static ISorterWithLegend::Legend BuildFor(ArrayW<GlobalNamespace::BeatmapLevel*> levels, std::function<std::string(GlobalNamespace::BeatmapLevel*)> builder, int entryLengthLimit = 6, int valueLimit = 28) {
        std::vector<GroupedLegendPairs> grouped;
        // start at -1 because 0 based index, and we have to pre increment due to the continue clause
        for (int i = -1; auto& level : levels) {
            i++;
            // make the str for the Select
            auto str = builder(level);
            /// the Where from the original method
            if (str.empty()) continue;

            // Grouping by key, toUpper;
            std::string KEY = str;
            std::transform(KEY.begin(), KEY.end(), KEY.begin(), ::toupper);
            auto itr = std::find(grouped.begin(), grouped.end(), KEY);
            if (itr == grouped.end()) {
                grouped.emplace_back(KEY, ISorterWithLegend::LegendPair(str, i));
            } else {
                itr->emplace_back(str, i);
            }
        }

        int grouped_size = grouped.size();
        int amt = std::min(grouped_size, valueLimit);
        ISorterWithLegend::Legend result;
        if (amt > 1) {
            // get the keys at intervals with step size
            float step = ((float)(grouped_size - 1) / (float)(amt - 1));
            int lastIdx = grouped_size - 1;
            for (int i = 0; i < amt; i++) {
                // get index for the group
                auto bmi = (int)std::round(step * i);

                // if we end up being bigger than the size, just use the last group
                if (bmi > lastIdx) bmi = lastIdx;
                auto& itr = grouped[bmi];

                // get the group;
                auto transformedResult = itr.group;

                // if the legend text is too long, shorten it.
                if (transformedResult.size() > entryLengthLimit)
                    transformedResult = transformedResult.substr(0, entryLengthLimit);
                result.emplace_back(transformedResult, itr[0].second);
            }
        }
        return result;
    }
}