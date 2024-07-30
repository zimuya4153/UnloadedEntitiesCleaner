#include <string>
#include <vector>


namespace UnloadedEntitiesCleaner {
struct Config {
    int                      version          = 1;     // 配置文件版本
    std::vector<std::string> clearList        = {};    // 清除列表，填命名空间ID
    std::vector<std::string> whiteList        = {};    // 白名单列表，填标签或命名
    bool                     clearNamedEntity = false; // 是否允许清除命名实体
};
} // namespace UnloadedEntitiesCleaner