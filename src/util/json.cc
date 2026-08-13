#include "util/json.h"

namespace util {

std::string JsonEscape(std::string_view s) {
    std::string out;
    out.reserve(s.size());
    for (char ch : s) {
        const unsigned char c = static_cast<unsigned char>(ch);
        switch (c) {
            case '"':
                out += "\\\"";
                break;
            case '\\':
                out += "\\\\";
                break;
            case '\n':
                out += "\\n";
                break;
            case '\r':
                out += "\\r";
                break;
            case '\t':
                out += "\\t";
                break;
            case '\b':
                out += "\\b";
                break;
            case '\f':
                out += "\\f";
                break;
            default:
                if (c < 0x20) {
                    static const char* kHex = "0123456789ABCDEF";
                    out += "\\u00";
                    out.push_back(kHex[c >> 4]);
                    out.push_back(kHex[c & 0x0f]);
                } else {
                    out.push_back(ch);  // UTF-8 多字节直通
                }
        }
    }
    return out;
}

void JsonWriter::NewLineAndIndent() {
    if (!pretty_) return;
    out_ += '\n';
    out_.append(stack_.size() * 2, ' ');
}

void JsonWriter::BeforeValue() {
    if (stack_.empty()) return;
    if (afterKey_) {
        afterKey_ = false;  // 键后的值:不加逗号也不换行,紧跟键后
    } else {
        if (needComma_.back()) out_ += ',';  // 数组元素之间、键与键之间
        NewLineAndIndent();
    }
    needComma_.back() = true;
}

void JsonWriter::BeginObject() {
    BeforeValue();
    out_ += '{';
    stack_.push_back(true);
    needComma_.push_back(false);
}

void JsonWriter::EndObject() {
    stack_.pop_back();
    needComma_.pop_back();
    NewLineAndIndent();
    out_ += '}';
}

void JsonWriter::Key(std::string_view key) {
    BeforeValue();
    out_ += '"';
    out_ += JsonEscape(key);
    out_ += "\":";
    if (pretty_) out_ += ' ';
    afterKey_ = true;  // 键后的值不加逗号
}

void JsonWriter::String(std::string_view value) {
    BeforeValue();
    out_ += '"';
    out_ += JsonEscape(value);
    out_ += '"';
}

void JsonWriter::Number(std::int64_t value) {
    BeforeValue();
    out_ += std::to_string(value);
}

void JsonWriter::Number(std::uint64_t value) {
    BeforeValue();
    out_ += std::to_string(value);
}

void JsonWriter::Bool(bool value) {
    BeforeValue();
    out_ += value ? "true" : "false";
}

void JsonWriter::Null() {
    BeforeValue();
    out_ += "null";
}

void JsonWriter::BeginArray() {
    BeforeValue();
    out_ += '[';
    stack_.push_back(false);
    needComma_.push_back(false);
}

void JsonWriter::EndArray() {
    stack_.pop_back();
    needComma_.pop_back();
    NewLineAndIndent();
    out_ += ']';
}

}  // namespace util
