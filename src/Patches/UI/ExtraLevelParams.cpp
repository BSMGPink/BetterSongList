#include "Patches/UI/ExtraLevelParams.hpp"
#include "config.hpp"
#include "logging.hpp"

#include "Utils/SongDetails.hpp"
#include "Utils/JumpDistanceCalculator.hpp"
#include "Utils/BeatmapUtils.hpp"
#include "Utils/PPUtils.hpp"

#include "bsml/shared/Helpers/utilities.hpp"
#include "UnityEngine/WaitForEndOfFrame.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/GameObject.hpp"
#include "HMUI/ImageView.hpp"
#include "HMUI/HoverHint.hpp"
#include "HMUI/CurvedTextMeshPro.hpp"
#include "GlobalNamespace/LocalizedHoverHint.hpp"
#include "bsml/shared/BSML/SharedCoroutineStarter.hpp"
#include "GlobalNamespace/BeatmapDifficultyMethods.hpp"
#include "GlobalNamespace/BeatmapCharacteristicSO.hpp"
#include "GlobalNamespace/BeatmapDifficulty.hpp"
#include "GlobalNamespace/BeatmapKey.hpp"
#include "GlobalNamespace/StandardLevelInfoSaveData.hpp"
#include "GlobalNamespace/BeatmapBasicData.hpp"
#include "BeatmapSaveDataVersion3/BeatmapSaveData.hpp"
#include "System/IO/File.hpp"
#include "song-details/shared/SongDetails.hpp"
#include "song-details/shared/Data/MapDifficulty.hpp"
#include "song-details/shared/Data/Song.hpp"
#include "song-details/shared/Data/SongDifficulty.hpp"
#include "Utils/SongDetails.hpp"
#include "song-details/shared/DiffArray.hpp"
#include "songcore/shared/SongLoader/CustomBeatmapLevel.hpp"
#include "CustomTypes/CanvasGroupLinker.hpp"

#include "paper/shared/utfcpp/source/utf8.h"

#include <algorithm>

std::array<SongDetailsCache::MapDifficulty, 5> diffToString {
    SongDetailsCache::MapDifficulty::Easy,
    SongDetailsCache::MapDifficulty::Normal,
    SongDetailsCache::MapDifficulty::Hard,
    SongDetailsCache::MapDifficulty::Expert,
    SongDetailsCache::MapDifficulty::ExpertPlus
};

// Map for difficulties
static const std::unordered_map<int, SongDetailsCache::MapDifficulty> diffMap = {
    
};

SongDetailsCache::MapDifficulty BeatmapDifficultyToString(int value) {
    return diffToString.at(value);
}

namespace BetterSongList::Hooks {
    SafePtrUnity<GlobalNamespace::StandardLevelDetailView> ExtraLevelParams::lastInstance;
    SafePtrUnity<UnityEngine::GameObject> ExtraLevelParams::extraUI;
    SafePtr<Array<TMPro::TextMeshProUGUI*>> ExtraLevelParams::fields;
    SafePtrUnity<HMUI::HoverHintController> ExtraLevelParams::hoverHintController;

    GlobalNamespace::StandardLevelDetailView* ExtraLevelParams::get_lastInstance() {
        if (!lastInstance) {
            return nullptr;
        }
        return lastInstance.ptr();
    }

    UnityEngine::GameObject* ExtraLevelParams::get_extraUI() {
        if (!extraUI) {
            return nullptr;
        }
        return extraUI.ptr();
    }

    ArrayW<TMPro::TextMeshProUGUI*> ExtraLevelParams::get_fields() {
        if (!fields) {
            return nullptr;
        }
        return fields.ptr();
    }

    HMUI::HoverHintController* ExtraLevelParams::get_hoverHintController() {
        if (!hoverHintController) {
            return nullptr;
        }
        return hoverHintController.ptr();
    }

    int monthsAgoFromUnixTimestamp(time_t timestamp) {
        // Get current time in UTC
        time_t now = time(nullptr);

        // Convert the Unix timestamp to a tm structure in UTC
        std::tm timeinfo = *std::gmtime(&timestamp);

        // Convert current time to a tm structure in UTC
        std::tm nowinfo = *std::gmtime(&now);

        // Calculate the difference in years and months
        int yearsDiff = nowinfo.tm_year - timeinfo.tm_year;
        int monthsDiff = nowinfo.tm_mon - timeinfo.tm_mon;

        // Adjust months difference for negative values
        if (monthsDiff < 0) {
            monthsDiff += 12;
            yearsDiff--;
        }

        // Convert years to months and add to months difference
        monthsDiff += yearsDiff * 12;

        return monthsDiff;
    }

    void ExtraLevelParams::StandardLevelDetailView_RefreshContent_Postfix(GlobalNamespace::StandardLevelDetailView* self, GlobalNamespace::BeatmapLevel* level, GlobalNamespace::BeatmapKey selectedDifficultyBeatmap, GlobalNamespace::LevelParamsPanel* levelParamsPanel) {
        if (!get_extraUI() || !get_extraUI()->___m_CachedPtr.m_value) {
            extraUI = UnityEngine::Object::Instantiate(levelParamsPanel->get_gameObject(), levelParamsPanel->get_transform()->get_parent());
            UnityEngine::Object::Destroy(extraUI->GetComponent<GlobalNamespace::LevelParamsPanel*>());
            auto linker = levelParamsPanel->get_gameObject()->AddComponent<BetterSongList::CanvasGroupLinker*>();
            linker->baseCanvasGroup = levelParamsPanel->GetComponent<UnityEngine::CanvasGroup*>();
            linker->targetCanvasGroup = extraUI->GetComponent<UnityEngine::CanvasGroup*>();
            linker->OnCanvasGroupChanged = []() {
                auto level = ExtraLevelParams::get_lastInstance()->____beatmapLevel;
                auto selectedDifficultyBeatmap = ExtraLevelParams::get_lastInstance()->get_beatmapKey();
                auto levelParamsPanel = ExtraLevelParams::get_lastInstance()->____levelParamsPanel;

                auto obstaclesText = levelParamsPanel->____obstaclesCountText;
                auto txt = obstaclesText->get_text();
                int obstacleCount = 0;
                std::istringstream(static_cast<std::string>(txt)) >> obstacleCount;

                obstaclesText->set_fontStyle(TMPro::FontStyles::Italic);
                if (config.get_showWarningIfMapHasCrouchWallsBecauseMappersThinkSprinklingThemInRandomlyIsFun()) {
                    obstaclesText->set_richText(true);
                    if(obstacleCount < 0) {
                        obstaclesText->set_fontStyle(TMPro::FontStyles::Normal);
                        obstaclesText->set_text(fmt::format("<i>{}</i> <b><size=3.3><color=#FF0>⚠</color></size></b>", std::abs(obstacleCount)));
                    }
                }
            };

            auto pos = levelParamsPanel->get_transform()->get_localPosition();
            pos.y += 3.5f;
            levelParamsPanel->get_transform()->set_localPosition(pos);
            pos = extraUI->get_transform()->get_localPosition();
            pos.y -= 1.0f;
            extraUI->get_transform()->set_localPosition(pos);

            fields.emplace(static_cast<Array<TMPro::TextMeshProUGUI*>*>(extraUI->GetComponentsInChildren<HMUI::CurvedTextMeshPro*>(true).convert()));
            BSML::SharedCoroutineStarter::get_instance()->StartCoroutine(custom_types::Helpers::CoroutineHelper::New(ProcessFields()));
        }

        lastInstance = self;

        auto basicData = level->GetDifficultyBeatmapData(selectedDifficultyBeatmap.beatmapCharacteristic, selectedDifficultyBeatmap.difficulty);

        if (get_fields()) {
            auto fieldsW = get_fields();
            if (!BetterSongList::SongDetails::get_isAvailable()) {
                INFO("No song details available");
                fieldsW[0]->set_text("N/A");
                fieldsW[1]->set_text("N/A");
            } else if (BetterSongList::SongDetails::get_songDetails()->songs.size() != 0) {
                INFO("details available, not empty");
                auto characteristic = selectedDifficultyBeatmap.beatmapCharacteristic;
                auto ch = characteristic ? BetterSongList::SongDetails::BeatmapCharacteristicToBeatStarCharacteristic(characteristic) : SongDetailsCache::MapCharacteristic::Custom;

                if (ch != SongDetailsCache::MapCharacteristic::Standard) {
                    INFO("Characteristic was not standard");
                    fieldsW[0]->set_text("-");
                    fieldsW[1]->set_text("-");
                } else {
                    INFO("Characteristic was standard");
                    auto hash = BeatmapUtils::GetHashOfPreview(level);
                    INFO("Got hash: {}", hash);
                    const SongDetailsCache::Song* song = nullptr;
                    const SongDetailsCache::SongDifficulty* diff = nullptr;
                    
                    // Get song and difficulty
                    {
                        if (!hash.empty()) {
                            auto &songRef = SongDetails::get_songDetails()->songs.FindByHash(hash);
                            if (songRef != SongDetailsCache::Song::none) {
                                song = &songRef;
                            }
                            if (song != nullptr) {
                                auto & diffRef = song->GetDifficulty(BeatmapDifficultyToString(selectedDifficultyBeatmap.difficulty.value__), ch);
                                if (diffRef != SongDetailsCache::SongDifficulty::none) {
                                    diff = &diffRef;
                                }
                            }
                        }
                    }
                    
                    if (hash.empty() || // no hash
                        !song || // song not found
                        !diff // diff not found
                    ) {
                        INFO("No diff|song|hash found, setting to ?");
                        fieldsW[0]->set_text("?");
                        fieldsW[1]->set_text("?");
                        fieldsW[3]->set_text("?");
                    } else {
                        INFO("diff was found and ranked, now to show those values");
                        bool isSs = config.get_preferredLeaderboardAsString() == "ScoreSaber";
                        float stars = isSs ? diff->starsSS : diff->starsBL;

                        if (stars <= 0) {
                            fieldsW[0]->set_text("-");
                            fieldsW[1]->set_text("-");
                        } else if (isSs) {
                            auto acc = 0.984f - (std::max(0.0f, (diff->starsSS - 1.5f) / (12.5f - 1.5f) / config.get_accuracyMultiplier()) * .027f);
                            auto pp = PPUtils::PPPercentage(acc) * diff->starsSS * 42.1f;

                            fieldsW[0]->set_text(fmt::format("{0} <size=2.5>({1:0.0f})</size>", (int)pp, acc));
                            fieldsW[1]->set_text(fmt::format("{:1.1f}", diff->starsSS));

                        } else {
                            fieldsW[0]->set_text("?");
                            fieldsW[1]->set_text(fmt::format("{:1.1f}", diff->starsBL));
                        }
                    }

                    if(song) {
                        auto unixDate = (int)song->uploadTimeUnix;

                        fieldsW[3]->set_text(fmt::format("{}", monthsAgoFromUnixTimestamp(unixDate)));
                    }
                }
            } else if (!BetterSongList::SongDetails::get_finishedInitAttempt()){
                INFO("details available, but empty and not yet fetched");
                fieldsW[0]->set_text("...");
                fieldsW[1]->set_text("...");
            }

            auto customBeatmapLevelOpt = il2cpp_utils::try_cast<SongCore::SongLoader::CustomBeatmapLevel>(level);
            if (customBeatmapLevelOpt) {
                fieldsW[2]->set_text(fmt::format("{:1.1f}", basicData->___noteJumpMovementSpeed));
            } else {
                fieldsW[2]->set_text("?");
            }
        } else {
            ERROR("Fields was nullptr!");
        }
    }

    void ExtraLevelParams::UpdateState() {
        auto inst = get_lastInstance();
        if (inst && inst->___m_CachedPtr.m_value && inst->get_isActiveAndEnabled()) inst->RefreshContent();
    }

    void ExtraLevelParams::ModifyValue(TMPro::TextMeshProUGUI* text, std::string_view hoverHint, std::string_view icon) {
        DEBUG("Get go");
        auto go = text->get_gameObject();
        go->SetActive(true);
        BSML::Utilities::SetImage(text->get_transform()->get_parent()->Find("Icon")->get_gameObject()->GetComponent<HMUI::ImageView*>(), icon);
        DEBUG("get hhint");
        auto localizedhhint = go->GetComponentInParent<GlobalNamespace::LocalizedHoverHint*>();
        if (localizedhhint) UnityEngine::Object::DestroyImmediate(localizedhhint);

        DEBUG("get hhint");
        auto hhint = go->GetComponentInParent<HMUI::HoverHint*>();

        if (!hhint) return;

        if (!get_hoverHintController() || !get_hoverHintController()->___m_CachedPtr.m_value)
            hoverHintController = UnityEngine::Object::FindObjectOfType<HMUI::HoverHintController*>();
        hhint->____hoverHintController = get_hoverHintController();
        hhint->set_text(hoverHint);
    }

    custom_types::Helpers::Coroutine ExtraLevelParams::ProcessFields() {
        if (!get_fields()) {
            ERROR("Fields were not set, returning");
            co_return;
        }

        co_yield reinterpret_cast<System::Collections::IEnumerator*>(UnityEngine::WaitForEndOfFrame::New_ctor());
        auto fieldsW = get_fields();

        ModifyValue(fieldsW[0], "ScoreSaber PP Value", "#DifficultyIcon");
		ModifyValue(fieldsW[1], "Star Rating", "#FavoritesIcon");
		ModifyValue(fieldsW[2], "NJS (Note Jump Speed)", "#FastNotesIcon");
		ModifyValue(fieldsW[3], "BeatSaver upload age (Months)", "#ClockIcon");

        fieldsW[0]->set_richText(true);
        fieldsW[0]->set_characterSpacing(-3.0f);
        fieldsW[3]->set_richText(true);
    }
}
