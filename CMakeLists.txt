cmake_minimum_required(VERSION 4.3)
project(LQZ)

set(CMAKE_CXX_STANDARD 26)

# 导出 compile_commands.json，方便 CLion/clangd 解析项目
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 编译警告选项：与 makefile 的 -Wall 保持一致
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall)
endif()

# 主程序目标：源文件列表与 makefile 的 SRC 保持一致
# gui.cc/gui.h 依赖 GTKmm，为 1.1 版本 GUI 功能预留，暂不参与编译
add_executable(phone_forensic
        src/main.cc
        src/controller/controller.cc
        src/controller/controller.h
        src/model/model.cc
        src/model/model.h
        src/view/view.cc
        src/view/view.h)

target_include_directories(phone_forensic PRIVATE ${CMAKE_SOURCE_DIR}/src)

# 可选 GUI 目标（1.1 版本功能）
# 仅当开启 LQZ_BUILD_GUI 且本机安装了 GTKmm 时才会构建
option(LQZ_BUILD_GUI "构建 GTKmm 图形界面（1.1 版本功能）" OFF)

if(LQZ_BUILD_GUI)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTKMM IMPORTED_TARGET gtkmm-4.0)
    if(GTKMM_FOUND)
        add_library(lqz_gui STATIC
                src/view/gui.cc
                src/view/gui.h)
        target_include_directories(lqz_gui PUBLIC ${CMAKE_SOURCE_DIR}/src)
        target_link_libraries(lqz_gui PUBLIC PkgConfig::GTKMM)
        message(STATUS "GTKmm 已找到，GUI 目标 lqz_gui 已启用")
    else()
        message(WARNING "未找到 GTKmm，跳过 GUI 目标（brew install gtkmm4 后可启用）")
    endif()
endif()
