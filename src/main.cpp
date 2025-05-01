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
        "covernts.custom_wraith_codes"
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
        auto restartPopup = geode::createQuickPopup(
            "Restart Required",
            "We've disabled one or more blacklisted mods.\nPlease restart to apply changes.\n<cr>(AUTO RESTART IN 5 SECONDS)</c>",
            "Restart Now", nullptr,
            [](bool restart_now, auto) {
                if (restart_now) {
                    game::restart();
                }
            },
            false);
        
        restartPopup->show();

        this->runAction(
            CCSequence::create(
                CCDelayTime::create(5.0f),
                CCCallFunc::create(this, callfunc_selector(MyMenuLayer::gameRestart)),
                nullptr
            )
        );
    }

    void gameRestart() {
        game::restart();
    }
};