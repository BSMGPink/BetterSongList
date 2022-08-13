#include "UI/FilterUI.hpp"
#include "config.hpp"
#include "assets.hpp"
#include "logging.hpp"

#include "bsml/shared/BSML.hpp"

#include "Sorters/SortMethods.hpp"
#include "Filters/FilterMethods.hpp"
#include "Utils/ITransformerPlugin.hpp"
#include "Utils/IAvailabilityCheck.hpp"

#include "Patches/HookSelectedCategory.hpp"
#include "Patches/HookSelectedCollection.hpp"
#include "Patches/HookLevelCollectionTableSet.hpp"
#include "Patches/RestoreTableScroll.hpp"

#include "sombrero/shared/RandomUtils.hpp"
#include "sombrero/shared/MiscUtils.hpp"

#include "GlobalNamespace/LevelCollectionTableView.hpp"
#include "GlobalNamespace/SharedCoroutineStarter.hpp"
#include "UnityEngine/WaitForSeconds.hpp"

#include "cpp-semver/shared/cpp-semver.hpp"
#include <algorithm>

DEFINE_TYPE(BetterSongList, FilterUI);

using namespace UnityEngine;

namespace BetterSongList {
    using namespace Hooks;
    SafePtr<FilterUI> FilterUI::instance;
    std::map<std::string, BetterSongList::ISorter*> FilterUI::sortOptions;
    std::map<std::string, BetterSongList::IFilter*> FilterUI::filterOptions;

    void FilterUI::ctor() {
        sortOptionsList = List<Il2CppObject*>::New_ctor();
        filterOptionsList = List<Il2CppObject*>::New_ctor();
    }
    
    void FilterUI::UpdateDropdowns() {
        DEBUG("FilterUI::UpdateDropdowns");
        if (sortDropDown && sortDropDown->m_CachedPtr.m_value) {
            sortDropDown->ReloadData();
            HackDropdown(sortDropDown);
        }
        if (filterDropDown && filterDropDown->m_CachedPtr.m_value) {
            filterDropDown->ReloadData();
            HackDropdown(filterDropDown);
        }

    }

    void FilterUI::UpdateTransformerOptionsAndDropdowns() {
		UpdateVisibleTransformers();
		UpdateDropdowns();
    }

    void FilterUI::ToggleSortDirection() {
		if(HookLevelCollectionTableSet::sorter == nullptr)
            return;
        SetSortDirection(!config.sortAsc);
    }

    void FilterUI::SelectRandom() {
		auto x = UnityEngine::Object::FindObjectOfType<GlobalNamespace::LevelCollectionTableView*>();

        if (!x || !x->m_CachedPtr.m_value) return;

        auto ml = HookLevelCollectionTableSet::get_lastOutMapList();
        if (!ml) ml = HookLevelCollectionTableSet::get_lastInMapList();

        if (ml.size() < 2) return;

        // random between 0 - 1, multiply by size - 1 -> random between 0 and size (exclusive max)
        int randomIdx = Sombrero::RandomFast::randomNumber() * (float)(ml.size() - 1);
        x->SelectLevel(ml[randomIdx]);
    }

    void FilterUI::_SetSort(StringW selected) {
        SetSort(selected);
    }

    void FilterUI::_SetFilter(StringW selected) {
        SetFilter(selected);
    }

    void FilterUI::PossiblyShowNextWarning() {
        ShowErrorASAP();
    }

    void FilterUI::SettingsOpened() {
		config.settingsSeenInVersion = VERSION;
		settingsWereOpened = true;

        // TODO: parsing settings n stuff
		//BSMLStuff.InitSplitView(ref settingsViewParams, rootTransform.gameObject, SplitViews.Settings.instance).EmitEvent("ShowSettings");
    }

    void FilterUI::PostParse() {
		UpdateVisibleTransformers();

		for(const auto& [key, value] : sortOptions) {
			if(value == HookLevelCollectionTableSet::sorter) {
				SetSort(key, false, false);
				break;
			}
		}
		for(const auto& [key, value] : filterOptions) {
			if(value == HookLevelCollectionTableSet::filter) {
				SetFilter(key, false, false);
				break;
			}
		}

		UpdateDropdowns();
		SetSortDirection(config.sortAsc, false);

		GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(custom_types::Helpers::CoroutineHelper::New(PossiblyDrawUserAttentionToSettingsButton()));
    }

    static bool CheckIsVisible(ITransformerPlugin* plugin) {
        if (!plugin) return false;
        DEBUG("CheckIsVisible");
        plugin->ContextSwitch(HookSelectedCategory::get_lastSelectedCategory(), HookSelectedCollection::get_lastSelectedCollection());
		return plugin->get_visible();
    }

    void FilterUI::UpdateVisibleTransformers() {
        DEBUG("FilterUI::UpdateVisibleTransformers");
        sortOptions.clear();
        auto& sortMethods = SortMethods::get_methods();
        for (const auto& [key, value] : sortMethods) {
            ITransformerPlugin* plugin = value ? value->as<ITransformerPlugin*>() : nullptr;
            
            if (!plugin || CheckIsVisible(plugin)) {
                sortOptions[key] = value;
            }
        }

        DEBUG("Got {} filters", sortOptions.size());
        auto instance = get_instance();
        auto& sortOptionsList = instance->sortOptionsList;
        sortOptionsList->Clear();
        for (const auto& [key, value] : sortOptions) sortOptionsList->Add(StringW(key));
        
        filterOptions.clear();
        auto& filterMethods = FilterMethods::get_methods();
        for (const auto& [key, value] : filterMethods) {
            ITransformerPlugin* plugin = value ? value->as<ITransformerPlugin*>() : nullptr;
            if (!plugin || CheckIsVisible(plugin)) {
                filterOptions[key] = value;
            }
        }
        
        DEBUG("Got {} filters", filterOptions.size());
        auto& filterOptionsList = instance->filterOptionsList;
        for (const auto& [key, value] : filterOptions) filterOptionsList->Add(StringW(key));
    }

    FilterUI* FilterUI::get_instance() {
        if (!instance) {
            instance = FilterUI::New_ctor();
        }
        return instance.ptr();
    }

    void FilterUI::SetSort(std::string selected, bool storeToConfig, bool refresh) {
        if (sortOptions.empty()) return;
        DEBUG("FilterUI::SetSort({}, {}, {})", selected, storeToConfig, refresh);

        auto sortItr = sortOptions.find(selected);
        if (selected.empty() || sortItr == sortOptions.end()) {
            sortItr = sortOptions.find("Default");
            selected = sortItr->first;
        }

        auto newSort = sortItr->second;
        auto unavCheck = newSort ? newSort->as<IAvailabilityCheck*>() : nullptr;
        std::string reason = unavCheck ? unavCheck->GetUnavailableReason() : "";

        auto instance = get_instance();
        if (unavCheck && !reason.empty()) {
            instance->ShowErrorASAP(fmt::format("Can't sort by {} - ", selected, reason));    
			SetSort("", false, false);
            return;
        }

        DEBUG("Setting sort to {}", selected);

        if (HookLevelCollectionTableSet::sorter != newSort) {
            if (storeToConfig) {
                config.lastSort = selected;
            }

            HookLevelCollectionTableSet::sorter = newSort;
            RestoreTableScroll::ResetScroll();

            if (refresh) HookLevelCollectionTableSet::Refresh(true);
        }

        auto dropDown = instance->sortDropDown;
        if (dropDown && dropDown->m_CachedPtr.m_value) {
            dropDown->SelectCellWithIdx(instance->sortOptionsList->ToArray()->IndexOf(StringW(selected)));
        }
    }

    void FilterUI::ClearFilter(bool reloadTable) {
        SetFilter("", false, reloadTable);
    }

    void FilterUI::SetFilter(std::string selected, bool storeToConfig, bool refresh) {
        if (filterOptions.size() == 0) return;

        DEBUG("FilterUI::SetFilter({}, {}, {})", selected, storeToConfig, refresh);
        auto filterItr = filterOptions.find(selected);
        if (selected.empty() || filterItr == filterOptions.end()) {
            filterItr = filterOptions.find("All");
            selected = filterItr->first;
        }

        auto newFilter = filterItr->second;
        auto unavCheck = newFilter ? newFilter->as<IAvailabilityCheck*>() : nullptr;
        std::string reason = unavCheck ? unavCheck->GetUnavailableReason() : "";

        auto instance = get_instance();
        if (unavCheck && !reason.empty()) {
            instance->ShowErrorASAP(fmt::format("Can't filter by {} - ", selected, reason));    
			SetFilter("", false, false);
            return;
        }

        if (HookLevelCollectionTableSet::filter != newFilter) {
            if (storeToConfig) {
                config.lastFilter = selected;
            }
            HookLevelCollectionTableSet::filter = newFilter;
            RestoreTableScroll::ResetScroll();
            if (refresh) HookLevelCollectionTableSet::Refresh(true);
        }

        auto dropDown = instance->filterDropDown;
        if (dropDown && dropDown->m_CachedPtr.m_value) {
            dropDown->SelectCellWithIdx(instance->filterOptionsList->ToArray()->IndexOf(StringW(selected)));
        }
    }

    void FilterUI::SetSortDirection(bool ascending, bool refresh) {
        if (!HookLevelCollectionTableSet::sorter) return;

        if (config.sortAsc != ascending) {
            config.sortAsc = ascending;
            RestoreTableScroll::ResetScroll();
            if (refresh)
                HookLevelCollectionTableSet::Refresh();
        }

		auto sortDirection = get_instance()->sortDirection;
        if (sortDirection && sortDirection->m_CachedPtr.m_value) {
            sortDirection->SetText(ascending ? "▲" : "▼");
        }
    }

    void FilterUI::Init() {
        DEBUG("FilterUI::Init");
		UpdateVisibleTransformers();
		SetSort(config.lastSort, false, false);
		SetFilter(config.lastFilter, false, false);
		SetSortDirection(config.sortAsc);
    }

    void FilterUI::AttachTo(UnityEngine::Transform* target) {
        auto instance = get_instance();
        instance->parser = BSML::parse_and_construct(IncludedAssets::MainUI_bsml, target, instance);
        auto root = instance->root;
		root->set_localScale(root->get_localScale() * 0.7f);

        auto targetRectT = reinterpret_cast<RectTransform*>(target);
        auto sd = targetRectT->get_sizeDelta();
        sd.y += 2;
		targetRectT->set_sizeDelta(sd);
        auto child = target->GetChild(0);
        auto pos = child->get_position();
        pos.y -= 0.02f;
        child->set_position(pos);
    }

    void FilterUI::HackDropdown(HMUI::DropdownWithTableView* dropdown) {
        int c = std::min(9, dropdown->get_tableViewDataSource()->NumberOfCells());
        dropdown->numberOfVisibleCells = c;
        dropdown->ReloadData();
    }

    void FilterUI::ShowErrorASAP(std::string_view text) {
        if (!text.empty()) warnings.push(static_cast<std::string>(text));
        if (!warningLoadInProgress)
            GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(custom_types::Helpers::CoroutineHelper::New(_ShowError()));
    }

    custom_types::Helpers::Coroutine FilterUI::_ShowError() {
		warningLoadInProgress = true;
        // wait till we can display
        while (!failTextLabel || !failTextLabel->m_CachedPtr.m_value) co_yield nullptr;

        auto x = failTextLabel->GetComponentInParent<HMUI::ViewController*>();
        if (x && x->m_CachedPtr.m_value) {
            while (x->get_isInTransition()) co_yield nullptr;

            if (x->get_isActivated() && warnings.size() > 0) {
                failTextLabel->set_text(warnings.front());
                warnings.pop();

                if (incompatibilityModal && incompatibilityModal->m_CachedPtr.m_value) {
                    incompatibilityModal->Show();
                }
            }
        }

		warningLoadInProgress = false;
        co_return;
    }

    custom_types::Helpers::Coroutine FilterUI::PossiblyDrawUserAttentionToSettingsButton() {
        bool valid = semver::valid(config.settingsSeenInVersion);
        if (valid && semver::gte(config.settingsSeenInVersion, VERSION)) co_return;

        while (!settingsWereOpened) {
            co_yield reinterpret_cast<System::Collections::IEnumerator*>(UnityEngine::WaitForSeconds::New_ctor(0.5f));
            if (settingsButton && settingsButton->m_CachedPtr.m_value)
                settingsButton->set_color({0, 1, 0, 1});


            co_yield reinterpret_cast<System::Collections::IEnumerator*>(UnityEngine::WaitForSeconds::New_ctor(0.5f));
            if (settingsButton && settingsButton->m_CachedPtr.m_value)
                settingsButton->set_color({1, 1, 1, 1});
        }
        co_return;
    }

    void FilterUI::CloseWarningModal() {
        if (incompatibilityModal && incompatibilityModal->m_CachedPtr.m_value) {
            incompatibilityModal->Hide();
        }
        PossiblyShowNextWarning();
    }

}