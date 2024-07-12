#include "hooking.hpp"
#include "logging.hpp"

#include "custom-types/shared/coroutine.hpp"
#include "HMUI/ContainerViewController.hpp"
#include "HMUI/ViewController.hpp"
#include "UnityEngine/EventSystems/EventSystem.hpp"

#include "UnityEngine/Time.hpp"
#include "System/Array.hpp"
#include "System/Action.hpp"
#include "System/Action_3.hpp"
#include "System/Action_2.hpp"
#include "System/Collections/Generic/HashSet_1.hpp"

#include "sombrero/shared/linq_functional.hpp"
using namespace Sombrero::Linq::Functional;

// just this because it's so fukin long
template<typename T>
using HashSet = System::Collections::Generic::HashSet_1<T>;
using AnimationLayouterAction_3 = System::Action_3<float_t, ArrayW<UnityW<HMUI::ViewController>>, System::Collections::Generic::HashSet_1<UnityW<HMUI::ViewController>>*>*;
custom_types::Helpers::Coroutine RemoveViewControllersCoroutine(HMUI::ContainerViewController* self, ::ArrayW<::HMUI::ViewController*> viewControllersToRemove, ::System::Action* finishedCallback, AnimationLayouterAction_3 animationLayouter, bool immediately);

// on PC this is a transpiler on the coroutine enumerator, but we can't do that, so we just make our own coro with the transpiler applied
// from UnscufContainerViewControllerRemove
MAKE_AUTO_HOOK_ORIG_MATCH(ContainerViewController_RemoveViewControllers, &HMUI::ContainerViewController::RemoveViewControllers, void, 
    HMUI::ContainerViewController* self, 
    ArrayW<HMUI::ViewController*> viewControllers, 
    System::Action* finishedCallback,
    AnimationLayouterAction_3 animationLayouter,
    bool immediately) {
    //self->StartCoroutine(custom_types::Helpers::CoroutineHelper::New(RemoveViewControllersCoroutine(self, viewControllers, finishedCallback, animationLayouter, immediately)));
    ContainerViewController_RemoveViewControllers(self, viewControllers, finishedCallback, animationLayouter, immediately);
    // not calling ORIG on purpose!
}

custom_types::Helpers::Coroutine RemoveViewControllersCoroutine(HMUI::ContainerViewController* self, ArrayW<HMUI::ViewController*> viewControllers, System::Action* finishedCallback, AnimationLayouterAction_3 animationLayouter, bool immediately) {
    if (self->get_isInTransition()) co_return;

    self->set_isInTransition(true);
    for (auto vc : viewControllers) vc->set_isInTransition(true);

    auto wrappedViewControllrs = ArrayW<UnityW<HMUI::ViewController>>(viewControllers.size());
    for (int i = 0; i < viewControllers.size(); i++) {
        wrappedViewControllrs[i] = viewControllers[i];
    }

    auto eventSystem = UnityEngine::EventSystems::EventSystem::get_current();
    if (eventSystem && eventSystem->___m_CachedPtr.m_value) eventSystem->SetSelectedGameObject(nullptr);

    // idk if casting an array like that is allowed but it should work like on PC?
    auto viewControllersToRemoveSet = HashSet<UnityW<HMUI::ViewController>>::New_ctor(reinterpret_cast<System::Collections::Generic::IEnumerable_1<UnityW<HMUI::ViewController>>*>(wrappedViewControllrs.convert()));
    if (!immediately) {
        const float transitionDuration = 0.4f;
        float elapsedTime = 0.0f;
        while(elapsedTime  < transitionDuration) {
            float arg = elapsedTime / transitionDuration;
            animationLayouter->Invoke(arg, viewControllersToRemoveSet, viewControllersToRemoveSet);
            elapsedTime += UnityEngine::Time::get_deltaTime();
            co_yield nullptr;
        }
    }
    animationLayouter->Invoke(1.0f, viewControllersToRemoveSet, viewControllersToRemoveSet);

    for (auto vc : viewControllers) {
        vc->__Deactivate(true, true, false);
        vc->__ResetViewController();
    }
    
    // this._viewControllers = this._viewControllers.Except(viewControllersToRemove).ToList<ViewController>();
    // this VV does the same thing except no need to make a new list
    for (auto vc : viewControllers) {
        self->____viewControllers->Remove(vc);
    }
    
    self->set_isInTransition(false);
    for (auto vc : viewControllers) vc->set_isInTransition(false);

    if (finishedCallback) finishedCallback->Invoke();

    co_return;
}