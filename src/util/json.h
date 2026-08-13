#ifndef UTIL_JSON_H
#define UTIL_JSON_H

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

// 极简 JSON 写入器(只写不读,零依赖)。
// 紧凑模式供 custody 单行记录使用;缩进模式供 manifest.json 等人类可读文件使用。
// 约定:键顺序由调用顺序决定且固定不变(custody 哈希链校验依赖这一点)。
namespace util {

// 转义规则:" \ \n \r \t \b \f;其余控制字符(<0x20)转 \u00XX 大写;
// UTF-8 多字节原样直通(中文路径无需转义)
std::string JsonEscape(std::string_view s);

class JsonWriter {
   public:
    explicit JsonWriter(bool pretty) : pretty_(pretty) {}

    void BeginObject();
    void EndObject();
    void Key(std::string_view key);
    void String(std::string_view value);
    void Number(std::int64_t value);
    void Number(std::uint64_t value);
    void Bool(bool value);
    void Null();
    void BeginArray();
    void EndArray();

    std::string Take() { return std::move(out_); }

   private:
    void BeforeValue();
    void NewLineAndIndent();

    bool pretty_;
    std::string out_;
    std::vector<bool> stack_;       // true = object, false = array
    std::vector<bool> needComma_;   // 当前容器是否已写过值
    bool afterKey_ = false;         // 刚写过键:下一个值前不加逗号
};

}  // namespace util

#endif
