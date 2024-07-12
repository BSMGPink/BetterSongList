#include "CustomTypes/CanvasGroupLinker.hpp"

DEFINE_TYPE(BetterSongList, CanvasGroupLinker);

namespace BetterSongList {

    void CanvasGroupLinker::Update() {
        auto currentAlpha = baseCanvasGroup->get_alpha();
        auto diff = std::abs(currentAlpha - lastAlpha);
        if(diff > 0.01f && !triggered && currentAlpha > 0.1f) {
            triggered = true;
            if(OnCanvasGroupChanged) {
                OnCanvasGroupChanged();
            }
        }
        if(diff < 0.005f) {
            triggered = false;
        }
        targetCanvasGroup->set_alpha(currentAlpha);
        lastAlpha = currentAlpha;
    }
}