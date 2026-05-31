#ifndef MILSPEC_VALIDATOR_HPP
#define MILSPEC_VALIDATOR_HPP

#include "milspec/record.hpp"
#include "milspec/result.hpp"
#include "milspec/schema.hpp"

#include <string>
#include <vector>

namespace milspec {

/// Applies a schema's field rules to parsed records. Validation is *total*:
/// every field is checked and every violation reported, rather than stopping at
/// the first failure.
class Validator {
public:
    explicit Validator(const Schema& schema) : schema_(schema) {}

    /// Validates a single parsed record, appending violations.
    void validate_record(const ParsedRecord& record, std::vector<Violation>& violations) const;

    /// Validates an entire multi-line input, returning a full report.
    [[nodiscard]] ValidationReport validate_input(const std::string& input) const;

private:
    const Schema& schema_;
};

} // namespace milspec

#endif // MILSPEC_VALIDATOR_HPP
