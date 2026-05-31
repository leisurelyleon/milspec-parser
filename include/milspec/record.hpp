#ifndef MILSPEC_RECORD_HPP
#define MILSPEC_RECORD_HPP

#include <cstddef>
#include <string>
#include <unordered_map>

namespace milspec {

/// A parsed record: field name -> raw extracted value, with source line.
struct ParsedRecord {
    std::size_t line; // 1-based source line this record came from
    std::unordered_map<std::string, std::string> values;

    [[nodiscard]] bool has(const std::string& field) const {
        return values.find(field) != values.end();
    }

    [[nodiscard]] const std::string& get(const std::string& field) const {
        static const std::string empty;
        const auto it = values.find(field);
        return it != values.end() ? it->second : empty;
    }
};

} // namespace milspec

#endif // MILSPEC_RECORD_HPP
