# UnloadedEntitiesCleaner - 清理未加载区块生物

> 温馨提示：此插件支持热加载/重载。

## 配置文件
```json
{
    "version": 1,// 配置文件版本
    "clearList": [ // 清除实体的列表(可填命名空间ID)
        "minecraft:zombie_pigman", // 僵尸猪人
        "minecraft:item", // 掉落物
        "minecraft:xp_orb", // 经验
        "minecraft:falling_block", // 下落的方块(如沙子，龙蛋等)
        "minecraft:silverfish", // 蠹虫
        "minecraft:tnt", // TNT
        "minecraft:tnt_minecart", // TNT矿车
        "minecraft:area_effect_cloud" // 区域效果云(滞留药水丢出来生成的)
    ],
    "whiteList": [ // 白名单列表(可填标签名或命名)
        "noClear"
    ],
    "clearNamedEntity": false // 是否允许清除命名实体(填false则不清除有命名的实体)
}
```

## 安装方法

- 手动安装
  - 前往[Releases](https://github.com/zimuya4153/UnloadedEntitiesCleaner/releases)下载最新版本的`UnloadedEntitiesCleaner-windows-x64.zip`
  - 解压`压缩包内的`文件夹到`./plugins/`目录
- Lip 安装
  - 输入命令`lip install -y github.com/zimuya4153/UnloadedEntitiesCleaner`
- ~~一条龙安装~~
  - ~~去 Q 群，喊人，帮你安装~~