#pragma once

#include "custom-types/shared/coroutine.hpp"
#include "custom-types/shared/macros.hpp"
#include "bsml/shared/BSML.hpp"
#include "bsml/shared/BSML/Components/ClickableText.hpp"
#include "bsml/shared/BSML/Components/ClickableImage.hpp"
#include "bsml/shared/BSML/Components/ModalView.hpp"
#include "bsml/shared/BSML/Components/Settings/DropdownListSetting.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "HMUI/DropdownWithTableView.hpp"
#include "TMPro/TextMeshProUGUI.hpp"

#include "ISorter.hpp"
#include "IFilter.hpp"

#include <queue>

DECLARE_CLASS_CODEGEN(BetterSongList, FilterUI, Il2CppObject,
    DECLARE_INSTANCE_FIELD(UnityEngine::RectTransform*, root);
    DECLARE_INSTANCE_FIELD(BSML::DropdownListSetting*, sortDropDown);
    DECLARE_INSTANCE_FIELD(BSML::DropdownListSetting*, filterDropDown);
    DECLARE_INSTANCE_FIELD(ListW<StringW>, sortOptionsList);
    DECLARE_INSTANCE_FIELD(ListW<StringW>, filterOptionsList);
    DECLARE_INSTANCE_FIELD(bool, warningLoadInProgress);
    DECLARE_INSTANCE_FIELD(HMUI::ImageView*, filterLoadingIndicator);
    DECLARE_INSTANCE_FIELD(BSML::ClickableImage*, sortDirection);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, failTextLabel);
    DECLARE_INSTANCE_FIELD(bool, settingsWereOpened);
    DECLARE_INSTANCE_FIELD(BSML::ClickableImage*, settingsButton);
    DECLARE_INSTANCE_FIELD(BSML::ModalView*, incompatibilityModal);

    DECLARE_INSTANCE_METHOD(void, UpdateDropdowns);
    DECLARE_INSTANCE_METHOD(void, UpdateTransformerOptionsAndDropdowns);
    DECLARE_INSTANCE_METHOD(void, ToggleSortDirection);
    DECLARE_INSTANCE_METHOD(void, SelectRandom);
    DECLARE_INSTANCE_METHOD(void, _SetSort, StringW selected);
    DECLARE_INSTANCE_METHOD(void, _SetFilter, StringW selected);
    DECLARE_INSTANCE_METHOD(void, PossiblyShowNextWarning);
    DECLARE_INSTANCE_METHOD(void, SettingsOpened);
    DECLARE_INSTANCE_METHOD(void, PostParse);
    DECLARE_INSTANCE_METHOD(void, CloseWarningModal);
    
    DECLARE_CTOR(ctor);
    public:
		static void UpdateVisibleTransformers();
        static FilterUI* get_instance();
		static void SetSort(std::string selected, bool storeToConfig = true, bool refresh = true);
		static void ClearFilter(bool reloadTable = false);
		static void SetFilter(std::string selected, bool storeToConfig = true, bool refresh = true);
		static void SetSortDirection(bool ascending, bool refresh = true);
		static void Init();
		static void AttachTo(UnityEngine::Transform* target);
		static void HackDropdown(HMUI::DropdownWithTableView* dropdown);
        void ShowErrorASAP(std::string_view text = "");

    private:
        static SafePtr<FilterUI> instance;
        static std::map<std::string, BetterSongList::ISorter*> sortOptions;
        static std::map<std::string, BetterSongList::IFilter*> filterOptions;
        
        std::queue<std::string> warnings;

        custom_types::Helpers::Coroutine _ShowError();
)