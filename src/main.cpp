#include "BlacklistedMods.hpp"
#include <Geode/Geode.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <unordered_set>

using namespace geode::prelude;

constexpr const char* KEY_FORCE_CHECK = "forceCheckMods";

$on_mod(Loaded) {
    log::info("Running blacklist check...");
    bool hasDisabled = false;

    for (auto const& mod : Loader::get()->getAllMods()) {
        if (blacklistedMods.count(mod->getID()) && mod->isEnabled()) {
            mod->disable();
            log::info("Disabling {}", mod->getID());
            hasDisabled = true;
        }
    }

    if (hasDisabled) {
        Mod::get()->setSavedValue(KEY_FORCE_CHECK, true);
    }
}

class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        if (Mod::get()->getSavedValue<bool>(KEY_FORCE_CHECK, false)) {
            Mod::get()->setSavedValue(KEY_FORCE_CHECK, false);
            this->runAction(
                CCSequence::createWithTwoActions(
                    CCDelayTime::create(0.3f),
                    CCCallFunc::create(this, callfunc_selector(MyMenuLayer::forceDisableBlacklistMods))
                )
            );
        }
        return true;
    }

    void forceDisableBlacklistMods() {
        log::info("Running post-load blacklist check...");
        bool foundEnabled = false;

        for (auto const& mod : Loader::get()->getAllMods()) {
            if (blacklistedMods.count(mod->getID()) && mod->isEnabled()) {
                mod->disable();
                log::warn("Mod {} was re-enabled - force-disabling again.", mod->getID());
                foundEnabled = true;
            }
        }

        if (foundEnabled) {
            this->runAction(
                CCSequence::createWithTwoActions(
                    CCDelayTime::create(0.05f),
                    CCCallFunc::create(this, callfunc_selector(MyMenuLayer::gameRestart))
                )
            );
        }
    }

    void gameRestart() {
        log::info("Restarting game to finalize blacklist enforcement.");
        game::restart();
    }
};
