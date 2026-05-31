#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "milspec/result.hpp"
#include "milspec/schema.hpp"
#include "milspec/validator.hpp"

namespace {

/// Reads an entire file into a string. Returns false on failure.
bool read_file(const std::string& path, std::string& out) {
    std::ifstream file(path);
    if (!file) {
        return false;
    }
    std::ostringstream buffer;
    buffer << file.rdbuf();
    out = buffer.str();
    return true;
}

int cmd_validate(const std::string& schema_path, const std::string& data_path) {
    std::string schema_text;
    if (!read_file(schema_path, schema_text)) {
        std::cerr << "error: could not read schema '" << schema_path << "'\n";
        return 2;
    }

    const milspec::ParseOutcome<milspec::Schema> loaded = milspec::Schema::load(schema_text);
    if (loaded.is_err()) {
        std::cerr << "schema error: " << loaded.error() << "\n";
        return 2;
    }

    std::string data_text;
    if (!read_file(data_path, data_text)) {
        std::cerr << "error: could not read data '" << data_path << "'\n";
        return 2;
    }

    const milspec::Validator validator(loaded.value());
    const milspec::ValidationReport report = validator.validate_input(data_text);

    std::cout << milspec::summarize(report) << "\n";

    if (!report.is_clean()) {
        std::cout << "\nViolations:\n";
        for (const milspec::Violation& v : report.violations) {
            std::cout << "  line " << v.line << ", col " << v.column;
            if (!v.field.empty()) {
                std::cout << " [" << v.field << "]";
            }
            std::cout << ": " << v.message << "\n";
        }
        return 1;
    }

    std::cout << "All records valid.\n";
    return 0;
}

int cmd_describe(const std::string& schema_path) {
    std::string schema_text;
    if (!read_file(schema_path, schema_text)) {
        std::cerr << "error: could not read schema '" << schema_path << "'\n";
        return 2;
    }

    const milspec::ParseOutcome<milspec::Schema> loaded = milspec::Schema::load(schema_text);
    if (loaded.is_err()) {
        std::cerr << "schema error: " << loaded.error() << "\n";
        return 2;
    }

    const milspec::Schema& schema = loaded.value();
    std::cout << "Format: "
              << (schema.kind() == milspec::FormatKind::FixedWidth ? "fixed-width" : "delimited")
              << "\n";
    std::cout << "Fields: " << schema.field_count() << "\n";
    for (const milspec::FieldSpec& field : schema.fields()) {
        std::cout << "  " << field.name << " (" << milspec::to_string(field.type) << ", "
                  << (field.required ? "required" : "optional") << ")\n";
    }
    return 0;
}

void print_usage() {
    std::cout << "milspec - strict fixed-format data parser and validator\n\n";
    std::cout << "Commands:\n";
    std::cout << "  validate <schema> <data>   Validate a data file against a schema\n";
    std::cout << "  describe <schema>          Print a schema's structure\n";
}

}  // namespace

int main(int argc, char** argv) {
    if (argc < 2) {
        print_usage();
        return 2;
    }

    const std::string command = argv[1];

    if (command == "validate") {
        if (argc < 4) {
            std::cerr << "usage: milspec validate <schema> <data>\n";
            return 2;
        }
        return cmd_validate(argv[2], argv[3]);
    }

    if (command == "describe") {
        if (argc < 3) {
            std::cerr << "usage: milspec describe <schema>\n";
            return 2;
        }
        return cmd_describe(argv[2]);
    }

    if (command == "--help" || command == "-h") {
        print_usage();
        return 0;
    }

    std::cerr << "unknown command: " << command << "\n";
    print_usage();
    return 2;
}
