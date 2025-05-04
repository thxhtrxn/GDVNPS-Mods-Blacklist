#include <Geode/Geode.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "BlacklistedMods.hpp"

using namespace geode::prelude;

// đề phòng trường hợp một số mod không disable được
class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        bool foundBlacklisted = false;
        log::info("Hello from CheckBlacklistAllMods.cpp");

        for (auto const& mod : Loader::get()->getAllMods()) {
            if (blacklistedMods.count(mod->getID()) && mod->isEnabled()) {
                foundBlacklisted = true;
                mod->disable();
                log::warn("Disabled blacklisted mod: {}", mod->getID());
            }
        }

        if (foundBlacklisted) {
            game::restart();
        }

        return true;
    }
};
