# SunnyLandDemo

## 版权声明

本项目中的代码和资源均遵循 `CC0` 许可证。

原作者在Bilibili上也有相关视频教程，欢迎关注和观看：

## 项目介绍

## 功能特点

## 使用方法

### 环境要求

- 操作系统：Windows
- 编译器：GCC / G++ / Clang
- 编译环境：VSCode/Visual Studio + CMake + vcpkg包管理器

### 编译方法

1. 克隆项目到本地
2. 确保你的vcpkg工具链已经安装并配置好以下库：
   - SDL3
   - SDL3_image
   - SDL3_ttf
   - glm
   - spdlog
   - nlohmann_json
3. 游戏在`CMakeLists.txt`中直接设置了vcpkg的路径为编写者的本地路径`（嘻嘻，其实是我懒）`，如果你使用路径和游戏的不一样，请自行修改`CMakeLists.txt`中的`set(VCPKG_ROOT_DIR "你的vcpkg路径")`。

## 联系方式

如有任何问题或建议，请不要联系我。`（因为我也不懂）`
