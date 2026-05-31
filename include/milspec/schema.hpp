#ifndef MILSPEC_SCHEMA_HPP
#define MILSPEC_SCHEMA_HPP

#include <string>
#include <vector>

#include "milspec/field.hpp"
#include "milspec/result.hpp"

namespace milspec {

/// The structural format a schema describes.
enum class FormatKind { FixedWidth, Delimited };

/// An ordered collection of field specifications plus the record format.
class Schema {
public:
    Schema(FormatKind kind, char delimiter = ',') : kind_(kind), delimiter_(delimiter) {}

    void add_field(FieldSpec field) { fields_.push_back(std::move(field)); }

    [[nodiscard]] FormatKind kind() const { return kind_; }
    [[nodiscard]] char delimiter() const { return delimiter_; }
    [[nodiscard]] const std::vector<FieldSpec>& fields() const { return fields_; }
    [[nodiscard]] std::size_t field_count() const { return fields_.size(); }

    /// For fixed-width schemas, the total expected record width.
    [[nodiscard]] std::size_t record_width() const;

    /// Loads a schema from its declarative text form. On a malformed schema,
    /// returns an error result describing the problem.
    [[nodiscard]] static ParseOutcome<Schema> load(const std::string& text);

private:
    FormatKind kind_;
    char delimiter_;
    std::vector<FieldSpec> fields_;
};

}  // namespace milspec

#endif  // MILSPEC_SCHEMA_HPP
