#ifndef MILSPEC_PARSER_HPP
#define MILSPEC_PARSER_HPP

#include <cstddef>
#include <string>
#include <vector>

#include "milspec/record.hpp"
#include "milspec/result.hpp"
#include "milspec/schema.hpp"

namespace milspec {

/// Splits raw input lines into structured records according to a schema's
/// format. Parsing is concerned only with *extraction*; field validation is the
/// validator's job. Structural problems (e.g. a short fixed-width line) are
/// reported as violations.
class Parser {
public:
    explicit Parser(const Schema& schema) : schema_(schema) {}

    /// Parses one line into a record, appending any structural violations.
    /// `line_no` is the 1-based source line number.
    [[nodiscard]] ParsedRecord parse_line(const std::string& line, std::size_t line_no,
                                          std::vector<Violation>& violations) const;

private:
    const Schema& schema_;
};

}  // namespace milspec

#endif  // MILSPEC_PARSER_HPP
