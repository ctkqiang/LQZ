#include <algorithm>
#include <string>

#include "test_framework.h"
#include "util/sha256.h"

TEST_GROUP(sha256_nist_vectors) {
    // FIPS 180-4 / NIST 标准测试向量
    CHECK_EQ(util::Sha256Hex(""),
             "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
    CHECK_EQ(util::Sha256Hex("abc"),
             "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");
    CHECK_EQ(util::Sha256Hex("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"),
             "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1");
}

TEST_GROUP(sha256_long_message) {
    // NIST 长消息向量:一百万个小写 'a'
    const std::string longMessage(1'000'000, 'a');
    CHECK_EQ(util::Sha256Hex(longMessage),
             "cdc76e5c9914fb9281a1c7e284d73e67f1809a48a497200e046d39ccc7112cd0");
}

TEST_GROUP(sha256_streaming_equals_oneshot) {
    // 分块流式与一次性摘要应一致(模拟大文件逐块喂入)
    std::string data;
    for (int i = 0; i < 1000; ++i) {
        data += "lqz-forensic-取证数据-" + std::to_string(i);
    }
    util::Sha256 hasher;
    for (std::size_t offset = 0; offset < data.size(); offset += 7) {
        hasher.Update(data.data() + offset,
                      std::min<std::size_t>(7, data.size() - offset));
    }
    const auto digest = hasher.Final();
    static const char* kHex = "0123456789abcdef";
    std::string hex;
    hex.reserve(64);
    for (unsigned char byte : digest) {
        hex.push_back(kHex[byte >> 4]);
        hex.push_back(kHex[byte & 0x0f]);
    }
    CHECK_EQ(hex, util::Sha256Hex(data));
}
