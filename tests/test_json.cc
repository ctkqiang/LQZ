#include <cstdint>
#include <string>

#include "test_framework.h"
#include "util/json.h"

TEST_GROUP(json_escape_basics) {
    CHECK_EQ(util::JsonEscape(R"(he said "hi")"), R"(he said \"hi\")");
    CHECK_EQ(util::JsonEscape("a\\b"), "a\\\\b");
    CHECK_EQ(util::JsonEscape("line1\nline2\ttab\rret\b\f"),
             "line1\\nline2\\ttab\\rret\\b\\f");
}

TEST_GROUP(json_escape_control_chars) {
    std::string input;
    input.push_back('\x01');
    input.push_back('\x1f');
    CHECK_EQ(util::JsonEscape(input), "\\u0001\\u001F");
}

TEST_GROUP(json_escape_utf8_passthrough) {
    // UTF-8 多字节直通:中文路径字节原样保留
    const std::string chinese = "Datas/微信/mm.db";
    CHECK_EQ(util::JsonEscape(chinese), chinese);
}

TEST_GROUP(json_writer_compact) {
    util::JsonWriter w(false);
    w.BeginObject();
    w.Key("a");
    w.String("x");
    w.Key("b");
    w.Number(static_cast<std::int64_t>(-42));
    w.Key("c");
    w.Bool(true);
    w.Key("d");
    w.Null();
    w.Key("e");
    w.BeginArray();
    w.String("s");
    w.Number(static_cast<std::uint64_t>(7));
    w.EndArray();
    w.EndObject();
    CHECK_EQ(w.Take(), R"({"a":"x","b":-42,"c":true,"d":null,"e":["s",7]})");
}

TEST_GROUP(json_writer_pretty) {
    util::JsonWriter w(true);
    w.BeginObject();
    w.Key("k");
    w.BeginObject();
    w.Key("n");
    w.Number(static_cast<std::int64_t>(1));
    w.EndObject();
    w.EndObject();
    CHECK_EQ(w.Take(), "{\n  \"k\": {\n    \"n\": 1\n  }\n}");
}
