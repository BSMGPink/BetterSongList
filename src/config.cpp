#include "config.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "logging.hpp"

Config config;

Configuration& get_config() {
    static Configuration config({MOD_ID, VERSION, 0});
    config.Load();
    return config;
}

#define Save(identifier) doc.AddMember(#identifier, config.identifier, allocator)

void Config::SaveConfig() {
    INFO("Saving Configuration...");
    rapidjson::Document& doc = get_config().config;

    doc.RemoveAllMembers();
    doc.SetObject();

    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    Save(lastSong);
    Save(lastPack);
    Save(lastSort);
    Save(lastFilter);
    doc.AddMember("lastCategory", config.get_lastCategory().value__, allocator);
    Save(enableAlphabetScrollBar);
    Save(clearFiltersOnPlaylistSelect);
    Save(modBasegameSearch);
    Save(autoFilterUnowned);
    Save(extendSongScrollbar);
    Save(allowWipDelete);
    Save(showWarningIfMapHasCrouchWallsBecauseMappersThinkSprinklingThemInRandomlyIsFun);
    Save(showMapJDInsteadOfOffset);
    Save(accuracyMultiplier);
    Save(allowPluginSortsAndFilters);
    Save(sortAsc);
    Save(settingsSeenInVersion);
    Save(preferredLeaderboard);

    get_config().Write();
    INFO("Saved Configuration!");
}

#define GET_INT(identifier)                                         \
    auto identifier##_itr = doc.FindMember(#identifier);            \
    if (identifier##_itr != doc.MemberEnd())                        \
        config.identifier = identifier##_itr->value.GetInt();       \
    else                                                            \
        foundEverything = false;

#define GET_FLOAT(identifier)                                       \
    auto identifier##_itr = doc.FindMember(#identifier);            \
    if (identifier##_itr != doc.MemberEnd())                        \
        config.identifier = identifier##_itr->value.GetFloat();     \
    else                                                            \
        foundEverything = false;

#define GET_STRING(identifier)                                                          \
    auto identifier##_itr = doc.FindMember(#identifier);                                \
    if (identifier##_itr != doc.MemberEnd())                                            \
        config.identifier = std::string_view(identifier##_itr->value.GetString());      \
    else                                                                                \
        foundEverything = false;

#define GET_BOOL(identifier)                                            \
    auto identifier##_itr = doc.FindMember(#identifier);                \
    if (identifier##_itr != doc.MemberEnd())                            \
        config.identifier = identifier##_itr->value.GetBool();          \
    else                                                                \
        foundEverything = false;


bool Config::LoadConfig() {
    INFO("Loading Configuration...");
    bool foundEverything = true;
    rapidjson::Document& doc = get_config().config;

    GET_STRING(lastSong);
    GET_STRING(lastPack);
    GET_STRING(lastSort);
    GET_STRING(lastFilter);
    GET_INT(lastCategory);
    GET_BOOL(enableAlphabetScrollBar);
    GET_BOOL(clearFiltersOnPlaylistSelect);
    GET_BOOL(modBasegameSearch);
    GET_BOOL(autoFilterUnowned);
    GET_BOOL(extendSongScrollbar);
    GET_BOOL(allowWipDelete);
    GET_BOOL(showWarningIfMapHasCrouchWallsBecauseMappersThinkSprinklingThemInRandomlyIsFun);
    GET_BOOL(showMapJDInsteadOfOffset);
    GET_FLOAT(accuracyMultiplier);
    GET_BOOL(allowPluginSortsAndFilters);
    GET_BOOL(sortAsc);
    GET_STRING(settingsSeenInVersion);
    GET_STRING(preferredLeaderboard);

    if (foundEverything)
        INFO("Loaded Configuration!");
    return foundEverything;
}

void SaveConfig() {
    config.SaveConfig();
}

bool LoadConfig() {
    return config.LoadConfig();
}