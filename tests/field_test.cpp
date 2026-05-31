#include "milspec/field.hpp"

#include <gtest/gtest.h>

TEST(Field, TypeRoundTrip) {
    EXPECT_EQ(milspec::field_type_from_string("integer"), milspec::FieldType::Integer);
    EXPECT_EQ(milspec::field_type_from_string("date"), milspec::FieldType::Date);
    EXPECT_FALSE(milspec::field_type_from_string("nonsense").has_value());
}

TEST(Field, ValidatesIntegers) {
    EXPECT_FALSE(milspec::validate_type(milspec::FieldType::Integer, "12345").has_value());
    EXPECT_FALSE(milspec::validate_type(milspec::FieldType::Integer, "-42").has_value());
    EXPECT_TRUE(milspec::validate_type(milspec::FieldType::Integer, "12.3").has_value());
    EXPECT_TRUE(milspec::validate_type(milspec::FieldType::Integer, "abc").has_value());
    EXPECT_TRUE(milspec::validate_type(milspec::FieldType::Integer, "").has_value());
}

TEST(Field, ValidatesDecimals) {
    EXPECT_FALSE(milspec::validate_type(milspec::FieldType::Decimal, "3.14").has_value());
    EXPECT_FALSE(milspec::validate_type(milspec::FieldType::Decimal, "42").has_value());
    EXPECT_TRUE(milspec::validate_type(milspec::FieldType::Decimal, "1.2.3").has_value());
    EXPECT_TRUE(milspec::validate_type(milspec::FieldType::Decimal, "x").has_value());
}

TEST(Field, ValidatesDates) {
    EXPECT_FALSE(milspec::validate_type(milspec::FieldType::Date, "20260115").has_value());
    EXPECT_TRUE(milspec::validate_type(milspec::FieldType::Date, "20261315").has_value()); // bad month
    EXPECT_TRUE(milspec::validate_type(milspec::FieldType::Date, "2026").has_value());     // too short
}

TEST(Field, TextAlwaysTypeValid) {
    EXPECT_FALSE(milspec::validate_type(milspec::FieldType::Text, "anything").has_value());
}
