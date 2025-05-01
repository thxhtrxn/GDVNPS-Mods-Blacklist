#include <Geode/Geode.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <unordered_set>

using namespace geode::prelude;

bool shouldShowPopup = false;

// you know what? fuck me
$on_mod(Loaded) {
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
                CCDelayTime::create(0.5f),
                CCCallFunc::create(this, callfunc_selector(MyMenuLayer::showBlacklistPopup))
            ));
        }
        return true;
    }

    void showBlacklistPopup() {
        geode::createQuickPopup(
            "Restart Required",
            "We've disabled one or more blacklisted mods.\nPlease restart to apply changes.",
            "No", "Yes",
            [](auto, bool yes) {
                if (yes) {
                    game::restart();
                }
            },
            false)->show();
    }
};