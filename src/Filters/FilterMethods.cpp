#include "Filters/FilterMethods.hpp"
#include "logging.hpp"
#include "config.hpp"

#include "Filters/Models/BasicSongDetailsFilter.hpp"
#include "Filters/Models/FunctionFilter.hpp"
#include "Filters/Models/PlayedFilter.hpp"
#include "Filters/Models/RequirementsFilter.hpp"

namespace BetterSongList {
    static BasicSongDetailsFilter blqualified([](const SongDetailsCache::Song* x){ 
        return hasFlags(x->rankedStates, SongDetailsCache::RankedStates::BeatleaderQualified);
    });

    static BasicSongDetailsFilter blranked([](const SongDetailsCache::Song* x){ 
        return hasFlags(x->rankedStates, SongDetailsCache::RankedStates::BeatleaderRanked);
    });

    // Legacy ranked filter
    static BasicSongDetailsFilter ranked([](const SongDetailsCache::Song* x){ 
        return hasFlags(x->rankedStates, SongDetailsCache::RankedStates::ScoresaberRanked );
    });
    
    static BasicSongDetailsFilter unranked([](const SongDetailsCache::Song* x){ 
        return x->rankedStatus == SongDetailsCache::RankedStatus::Unranked;
    });

    static BasicSongDetailsFilter qualified([](const SongDetailsCache::Song* x) {
        return x->rankedStatus == SongDetailsCache::RankedStatus::Qualified;
    });

    static BasicSongDetailsFilter onBeatsaver([](const SongDetailsCache::Song* x) { return true; });

    static PlayedFilter unplayed(true);
    static PlayedFilter played{};

    static RequirementsFilter requirements(false);
    static RequirementsFilter noRequirements(true);

    const std::map<std::string, IFilter*>& FilterMethods::get_methods() {
        return methods;
    }

    bool FilterMethods::Register(ITransformerPlugin* filter) {
        auto name = filter->get_name();
        if (name.size() > 20) {
            name = name.substr(0, 20);
            ERROR("Name of the Transformer can not be more than 20 characters!");
        }

        if (!config.get_allowPluginSortsAndFilters()) return false;
        name = "P " + name;

        auto itr = methods.find(name);
        if (itr != methods.end()) {
            ERROR("filter with name {} was already registerd!", name);
            return false;
        }

        methods.insert({ name, reinterpret_cast<IFilter*>(filter) });
        return true;
    }

    std::map<std::string, IFilter*> FilterMethods::methods{
        {"BL Qualified", &blqualified},
        {"BL Ranked", &blranked},
        {"SS Ranked", &ranked},
		{"Unplayed", &unplayed},
		{"Played", &played},
		{"Requirements", &requirements},
		{"No Requirements", &noRequirements},
		{"Unranked", &unranked},
		{"All", nullptr}
    };
}