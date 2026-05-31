#ifndef MILSPEC_VIOLATION_HPP
#define MILSPEC_VIOLATION_HPP

#include <cstddef>
#include <string>

namespace milspec {

/// How serious a violation is.
enum class Severity { Error, Warning };

/// A single validation problem, located precisely within the input.
struct Violation {
    std::size_t line;    // 1-based line number in the source
    std::size_t column;  // 1-based column (or field start) where the problem is
    std::string field;   // the field name involved (empty for structural issues)
    Severity severity;
    std::string message;

    Violation(std::size_t line_no, std::size_t column_no, std::string field_name,
              Severity sev, std::string msg)
        : line(line_no), column(column_no), field(std::move(field_name)), severity(sev),
          message(std::move(msg)) {}
};

}  // namespace milspec

#endif  // MILSPEC_VIOLATION_HPP
