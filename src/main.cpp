#include <Geode/Geode.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <unordered_set>

using namespace geode::prelude;

bool shouldShowPopup = false;

// You know what? Fuck me.
$on_mod(Loaded) {
    // More will coming soon...
    std::unordered_set<std::string> modList = {
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

    for (auto const& mod : Loader::get()->getAllMods()) {
        if (modList.find(mod->getID()) != modList.end()) {
            if (mod->isEnabled()) {
                mod->disable();
                shouldShowPopup = true;
            }
        }
    }
}

class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        if(shouldShowPopup) {
            this->runAction(CCSequence::createWithTwoActions(
                CCDelayTime::create(0.05f),
                CCCallFunc::create(this, callfunc_selector(MyMenuLayer::showBlacklistPopup))
            ));
        }
        return true;
    }

    void showBlacklistPopup() {
        this->runAction(
            CCSequence::create(
                CCDelayTime::create(1.0f),
                CCCallFunc::create(this, callfunc_selector(MyMenuLayer::gameRestart)),
                nullptr
            )
        );
    }

    void gameRestart() {
        game::restart();
    }
};