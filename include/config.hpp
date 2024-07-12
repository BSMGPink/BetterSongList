#pragma once

#include <string>
#include "GlobalNamespace/SelectLevelCategoryViewController.hpp"
// honestly fuck typing this out completely every time
using LevelCategory = GlobalNamespace::SelectLevelCategoryViewController::LevelCategory;

bool LoadConfig();
void SaveConfig();

struct Config {
    public:
        const auto& get_lastSong() { return lastSong; }
        void set_lastSong(const auto& value) { lastSong = value; SaveConfig(); }
        const auto& get_lastPack() { return lastPack; }
        void set_lastPack(const auto& value) { lastPack = value; SaveConfig(); }
        const auto& get_lastSort() { return lastSort; }
        void set_lastSort(const auto& value) { lastSort = value; SaveConfig(); }
        const auto& get_lastFilter() { return lastFilter; }
        void set_lastFilter(const auto& value) { lastFilter = value; SaveConfig(); }
        auto get_lastCategory() { return lastCategory; }
        void set_lastCategory(auto value) { lastCategory = value; SaveConfig(); }
        auto get_enableAlphabetScrollBar() { return enableAlphabetScrollBar; }
        void set_enableAlphabetScrollBar(auto value) { enableAlphabetScrollBar = value; SaveConfig(); }
        auto get_clearFiltersOnPlaylistSelect() { return clearFiltersOnPlaylistSelect; }
        void set_clearFiltersOnPlaylistSelect(auto value) { clearFiltersOnPlaylistSelect = value; SaveConfig(); }
        auto get_modBasegameSearch() { return modBasegameSearch; }
        void set_modBasegameSearch(auto value) { modBasegameSearch = value; SaveConfig(); }
        auto get_autoFilterUnowned() { return autoFilterUnowned; }
        void set_autoFilterUnowned(auto value) { autoFilterUnowned = value; SaveConfig(); }
        auto get_extendSongScrollbar() { return extendSongScrollbar; }
        void set_extendSongScrollbar(auto value) { extendSongScrollbar = value; SaveConfig(); }
        auto get_allowWipDelete() { return allowWipDelete; }
        void set_allowWipDelete(auto value) { allowWipDelete = value; SaveConfig(); }
        auto get_showWarningIfMapHasCrouchWallsBecauseMappersThinkSprinklingThemInRandomlyIsFun() { return showWarningIfMapHasCrouchWallsBecauseMappersThinkSprinklingThemInRandomlyIsFun; }
        void set_showWarningIfMapHasCrouchWallsBecauseMappersThinkSprinklingThemInRandomlyIsFun(auto value) { showWarningIfMapHasCrouchWallsBecauseMappersThinkSprinklingThemInRandomlyIsFun = value; SaveConfig(); }
        auto get_showMapJDInsteadOfOffset() { return showMapJDInsteadOfOffset; }
        void set_showMapJDInsteadOfOffset(auto value) { showMapJDInsteadOfOffset = value; SaveConfig(); }
        auto get_accuracyMultiplier() { return accuracyMultiplier; }
        void set_accuracyMultiplier(auto value) { accuracyMultiplier = value; SaveConfig(); }
        auto get_allowPluginSortsAndFilters() { return allowPluginSortsAndFilters; }
        void set_allowPluginSortsAndFilters(auto value) { allowPluginSortsAndFilters = value; SaveConfig(); }
        auto get_sortAsc() { return sortAsc; }
        void set_sortAsc(auto value) { sortAsc = value; SaveConfig(); }
        const auto& get_settingsSeenInVersion() { return settingsSeenInVersion; }
        void set_settingsSeenInVersion(const auto& value) { settingsSeenInVersion = value; SaveConfig(); }
        StringW get_preferredLeaderboard() { return preferredLeaderboard; } 
        // This is for stability, because I don't know if it will cause crashes with StringW
        std::string get_preferredLeaderboardAsString() { return preferredLeaderboard; } 
        void set_preferredLeaderboard(StringW value) { preferredLeaderboard = std::string(value); SaveConfig(); }

        void SaveConfig();
        bool LoadConfig();
    private:
        std::string lastSong = "";
        std::string lastPack = "";
        std::string lastSort = "";
        std::string lastFilter = "";
        LevelCategory lastCategory = LevelCategory::All;
        bool enableAlphabetScrollBar = true;
        bool clearFiltersOnPlaylistSelect = true;
        bool modBasegameSearch = true;
        bool autoFilterUnowned = true;
        bool extendSongScrollbar = true;
        bool allowWipDelete = false;
        bool showWarningIfMapHasCrouchWallsBecauseMappersThinkSprinklingThemInRandomlyIsFun = true;
        bool showMapJDInsteadOfOffset = true;
        float accuracyMultiplier = 1.0f;
        bool allowPluginSortsAndFilters = true;
        bool sortAsc = false;
        std::string settingsSeenInVersion = "";
        // ScoreSaber or BeatLeader
        std::string preferredLeaderboard = "ScoreSaber";
};

extern Config config;

#ifndef BSML_PROPERTY_DEFINITION
#define BSML_PROPERTY_DEFINITION(type, name)                                                                            \
public:                                                                                                                 \
type get_##name() { return config.get_##name(); }                                                                       \
___CREATE_INSTANCE_METHOD(get_##name, "get_" #name, METHOD_ATTRIBUTE_PUBLIC | METHOD_ATTRIBUTE_HIDE_BY_SIG, nullptr);   \
void set_##name(type value) { config.set_##name(value); }                                                               \
___CREATE_INSTANCE_METHOD(set_##name, "set_" #name, METHOD_ATTRIBUTE_PUBLIC | METHOD_ATTRIBUTE_HIDE_BY_SIG, nullptr)
#endif