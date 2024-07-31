#include "UnloadedEntitiesCleaner.h"
#include "Config.h"

#include <algorithm>
#include <ll/api/Config.h>
#include <ll/api/Logger.h>
#include <ll/api/memory/Hook.h>
#include <ll/api/mod/RegisterHelper.h>
#include <ll/api/service/Bedrock.h>
#include <mc/nbt/CompoundTag.h>
#include <mc/world/ActorUniqueID.h>
#include <mc/world/level/Level.h>
#include <mc/world/level/storage/DBStorage.h>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>


ll::Logger                      logger("UnloadedEntitiesCleaner");
UnloadedEntitiesCleaner::Config config;

bool checkEntity(ll::Expected<CompoundTag> data) {
    try {
        if (!data->contains("UniqueID")) {
            return false;
        }
        if (!config.clearNamedEntity && data->contains("CustomName")) {
            return false;
        }
        if (data->contains("CustomName")
            && std::count(config.whiteList.begin(), config.whiteList.end(), data->getString("CustomName"))) {
            return false;
        }
        if (data->contains("Tags") && !data->getList("Tags")->empty()) {
            for (auto& tag : *data->getList("Tags")) {
                if (std::count(config.whiteList.begin(), config.whiteList.end(), tag->toString())) {
                    return false;
                }
            }
        }
        if (std::count(config.clearList.begin(), config.clearList.end(), data->getString("identifier"))) {
            return true;
        }
    } catch (std::string const& error) {
        logger.error("解析数据时发生错误: {}", error);
    } catch (std::exception const& error) {
        logger.error("解析数据时发生错误: {}", error.what());
    }
    return false;
}

namespace UnloadedEntitiesCleaner {

static std::unique_ptr<Entry> instance;

Entry& Entry::getInstance() { return *instance; }

bool Entry::load() {
    const auto& configFilePath = getSelf().getConfigDir() / "config.json";
    try {
        if (!ll::config::loadConfig(config, configFilePath)) {
            ll::config::saveConfig(config, configFilePath);
        }
    } catch (...) {
        ll::config::saveConfig(config, configFilePath);
    }
    return true;
}

bool Entry::enable() {
    if (config.clearList.size() == 0) {
        logger.warn("未填写清除配置，将不启用清除");
        return true;
    }
    std::thread([]() {
        auto& Storage = ll::service::getLevel()->getLevelStorage();
        logger.warn("开始清除");
        std::unordered_map<std::string, int> results = {};
        Storage.forEachKeyWithPrefix(
            "actorprefix",
            DBHelpers::Category::Actor,
            [&results, &Storage](std::string_view key_left, std::string_view data) {
                auto nbt = CompoundTag::fromBinaryNbt(data);
                if (checkEntity(nbt)) {
                    results[nbt->getString("identifier")] += 1;
                    Storage.deleteData("actorprefix" + std::string(key_left), DBHelpers::Category::Actor);
                }
            }
        );
        for (auto& result : results) {
            logger.warn("成功清除 {} 个 {}", std::to_string(result.second), result.first);
        }
        logger.warn("清除结束");
    }).detach();
    return true;
}

bool Entry::disable() { return true; }

bool Entry::unload() { return true; }

} // namespace UnloadedEntitiesCleaner

LL_REGISTER_MOD(UnloadedEntitiesCleaner::Entry, UnloadedEntitiesCleaner::instance);
