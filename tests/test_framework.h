#ifndef TESTS_TEST_FRAMEWORK_H
#define TESTS_TEST_FRAMEWORK_H

#include <iostream>
#include <vector>

// 零依赖测试微框架:每个测试文件用 TEST_GROUP 注册用例,
// test_main.cc 负责遍历执行;CHECK 失败会计入 g_failures(test_main.cc 定义)。

struct TestCase {
    const char* name;
    void (*fn)();
};

inline std::vector<TestCase>& TestRegistry() {
    static std::vector<TestCase> registry;
    return registry;
}

struct TestRegistrar {
    TestRegistrar(const char* name, void (*fn)()) {
        TestRegistry().push_back({name, fn});
    }
};

// 全局失败计数,由 test_main.cc 定义
extern int g_failures;

#define TEST_GROUP(name) \
    static void name##_body(); \
    static TestRegistrar name##_registrar(#name, name##_body); \
    static void name##_body()

#define CHECK(expr)                                                  \
    do {                                                             \
        if (!(expr)) {                                               \
            std::cout << "  [FAIL] " << __FILE__ << ":" << __LINE__  \
                      << ": " << #expr << "\n";                      \
            ++g_failures;                                            \
        }                                                            \
    } while (0)

#define CHECK_EQ(actual, expected)                                   \
    do {                                                             \
        if (!((actual) == (expected))) {                             \
            std::cout << "  [FAIL] " << __FILE__ << ":" << __LINE__  \
                      << ": " << #actual << " == " << #expected      \
                      << "\n";                                       \
            ++g_failures;                                            \
        }                                                            \
    } while (0)

#endif
