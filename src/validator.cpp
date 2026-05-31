#include "milspec/validator.hpp"

#include <sstream>
#include <string>

#include "milspec/field.hpp"
#include "milspec/parser.hpp"

namespace milspec {

/// Column to report a field violation at: its fixed-width start, or its
/// delimited index, both made 1-based for human readability.
namespace {

std::size_t field_column(const Schema& schema, const FieldSpec& field) {
    return schema.kind() == FormatKind::FixedWidth ? field.start + 1 : field.index + 1;
}

}  // namespace

void Validator::validate_record(const ParsedRecord& record,
                                std::vector<Violation>& violations) const {
    for (const FieldSpec& field : schema_.fields()) {
        const std::size_t column = field_column(schema_, field);
        const std::string& value = record.get(field.name);

        // Required-but-empty.
        if (value.empty()) {
            if (field.required) {
                violations.emplace_back(record.line, column, field.name, Severity::Error,
                                        "required field is empty");
            }
            continue;  // nothing further to check on an empty value
        }

        // Type validity.
        if (const std::optional<std::string> type_error = validate_type(field.type, value)) {
            violations.emplace_back(record.line, column, field.name, Severity::Error,
                                    type_error.value());
            continue;
        }

        // Exact-length constraint (text).
        if (field.exact_length.has_value() && value.size() != field.exact_length.value()) {
            violations.emplace_back(record.line, column, field.name, Severity::Error,
                                    "expected exactly " +
                                        std::to_string(field.exact_length.value()) +
                                        " characters");
        }

        // Numeric range constraints.
        if (field.type == FieldType::Integer &&
            (field.min_value.has_value() || field.max_value.has_value())) {
            const long long number = std::stoll(value);
            if (field.min_value.has_value() && number < field.min_value.value()) {
                violations.emplace_back(record.line, column, field.name, Severity::Error,
                                        "value below minimum of " +
                                            std::to_string(field.min_value.value()));
            }
            if (field.max_value.has_value() && number > field.max_value.value()) {
                violations.emplace_back(record.line, column, field.name, Severity::Error,
                                        "value above maximum of " +
                                            std::to_string(field.max_value.value()));
            }
        }
    }
}

ValidationReport Validator::validate_input(const std::string& input) const {
    ValidationReport report;
    Parser parser(schema_);

    std::istringstream stream(input);
    std::string line;
    std::size_t line_no = 0;

    while (std::getline(stream, line)) {
        ++line_no;
        // Skip blank lines and comments.
        if (line.find_first_not_of(" \t\r\n") == std::string::npos || line[0] == '#') {
            continue;
        }

        ++report.records_read;
        const std::size_t before = report.violations.size();

        const ParsedRecord record = parser.parse_line(line, line_no, report.violations);
        validate_record(record, report.violations);

        if (report.violations.size() == before) {
            ++report.records_valid;
        }
    }

    return report;
}

}  // namespace milspec
