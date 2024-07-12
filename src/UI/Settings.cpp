#include "UI/Settings.hpp"
#include "assets.hpp"

#include "bsml/shared/BSML.hpp"

DEFINE_TYPE(BetterSongList, Settings);

namespace BetterSongList {
    SafePtr<Settings> Settings::instance;

    Settings* Settings::get_instance() {
        if (!instance || !instance.ptr()) {
            instance.emplace(Settings::New_ctor());
        }
        return instance.ptr();
    }

    std::string Settings::get_version() {
        return "BetterSongList v" VERSION " port by RedBrumbler";
    }

    void Settings::PostParse() {
        versionText->set_text(get_version());
        inited = true;
    }

    void Settings::OpenSponsorModal() {
        SettingsClosed();
        sponsorModal->Show();
        // TODO: sponsor text
    }

    void Settings::SettingsClosed() {
        settingsModal->Hide();
    }

    void Settings::Init(UnityEngine::Transform* parent) {
        if (inited && this->parent && this->parent->___m_CachedPtr.m_value) return;
        this->parent = parent;
        BSML::parse_and_construct(IncludedAssets::Settings_bsml, parent, reinterpret_cast<System::Object*>(this));
    }
}