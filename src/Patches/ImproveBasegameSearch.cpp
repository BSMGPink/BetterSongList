#include "Patches/ImproveBasegameSearch.hpp"
#include "config.hpp"
#include "GlobalNamespace/LevelFilter.hpp"

#include <map>
namespace BetterSongList::Hooks {
    bool ImproveBasegameSearch::LevelFilter_FilterLevelByText_Prefix(System::Collections::Generic::List_1<GlobalNamespace::BeatmapLevel*>* levels, ArrayW<StringW>& searchTexts, System::Collections::Generic::List_1<GlobalNamespace::BeatmapLevel*>*& result) {
        if (!config.get_modBasegameSearch()) return false;

        auto intermediate = std::vector<std::pair<int, GlobalNamespace::BeatmapLevel*>>{};
        
        for(auto i = 0; i < levels->get_Count(); i++) {
            auto level = levels->get_Item(i);
            auto txtToClean = fmt::format("{} {} {} {}", level->___songName, level->___songSubName, level->___songAuthorName, fmt::join(level->___allMappers, " "));
            auto cleanedText = GlobalNamespace::LevelFilter::CleanText(txtToClean);
            auto score = GlobalNamespace::LevelFilter::_FilterLevelByText_g__CalculateMatchScore_14_1(cleanedText, searchTexts);
            if(score > 0) {
                intermediate.push_back({score, level});
            }
        }

        //sort the intermediate list by score
        std::sort(intermediate.begin(), intermediate.end(), [](auto a, auto b) {
            return a.first > b.first;
        });

        ListW<GlobalNamespace::BeatmapLevel*> resultLevels = ListW<GlobalNamespace::BeatmapLevel*>::New();

        for(auto& [score, level] : intermediate) {
            resultLevels.push_back(level);
        }
        
        result = resultLevels;

        return true;
    }
}