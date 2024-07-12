#pragma once

#include "custom-types/shared/macros.hpp"
#include "bsml/shared/macros.hpp"
#include "bsml/shared/BSML/Components/ModalView.hpp"
#include "bsml/shared/BSML/Components/Backgroundable.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "UnityEngine/Transform.hpp"
#include "config.hpp"

DECLARE_CLASS_CODEGEN(BetterSongList, Settings, Il2CppObject,
    DECLARE_INSTANCE_METHOD(void, SettingsClosed);
    DECLARE_INSTANCE_METHOD(void, PostParse);
    DECLARE_INSTANCE_METHOD(void, OpenSponsorModal);
    DECLARE_INSTANCE_METHOD(void, Init, UnityEngine::Transform* parent);
    DECLARE_INSTANCE_FIELD(UnityEngine::Transform*, parent);
    DECLARE_INSTANCE_FIELD(BSML::ModalView*, settingsModal);
    DECLARE_INSTANCE_FIELD(BSML::ModalView*, sponsorModal);
    DECLARE_INSTANCE_FIELD(BSML::Backgroundable*, settingsTitle);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, versionText);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, sponsorText);
    DECLARE_INSTANCE_FIELD(bool, inited);

    BSML_PROPERTY_DEFINITION(bool, allowWipDelete);
    BSML_PROPERTY_DEFINITION(bool, autoFilterUnowned);
    BSML_PROPERTY_DEFINITION(bool, showWarningIfMapHasCrouchWallsBecauseMappersThinkSprinklingThemInRandomlyIsFun);
    BSML_PROPERTY_DEFINITION(bool, clearFiltersOnPlaylistSelect);
    BSML_PROPERTY_DEFINITION(bool, modBasegameSearch);
    BSML_PROPERTY_DEFINITION(bool, extendSongScrollbar);
    BSML_PROPERTY_DEFINITION(bool, showMapJDInsteadOfOffset);
    BSML_PROPERTY_DEFINITION(float, accuracyMultiplier);
    BSML_OPTIONS_LIST_OBJECT(preferredLeaderboardChoices, "ScoreSaber", "BeatLeader");
    BSML_PROPERTY_DEFINITION(StringW, preferredLeaderboard);
    
    DECLARE_DEFAULT_CTOR();
    public:
        static std::string get_version();
        static Settings* get_instance();
    private:
        static SafePtr<Settings> instance;
)