#ifndef UTIL_SHA256_H
#define UTIL_SHA256_H

#include <array>
#include <cstddef>
#include <string>
#include <string_view>

// SHA-256 摘要工具(零依赖)。
// 算法依据 FIPS 180-4 实现,参考 Brad Conte 的公开领域实现:
//   https://github.com/B-Con/crypto-algorithms
// 该算法本体为公开领域代码,可随本项目以 Mulan PSL v2 再分发。
namespace util {

class Sha256 {
   public:
    Sha256();

    // 流式喂入数据,支持分块(大文件逐块调用)
    void Update(const void* data, std::size_t len);
    void Update(std::string_view data);

    // 收尾并输出 32 字节摘要;调用后对象自动复位,可复用
    std::array<unsigned char, 32> Final();

    // 一次性摘要(小数据便捷接口)
    static std::array<unsigned char, 32> OneShot(std::string_view data);

   private:
    void Transform(const unsigned char* block);

    std::uint32_t state_[8];
    std::uint64_t bitLength_;
    unsigned char buffer_[64];
    std::size_t bufferLength_;
};

// 返回小写 64 位十六进制摘要
std::string Sha256Hex(std::string_view data);

// 分块流式计算文件摘要;失败时返回空串并在 err 中写明原因
std::string Sha256FileHex(const std::string& path, std::string& err);

}  // namespace util

#endif
