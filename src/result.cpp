#include "milspec/result.hpp"

// ValidationReport's helpers (is_clean, error_count) are defined inline in the
// header. This translation unit exists so the library has a stable compilation
// unit for result-related symbols and to anchor future result logic (e.g.
// formatted report rendering) without changing the build graph.

namespace milspec {

// Renders a one-line human summary of a report.
std::string summarize(const ValidationReport& report) {
    return "records: " + std::to_string(report.records_read) + ", valid: " +
           std::to_string(report.records_valid) + ", violations: " +
           std::to_string(report.violations.size());
}

}  // namespace milspec
