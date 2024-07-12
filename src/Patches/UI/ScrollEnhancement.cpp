#include "Patches/UI/ScrollEnhancement.hpp"
#include "config.hpp"
#include "logging.hpp"

#include "bsml/shared/Helpers/utilities.hpp"
#include "bsml/shared/Helpers/delegates.hpp"

#include "UnityEngine/WaitForEndOfFrame.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "bsml/shared/BSML/SharedCoroutineStarter.hpp"

#include "System/Tuple_2.hpp"
#include "HMUI/ImageView.hpp"
#include "HMUI/TableView.hpp"
#include "HMUI/NoTransitionsButton.hpp"

void Scroll(HMUI::TableView* table, float step, int direction) {
    auto cells = table->get_dataSource()->NumberOfCells();
    if (cells == 0) return;
    float amt = (float)cells * step * (float)direction;

    if (step != 1) {
        amt += table->GetVisibleCellsIdRange()->get_Item1();
    }

    table->ScrollToCellWithIdx(amt, HMUI::TableView::ScrollPositionType::Beginning, true);
}

namespace BetterSongList::Hooks {
    std::array<SafePtrUnity<UnityEngine::GameObject>, 4> ScrollEnhancement::buttons;

    void ScrollEnhancement::GameRestart() {
        for (auto& btn : buttons) {
            if (btn && btn.ptr() && btn->___m_CachedPtr.m_value) {
                UnityEngine::Object::DestroyImmediate(btn.ptr());
            }
            btn = nullptr;
        }
    }

    void ScrollEnhancement::LevelCollectionTableView_Init_Prefix(GlobalNamespace::LevelCollectionTableView* self, bool isInitialized, HMUI::TableView* tableView) {
        INFO("ScrollEnhancement::LevelCollectionTableView_Init_Prefix({}, {}, {})", fmt::ptr(self), isInitialized, fmt::ptr(tableView));
        if (!isInitialized)
            BSML::SharedCoroutineStarter::get_instance()->StartCoroutine(custom_types::Helpers::CoroutineHelper::New(SetupExtraScrollButtons(tableView, self->get_transform())));

        UpdateState();
    }

    void ScrollEnhancement::UpdateState() {
        for (auto& btn : buttons) {
            if (btn && btn.ptr() && btn->___m_CachedPtr.m_value) {
                btn->SetActive(config.get_extendSongScrollbar());
            }
        }
    }

    UnityEngine::Transform* ScrollEnhancement::BuildButton(UnityEngine::Transform* baseButton, StringW Icon, float vOffs, float rotation, std::function<void()> cb) {
        auto newBtnGo = UnityEngine::Object::Instantiate(baseButton->get_gameObject(), 
                baseButton
                ->get_parent() // ScrollBar
                ->get_parent() // LevelsTableView
                ->get_parent() // LevelCollectionViewController
            );

        auto newBtn = newBtnGo->get_transform().try_cast<UnityEngine::RectTransform>().value_or(nullptr);
        newBtn->set_anchorMin({0.92f, 0.893f - vOffs});
        newBtn->set_anchorMax({0.96f, 0.953f - vOffs});

        auto i = newBtn->GetComponentInChildren<HMUI::ImageView*>(true);
        BSML::Utilities::SetImage(i, Icon);

        auto iT = i->get_rectTransform();
        iT->set_offsetMax({0, 0});
        iT->set_offsetMin({-2.5f, -2.5f});
        iT->set_localEulerAngles({0, 0, rotation});

        auto btn = newBtnGo->GetComponent<HMUI::NoTransitionsButton*>();
        btn->set_interactable(true);
        auto btnClickedEvent = UnityEngine::UI::Button::ButtonClickedEvent::New_ctor();
        btnClickedEvent->AddListener(BSML::MakeUnityAction(cb));
        btn->set_onClick(btnClickedEvent);

        return newBtn;
    }

    static HMUI::TableView* storedTable = nullptr;

    custom_types::Helpers::Coroutine ScrollEnhancement::SetupExtraScrollButtons(HMUI::TableView* table, UnityEngine::Transform* a) {
        storedTable = table;
        co_yield reinterpret_cast<System::Collections::IEnumerator*>(UnityEngine::WaitForEndOfFrame::New_ctor());

        auto r = table->get_transform()->get_parent()->get_parent().try_cast<UnityEngine::RectTransform>().value_or(nullptr);
        auto sizeDelta = r->get_sizeDelta();
        sizeDelta.x += 4;
        r->set_sizeDelta(sizeDelta);

        r = table->get_transform()->get_parent().try_cast<UnityEngine::RectTransform>().value_or(nullptr);
        auto anchorMin = r->get_anchorMin();
        anchorMin.x += 0.02f;
        r->set_anchorMin(anchorMin);
        sizeDelta = r->get_sizeDelta();
        sizeDelta.x -= 2.0f;
        r->set_sizeDelta(sizeDelta);
        static ConstString btnPath{"ScrollBar/UpButton"};
        auto button = a->Find(btnPath);

        // move the scroll bar to the right slightly
        static ConstString scrollBar{"ScrollBar"};
        auto bar = a->Find(scrollBar).try_cast<UnityEngine::RectTransform>().value_or(nullptr);
        auto min = bar->get_anchorMin();
        min.x -= 0.04f;
        bar->set_anchorMin(min);
        auto max = bar->get_anchorMax();
        max.x -= 0.04f;
        bar->set_anchorMax(max);

        

        auto btnUpFast = BuildButton(button, MOD_ID "_double_arrow", 0, -90, [](){Scroll(storedTable, 0.1f, -1);});
        auto btnDownFast = BuildButton(button, MOD_ID "_double_arrow", 0.86f, 90, [](){Scroll(storedTable, 0.1f, 1);});

        buttons[0] = btnUpFast->get_gameObject();
        buttons[1] = BuildButton(button, "#HeightIcon", 0.09f, 0, [](){Scroll(storedTable, 1.0f, 0);})->get_gameObject();
        buttons[2] = BuildButton(button, "#HeightIcon", 0.77f, 180, [](){Scroll(storedTable, 1.0f, 1);})->get_gameObject();
        buttons[3] = btnDownFast->get_gameObject();
        co_yield nullptr;
        UpdateState();
    }
}
