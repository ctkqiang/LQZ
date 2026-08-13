#include <iostream>

#include "test_framework.h"

// 全局失败计数:CHECK/CHECK_EQ 断言失败时累加
int g_failures = 0;

int main() {
    int totalFailures = 0;
    int totalTests = 0;
    for (const auto& test : TestRegistry()) {
        g_failures = 0;
        std::cout << "[TEST] " << test.name << "\n";
        test.fn();
        ++totalTests;
        if (g_failures > 0) {
            std::cout << "  " << g_failures << " 个断言失败\n";
            totalFailures += g_failures;
        }
    }
    if (totalFailures == 0) {
        std::cout << "全部通过(" << totalTests << " 组用例)\n";
        return 0;
    }
    std::cout << "失败:" << totalFailures << " 个断言\n";
    return 1;
}
