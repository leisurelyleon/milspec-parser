#include "milspec/parser.hpp"

#include <sstream>
#include <string>
#include <vector>

namespace milspec {

namespace {

/// Trims surrounding ASCII whitespace from an extracted field value.
std::string trim(const std::string& text) {
    const std::size_t first = text.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return "";
    }
    const std::size_t last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

/// Splits a line on a delimiter into raw fields.
std::vector<std::string> split(const std::string& line, char delimiter) {
    std::vector<std::string> fields;
    std::string current;
    std::istringstream stream(line);
    while (std::getline(stream, current, delimiter)) {
        fields.push_back(current);
    }
    return fields;
}

} // namespace

ParsedRecord Parser::parse_line(const std::string& line, std::size_t line_no,
                                std::vector<Violation>& violations) const {
    ParsedRecord record;
    record.line = line_no;

    if (schema_.kind() == FormatKind::FixedWidth) {
        const std::size_t expected = schema_.record_width();
        if (line.size() < expected) {
            violations.emplace_back(line_no, line.size() + 1, "", Severity::Error,
                                    "line is shorter than the schema's record width (" +
                                        std::to_string(expected) + ")");
        }
        for (const FieldSpec& field : schema_.fields()) {
            if (field.start < line.size()) {
                const std::size_t avail = line.size() - field.start;
                const std::size_t take = field.width < avail ? field.width : avail;
                record.values[field.name] = trim(line.substr(field.start, take));
            } else {
                record.values[field.name] = "";
            }
        }
    } else {
        const std::vector<std::string> raw = split(line, schema_.delimiter());
        for (const FieldSpec& field : schema_.fields()) {
            if (field.index < raw.size()) {
                record.values[field.name] = trim(raw[field.index]);
            } else {
                violations.emplace_back(line_no, field.index + 1, field.name, Severity::Error,
                                        "missing delimited field at index " +
                                            std::to_string(field.index));
                record.values[field.name] = "";
            }
        }
    }

    return record;
}

} // namespace milspec
