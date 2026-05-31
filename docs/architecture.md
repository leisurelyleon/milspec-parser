# Architecture

`milspec-parser` is a C++20 library (`milspec`) plus a thin CLI, organized so
that all parsing and validation logic is testable independently of the
command-line wrapper.

## Layout

```text
include/milspec/   public interface (headers)
src/                library: field types, schema, parser, validator, result
src/cli/            the milspec CLI: validate / describe
tests/              GoogleTest suite, fetched via CMake FetchContent
```

## Components

- **field** — field types (text, integer, decimal, date) and per-type value
  validation.
- **schema** — a declarative, ordered set of field specifications plus the
  record format (fixed-width or delimited), loadable from a text spec.
- **parser** — extracts raw field values from a line according to the schema's
  format; reports structural problems (short lines, missing fields).
- **validator** — applies the schema's rules to extracted values. Validation is
  *total*: every field is checked and every violation reported.
- **result** — `ParseOutcome<T>` for success-or-error returns without
  exceptions, and `ValidationReport` aggregating all violations.

## Why schema-driven

The parser and validator are driven entirely by a `Schema` value rather than any
hardcoded layout. New record formats are expressed as data, not code, which
keeps the engine reusable and the logic uniformly testable.

## Error reporting

Every violation carries a 1-based line and column (or field position) plus the
field name and a message, so a malformed record is diagnosable precisely rather
than reported as an opaque failure.
