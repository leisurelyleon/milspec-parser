#include "milspec/schema.hpp"

#include <sstream>
#include <string>
#include <vector>

namespace milspec {

std::size_t Schema::record_width() const {
    std::size_t width = 0;
    for (const FieldSpec& field : fields_) {
        const std::size_t end = field.start + field.width;
        if (end > width) {
            width = end;
        }
    }
    return width;
}

namespace {

/// Trims surrounding ASCII whitespace.
std::string trim(const std::string& text) {
    const std::size_t first = text.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return "";
    }
    const std::size_t last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

} // namespace

ParseOutcome<Schema> Schema::load(const std::string& text) {
    // Schema format (one directive per line):
    //   format fixed | format delimited <char>
    //   field <name> <type> <required|optional> fixed <start> <width>
    //   field <name> <type> <required|optional> delim <index>
    std::istringstream stream(text);
    std::string line;

    FormatKind kind = FormatKind::Delimited;
    char delimiter = ',';
    bool format_seen = false;
    std::vector<FieldSpec> fields;

    std::size_t line_no = 0;
    while (std::getline(stream, line)) {
        ++line_no;
        const std::string trimmed = trim(line);
        if (trimmed.empty() || trimmed[0] == '#') {
            continue;
        }

        std::istringstream parts(trimmed);
        std::string keyword;
        parts >> keyword;

        if (keyword == "format") {
            std::string kind_word;
            parts >> kind_word;
            if (kind_word == "fixed") {
                kind = FormatKind::FixedWidth;
            } else if (kind_word == "delimited") {
                kind = FormatKind::Delimited;
                std::string delim_word;
                if (parts >> delim_word && !delim_word.empty()) {
                    delimiter = delim_word[0];
                }
            } else {
                return ParseOutcome<Schema>::err("unknown format kind on line " +
                                                 std::to_string(line_no));
            }
            format_seen = true;
        } else if (keyword == "field") {
            FieldSpec spec;
            std::string type_word;
            std::string req_word;
            std::string placement;
            if (!(parts >> spec.name >> type_word >> req_word >> placement)) {
                return ParseOutcome<Schema>::err("malformed field on line " +
                                                 std::to_string(line_no));
            }

            const std::optional<FieldType> type = field_type_from_string(type_word);
            if (!type.has_value()) {
                return ParseOutcome<Schema>::err("unknown field type '" + type_word + "' on line " +
                                                 std::to_string(line_no));
            }
            spec.type = type.value();
            spec.required = (req_word == "required");

            if (placement == "fixed") {
                unsigned long long start = 0;
                unsigned long long width = 0;
                if (!(parts >> start >> width)) {
                    return ParseOutcome<Schema>::err("fixed field needs start and width on line " +
                                                     std::to_string(line_no));
                }
                spec.start = static_cast<std::size_t>(start);
                spec.width = static_cast<std::size_t>(width);
            } else if (placement == "delim") {
                unsigned long long index = 0;
                if (!(parts >> index)) {
                    return ParseOutcome<Schema>::err("delimited field needs an index on line " +
                                                     std::to_string(line_no));
                }
                spec.index = static_cast<std::size_t>(index);
            } else {
                return ParseOutcome<Schema>::err("unknown placement '" + placement + "' on line " +
                                                 std::to_string(line_no));
            }

            fields.push_back(std::move(spec));
        } else {
            return ParseOutcome<Schema>::err("unknown directive '" + keyword + "' on line " +
                                             std::to_string(line_no));
        }
    }

    if (!format_seen) {
        return ParseOutcome<Schema>::err("schema is missing a 'format' directive");
    }
    if (fields.empty()) {
        return ParseOutcome<Schema>::err("schema defines no fields");
    }

    Schema schema(kind, delimiter);
    for (FieldSpec& field : fields) {
        schema.add_field(std::move(field));
    }
    return ParseOutcome<Schema>::ok(std::move(schema));
}

} // namespace milspec
