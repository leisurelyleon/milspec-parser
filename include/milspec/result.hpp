#ifndef MILSPEC_RESULT_HPP
#define MILSPEC_RESULT_HPP

#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "milspec/violation.hpp"

namespace milspec {

/// A success-or-error outcome for operations that produce a value of type T
/// (e.g. loading a schema), without using exceptions.
template <typename T>
class ParseOutcome {
public:
    [[nodiscard]] static ParseOutcome ok(T value) { return ParseOutcome(std::move(value)); }
    [[nodiscard]] static ParseOutcome err(std::string message) {
        return ParseOutcome(Failure{std::move(message)});
    }

    [[nodiscard]] bool is_ok() const { return std::holds_alternative<T>(data_); }
    [[nodiscard]] bool is_err() const { return !is_ok(); }

    [[nodiscard]] const T& value() const { return std::get<T>(data_); }
    [[nodiscard]] const std::string& error() const { return std::get<Failure>(data_).message; }

private:
    struct Failure {
        std::string message;
    };

    explicit ParseOutcome(T value) : data_(std::move(value)) {}
    explicit ParseOutcome(Failure failure) : data_(std::move(failure)) {}

    std::variant<T, Failure> data_;
};

/// The aggregate result of validating a data file: how many records were read,
/// and every violation found across all of them.
struct ValidationReport {
    std::size_t records_read{0};
    std::size_t records_valid{0};
    std::vector<Violation> violations;

    [[nodiscard]] bool is_clean() const { return violations.empty(); }

    [[nodiscard]] std::size_t error_count() const {
        std::size_t count = 0;
        for (const Violation& v : violations) {
            if (v.severity == Severity::Error) {
                ++count;
            }
        }
        return count;
    }
};

}  // namespace milspec

#endif  // MILSPEC_RESULT_HPP
