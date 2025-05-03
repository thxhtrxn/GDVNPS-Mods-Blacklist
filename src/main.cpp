#include <Geode/Geode.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <unordered_set>

using namespace geode::prelude;

bool shouldForceCheckMods = false;

const std::unordered_set<std::string> blacklistedMods = {
    "beat.shipcopter",
    "raydeeux.frickglow",
    "ziegenhainy.fungal_shift",
    "covernts.custom_wraith_codes",
    "capeling.gamemode-switcher",
    "hiimjustin000.fake_rate",
    "bitz.moregames",
    "raydeeux.antilobotomy",
    "alphalaneous.asyncweb",
    "raydeeux_colon.towersecretending",
    "mariomastr.customisable-speed-portals",
    "camila314.pathfinder"
};

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
        shouldForceCheckMods = true;
    }
}

class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        if (shouldForceCheckMods) {
            this->runAction(
                CCSequence::createWithTwoActions(
                    CCDelayTime::create(0.5f),
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
                log::warn("Mod {} was re-enabled — force-disabling again.", mod->getID());
                foundEnabled = true;
            }
        }

        if (foundEnabled) {
            this->runAction(
                CCSequence::createWithTwoActions(
                    CCDelayTime::create(0.1f),
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
