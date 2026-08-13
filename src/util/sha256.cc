#include "util/sha256.h"

#include <cstring>
#include <fstream>

namespace util {
namespace {

// FIPS 180-4 轮常量表
constexpr std::uint32_t kRoundConstants[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
    0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
    0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
    0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
    0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
    0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
};

constexpr std::uint32_t RotateRight(std::uint32_t x, int n) {
    return (x >> n) | (x << (32 - n));
}

std::string DigestToHex(const std::array<unsigned char, 32>& digest) {
    static const char* kHexDigits = "0123456789abcdef";
    std::string out;
    out.reserve(64);
    for (unsigned char byte : digest) {
        out.push_back(kHexDigits[byte >> 4]);
        out.push_back(kHexDigits[byte & 0x0f]);
    }
    return out;
}

}  // namespace

Sha256::Sha256() {
    state_[0] = 0x6a09e667;
    state_[1] = 0xbb67ae85;
    state_[2] = 0x3c6ef372;
    state_[3] = 0xa54ff53a;
    state_[4] = 0x510e527f;
    state_[5] = 0x9b05688c;
    state_[6] = 0x1f83d9ab;
    state_[7] = 0x5be0cd19;
    bitLength_ = 0;
    bufferLength_ = 0;
}

void Sha256::Update(const void* data, std::size_t len) {
    const auto* bytes = static_cast<const unsigned char*>(data);
    bitLength_ += static_cast<std::uint64_t>(len) * 8;

    // 先把上次残留的半块补满
    if (bufferLength_ > 0) {
        while (len > 0 && bufferLength_ < 64) {
            buffer_[bufferLength_++] = *bytes++;
            --len;
        }
        if (bufferLength_ == 64) {
            Transform(buffer_);
            bufferLength_ = 0;
        }
    }
    // 整块直通
    while (len >= 64) {
        Transform(bytes);
        bytes += 64;
        len -= 64;
    }
    // 剩余部分留在缓冲里等收尾
    if (len > 0) {
        std::memcpy(buffer_, bytes, len);
        bufferLength_ = len;
    }
}

void Sha256::Update(std::string_view data) {
    Update(data.data(), data.size());
}

std::array<unsigned char, 32> Sha256::Final() {
    // 填充:0x80 + 若干 0 + 64 位大端比特长度,
    // 使填充后的总长(含 8 字节长度)恰好是 64 的倍数
    std::array<unsigned char, 136> padding{};
    padding[0] = 0x80;
    const std::size_t padTotal =
        (bufferLength_ <= 55) ? (56 - bufferLength_) : (120 - bufferLength_);
    for (int i = 0; i < 8; ++i) {
        padding[padTotal + i] =
            static_cast<unsigned char>((bitLength_ >> (56 - 8 * i)) & 0xff);
    }
    Update(padding.data(), padTotal + 8);

    std::array<unsigned char, 32> digest{};
    for (int i = 0; i < 8; ++i) {
        digest[4 * i] = static_cast<unsigned char>((state_[i] >> 24) & 0xff);
        digest[4 * i + 1] = static_cast<unsigned char>((state_[i] >> 16) & 0xff);
        digest[4 * i + 2] = static_cast<unsigned char>((state_[i] >> 8) & 0xff);
        digest[4 * i + 3] = static_cast<unsigned char>(state_[i] & 0xff);
    }
    *this = Sha256();  // 复位,支持复用
    return digest;
}

std::array<unsigned char, 32> Sha256::OneShot(std::string_view data) {
    Sha256 hasher;
    hasher.Update(data);
    return hasher.Final();
}

void Sha256::Transform(const unsigned char* block) {
    // 消息扩展
    std::uint32_t w[64];
    for (int i = 0; i < 16; ++i) {
        w[i] = (static_cast<std::uint32_t>(block[4 * i]) << 24) |
               (static_cast<std::uint32_t>(block[4 * i + 1]) << 16) |
               (static_cast<std::uint32_t>(block[4 * i + 2]) << 8) |
               static_cast<std::uint32_t>(block[4 * i + 3]);
    }
    for (int i = 16; i < 64; ++i) {
        const std::uint32_t s0 = RotateRight(w[i - 15], 7) ^
                                 RotateRight(w[i - 15], 18) ^ (w[i - 15] >> 3);
        const std::uint32_t s1 = RotateRight(w[i - 2], 17) ^
                                 RotateRight(w[i - 2], 19) ^ (w[i - 2] >> 10);
        w[i] = w[i - 16] + s0 + w[i - 7] + s1;
    }

    // 64 轮压缩
    std::uint32_t a = state_[0], b = state_[1], c = state_[2], d = state_[3];
    std::uint32_t e = state_[4], f = state_[5], g = state_[6], h = state_[7];
    for (int i = 0; i < 64; ++i) {
        const std::uint32_t s1 =
            RotateRight(e, 6) ^ RotateRight(e, 11) ^ RotateRight(e, 25);
        const std::uint32_t ch = (e & f) ^ (~e & g);
        const std::uint32_t temp1 = h + s1 + ch + kRoundConstants[i] + w[i];
        const std::uint32_t s0 =
            RotateRight(a, 2) ^ RotateRight(a, 13) ^ RotateRight(a, 22);
        const std::uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
        const std::uint32_t temp2 = s0 + maj;
        h = g;
        g = f;
        f = e;
        e = d + temp1;
        d = c;
        c = b;
        b = a;
        a = temp1 + temp2;
    }
    state_[0] += a;
    state_[1] += b;
    state_[2] += c;
    state_[3] += d;
    state_[4] += e;
    state_[5] += f;
    state_[6] += g;
    state_[7] += h;
}

std::string Sha256Hex(std::string_view data) {
    return DigestToHex(Sha256::OneShot(data));
}

std::string Sha256FileHex(const std::string& path, std::string& err) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        err = "无法打开文件: " + path;
        return {};
    }
    Sha256 hasher;
    char buffer[64 * 1024];
    while (file) {
        file.read(buffer, sizeof(buffer));
        const std::streamsize got = file.gcount();
        if (got > 0) hasher.Update(buffer, static_cast<std::size_t>(got));
    }
    if (file.bad()) {
        err = "读取文件失败: " + path;
        return {};
    }
    return DigestToHex(hasher.Final());
}

}  // namespace util
