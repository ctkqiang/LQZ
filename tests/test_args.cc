#include <string>
#include <vector>

#include "test_framework.h"
#include "util/args.h"

namespace {

// 把字面量列表转成 char* argv 风格,调用 ParseArgs
util::ParsedArgs Parse(std::initializer_list<const char*> args, int& rc,
                       std::string& error) {
    std::vector<std::string> storage(args.begin(), args.end());
    std::vector<char*> argv;
    argv.reserve(storage.size());
    for (auto& s : storage) argv.push_back(s.data());
    util::ParsedArgs out;
    rc = util::ParseArgs(static_cast<int>(argv.size()), argv.data(), out, error);
    return out;
}

}  // namespace

TEST_GROUP(args_subcommand_and_flags) {
    int rc = 0;
    std::string error;
    const auto a =
        Parse({"lqz", "dump-sms", "-s", "TEST123", "-o", "/tmp/out", "--json"},
              rc, error);
    CHECK(rc == 0);
    CHECK_EQ(a.command, "dump-sms");
    CHECK_EQ(a.serial, "TEST123");
    CHECK_EQ(a.outputDir, "/tmp/out");
    CHECK(a.jsonFlag);
    CHECK(!a.helpFlag);
    CHECK(!a.versionFlag);
    CHECK(a.positional.empty());
}

TEST_GROUP(args_long_equals_form) {
    int rc = 0;
    std::string error;
    const auto a = Parse({"lqz", "--serial=ABC", "--output=dir2", "dump-photos"},
                         rc, error);
    CHECK(rc == 0);
    CHECK_EQ(a.command, "dump-photos");
    CHECK_EQ(a.serial, "ABC");
    CHECK_EQ(a.outputDir, "dir2");
}

TEST_GROUP(args_help_version_priority) {
    int rc = 0;
    std::string error;
    const auto a = Parse({"lqz", "dump-sms", "--help"}, rc, error);
    CHECK(rc == 0);
    CHECK(a.helpFlag);
    const auto b = Parse({"lqz", "--version"}, rc, error);
    CHECK(rc == 0);
    CHECK(b.versionFlag);
    const auto c = Parse({"lqz", "-h", "-V"}, rc, error);
    CHECK(c.helpFlag);
    CHECK(c.versionFlag);
}

TEST_GROUP(args_double_dash_positional) {
    int rc = 0;
    std::string error;
    const auto a = Parse({"lqz", "connect", "--", "--json"}, rc, error);
    CHECK(rc == 0);
    CHECK_EQ(a.command, "connect");
    CHECK(!a.jsonFlag);
    CHECK_EQ(a.positional.size(), 1u);
    CHECK_EQ(a.positional[0], "--json");
}

TEST_GROUP(args_connect_positional) {
    int rc = 0;
    std::string error;
    const auto a = Parse({"lqz", "connect", "192.168.1.10"}, rc, error);
    CHECK(rc == 0);
    CHECK_EQ(a.command, "connect");
    CHECK_EQ(a.positional.size(), 1u);
    CHECK_EQ(a.positional[0], "192.168.1.10");
}

TEST_GROUP(args_errors) {
    int rc = 0;
    std::string error;

    Parse({"lqz", "dump-sms", "--bogus"}, rc, error);
    CHECK(rc != 0);
    CHECK(!error.empty());

    Parse({"lqz", "dump-sms", "-s"}, rc, error);
    CHECK(rc != 0);
    CHECK(!error.empty());

    Parse({"lqz", "dump-sms", "--output"}, rc, error);
    CHECK(rc != 0);
    CHECK(!error.empty());
}
