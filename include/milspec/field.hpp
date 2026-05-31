#ifndef MILSPEC_FIELD_HPP
#define MILSPEC_FIELD_HPP

#include <cstddef>
#include <optional>
#include <string>

namespace milspec {

/// The value type a field is expected to hold.
enum class FieldType { Text, Integer, Decimal, Date };

[[nodiscard]] std::string to_string(FieldType type);
[[nodiscard]] std::optional<FieldType> field_type_from_string(const std::string& text);

/// A declarative description of one field in a record.
///
/// For fixed-width formats, `start` and `width` locate the field. For delimited
/// formats, `index` gives the field's position. A spec carries both; the parser
/// uses whichever the schema's format requires.
struct FieldSpec {
    std::string name;
    FieldType type{FieldType::Text};
    bool required{true};

    // Fixed-width placement (0-based start column, character width).
    std::size_t start{0};
    std::size_t width{0};

    // Delimited placement (0-based field index).
    std::size_t index{0};

    // Optional range constraints for numeric types.
    std::optional<long long> min_value;
    std::optional<long long> max_value;

    // Optional exact-length constraint for text.
    std::optional<std::size_t> exact_length;
};

/// Checks whether a raw string value is well-formed for `type`. Returns an
/// error message when invalid, or std::nullopt when valid.
[[nodiscard]] std::optional<std::string> validate_type(FieldType type, const std::string& value);

} // namespace milspec

#endif // MILSPEC_FIELD_HPP
