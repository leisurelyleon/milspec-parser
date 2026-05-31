#include "milspec/result.hpp"

#include <gtest/gtest.h>

#include "milspec/schema.hpp"

TEST(ParseOutcome, OkCarriesValue) {
    const auto outcome = milspec::ParseOutcome<int>::ok(7);
    ASSERT_TRUE(outcome.is_ok());
    EXPECT_EQ(outcome.value(), 7);
}

TEST(ParseOutcome, ErrCarriesMessage) {
    const auto outcome = milspec::ParseOutcome<int>::err("boom");
    ASSERT_TRUE(outcome.is_err());
    EXPECT_EQ(outcome.error(), "boom");
}

TEST(Result, SchemaLoadReportsMissingFormat) {
    const auto outcome = milspec::Schema::load("field x text required delim 0\n");
    ASSERT_TRUE(outcome.is_err());
    EXPECT_TRUE(outcome.error().find("format") != std::string::npos);
}

TEST(Result, SchemaLoadReportsNoFields) {
    const auto outcome = milspec::Schema::load("format delimited ,\n");
    ASSERT_TRUE(outcome.is_err());
    EXPECT_TRUE(outcome.error().find("no fields") != std::string::npos);
}

TEST(Result, SummarizeIncludesCounts) {
    milspec::ValidationReport report;
    report.records_read = 5;
    report.records_valid = 4;
    const std::string summary = milspec::summarize(report);
    EXPECT_TRUE(summary.find("5") != std::string::npos);
}
