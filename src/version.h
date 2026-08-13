#ifndef VERSION_H
#define VERSION_H

// 版本号唯一来源:--version、manifest、custody 记录、TUI 横幅都从这里读取。
// 发布新版本时只需修改这里三行(CI 的 git tag 建议与 LQZ_VERSION_STRING 保持一致)。
#define LQZ_VERSION_MAJOR 1
#define LQZ_VERSION_MINOR 0
#define LQZ_VERSION_PATCH 0
#define LQZ_VERSION_STRING "1.0.0"

#endif
