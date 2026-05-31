#include "milspec/schema.hpp"
#include "milspec/validator.hpp"

#include <gtest/gtest.h>

namespace {

const char* kSchema = "format delimited ,\n"
                      "field sku text required delim 0\n"
                      "field qty integer required delim 1\n";

milspec::Schema load_or_die() {
    const auto outcome = milspec::Schema::load(kSchema);
    EXPECT_TRUE(outcome.is_ok());
    return outcome.value();
}

} // namespace

TEST(Validator, AcceptsValidRecord) {
    const milspec::Schema schema = load_or_die();
    const milspec::Validator validator(schema);
    const milspec::ValidationReport report = validator.validate_input("WIDGET,42\n");

    EXPECT_TRUE(report.is_clean());
    EXPECT_EQ(report.records_read, 1u);
    EXPECT_EQ(report.records_valid, 1u);
}

TEST(Validator, FlagsBadInteger) {
    const milspec::Schema schema = load_or_die();
    const milspec::Validator validator(schema);
    const milspec::ValidationReport report = validator.validate_input("WIDGET,notanumber\n");

    EXPECT_FALSE(report.is_clean());
    EXPECT_EQ(report.error_count(), 1u);
    EXPECT_EQ(report.violations[0].field, "qty");
}

TEST(Validator, FlagsRequiredEmpty) {
    const milspec::Schema schema = load_or_die();
    const milspec::Validator validator(schema);
    // Empty sku field (leading comma).
    const milspec::ValidationReport report = validator.validate_input(",42\n");

    EXPECT_FALSE(report.is_clean());
    EXPECT_TRUE(report.violations[0].message.find("required") != std::string::npos);
}

TEST(Validator, ReportsAllViolationsInOnePass) {
    const milspec::Schema schema = load_or_die();
    const milspec::Validator validator(schema);
    // Both fields bad: empty sku AND non-integer qty.
    const milspec::ValidationReport report = validator.validate_input(",bad\n");

    // Total validation: more than one violation reported for the same record.
    EXPECT_GE(report.violations.size(), 2u);
}

TEST(Validator, CountsRecordsAcrossLines) {
    const milspec::Schema schema = load_or_die();
    const milspec::Validator validator(schema);
    const milspec::ValidationReport report =
        validator.validate_input("A,1\nB,2\nC,3\n# a comment\n\n");

    EXPECT_EQ(report.records_read, 3u); // blanks and comments skipped
    EXPECT_EQ(report.records_valid, 3u);
}
