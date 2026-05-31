#include "milspec/field.hpp"

#include <algorithm>
#include <cctype>
#include <string>

namespace milspec {

std::string to_string(FieldType type) {
    switch (type) {
    case FieldType::Text:
        return "text";
    case FieldType::Integer:
        return "integer";
    case FieldType::Decimal:
        return "decimal";
    case FieldType::Date:
        return "date";
    }
    return "text";
}

std::optional<FieldType> field_type_from_string(const std::string& text) {
    if (text == "text") {
        return FieldType::Text;
    }
    if (text == "integer") {
        return FieldType::Integer;
    }
    if (text == "decimal") {
        return FieldType::Decimal;
    }
    if (text == "date") {
        return FieldType::Date;
    }
    return std::nullopt;
}

namespace {

bool is_all_digits(const std::string& value) {
    if (value.empty()) {
        return false;
    }
    return std::all_of(value.begin(), value.end(),
                       [](unsigned char ch) { return std::isdigit(ch) != 0; });
}

/// Validates an optionally-signed integer.
bool is_integer(const std::string& value) {
    if (value.empty()) {
        return false;
    }
    std::size_t start = 0;
    if (value[0] == '+' || value[0] == '-') {
        start = 1;
    }
    if (start >= value.size()) {
        return false;
    }
    return std::all_of(value.begin() + static_cast<std::string::difference_type>(start),
                       value.end(), [](unsigned char ch) { return std::isdigit(ch) != 0; });
}

/// Validates a decimal number: optional sign, digits, optional single dot.
bool is_decimal(const std::string& value) {
    if (value.empty()) {
        return false;
    }
    std::size_t start = 0;
    if (value[0] == '+' || value[0] == '-') {
        start = 1;
    }
    bool seen_dot = false;
    bool seen_digit = false;
    for (std::size_t i = start; i < value.size(); ++i) {
        const unsigned char ch = static_cast<unsigned char>(value[i]);
        if (ch == '.') {
            if (seen_dot) {
                return false;
            }
            seen_dot = true;
        } else if (std::isdigit(ch) != 0) {
            seen_digit = true;
        } else {
            return false;
        }
    }
    return seen_digit;
}

/// Validates a YYYYMMDD date (structure and basic range, not calendar-exact).
bool is_date(const std::string& value) {
    if (value.size() != 8 || !is_all_digits(value)) {
        return false;
    }
    const int month = std::stoi(value.substr(4, 2));
    const int day = std::stoi(value.substr(6, 2));
    return month >= 1 && month <= 12 && day >= 1 && day <= 31;
}

} // namespace

std::optional<std::string> validate_type(FieldType type, const std::string& value) {
    switch (type) {
    case FieldType::Text:
        return std::nullopt; // any text is type-valid; length checked elsewhere
    case FieldType::Integer:
        return is_integer(value) ? std::nullopt : std::optional<std::string>("expected an integer");
    case FieldType::Decimal:
        return is_decimal(value) ? std::nullopt
                                 : std::optional<std::string>("expected a decimal number");
    case FieldType::Date:
        return is_date(value) ? std::nullopt
                              : std::optional<std::string>("expected a YYYYMMDD date");
    }
    return std::nullopt;
}

} // namespace milspec
