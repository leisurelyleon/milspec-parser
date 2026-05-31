#include "milspec/parser.hpp"
#include "milspec/schema.hpp"
#include "milspec/violation.hpp"

#include <gtest/gtest.h>
#include <vector>

namespace {

milspec::Schema delimited_schema() {
    milspec::Schema schema(milspec::FormatKind::Delimited, ',');
    milspec::FieldSpec a;
    a.name = "sku";
    a.type = milspec::FieldType::Text;
    a.index = 0;
    milspec::FieldSpec b;
    b.name = "qty";
    b.type = milspec::FieldType::Integer;
    b.index = 1;
    schema.add_field(a);
    schema.add_field(b);
    return schema;
}

milspec::Schema fixed_schema() {
    milspec::Schema schema(milspec::FormatKind::FixedWidth);
    milspec::FieldSpec a;
    a.name = "code";
    a.type = milspec::FieldType::Text;
    a.start = 0;
    a.width = 4;
    milspec::FieldSpec b;
    b.name = "amount";
    b.type = milspec::FieldType::Integer;
    b.start = 4;
    b.width = 6;
    schema.add_field(a);
    schema.add_field(b);
    return schema;
}

} // namespace

TEST(Parser, DelimitedExtractsFields) {
    const milspec::Schema schema = delimited_schema();
    milspec::Parser parser(schema);
    std::vector<milspec::Violation> violations;

    const milspec::ParsedRecord record = parser.parse_line("WIDGET,42", 1, violations);
    EXPECT_TRUE(violations.empty());
    EXPECT_EQ(record.get("sku"), "WIDGET");
    EXPECT_EQ(record.get("qty"), "42");
}

TEST(Parser, DelimitedMissingFieldReportsViolation) {
    const milspec::Schema schema = delimited_schema();
    milspec::Parser parser(schema);
    std::vector<milspec::Violation> violations;

    parser.parse_line("WIDGET", 3, violations); // missing qty
    ASSERT_EQ(violations.size(), 1u);
    EXPECT_EQ(violations[0].line, 3u);
    EXPECT_EQ(violations[0].field, "qty");
}

TEST(Parser, FixedWidthExtractsColumns) {
    const milspec::Schema schema = fixed_schema();
    milspec::Parser parser(schema);
    std::vector<milspec::Violation> violations;

    const milspec::ParsedRecord record = parser.parse_line("ABCD000123", 1, violations);
    EXPECT_TRUE(violations.empty());
    EXPECT_EQ(record.get("code"), "ABCD");
    EXPECT_EQ(record.get("amount"), "000123");
}

TEST(Parser, FixedWidthShortLineReportsViolation) {
    const milspec::Schema schema = fixed_schema();
    milspec::Parser parser(schema);
    std::vector<milspec::Violation> violations;

    parser.parse_line("ABC", 5, violations); // shorter than record width (10)
    EXPECT_FALSE(violations.empty());
    EXPECT_EQ(violations[0].line, 5u);
}
