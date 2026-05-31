# milspec-parser

> A strict fixed-format data parser and validator in C++20 for federal logistics records.

`milspec-parser` parses fixed-width and delimited record formats against a
declarative schema and validates every field. It reports each violation with a
precise line and column, and it refuses to silently accept malformed input —
making it suitable for auditable, correctness-critical data pipelines where a
quietly-accepted bad record is unacceptable.

## The Problem

Federal logistics data arrives in rigid, legacy formats — fixed-width columns
or strict delimited layouts — where a misaligned field or an out-of-range value
must be caught and reported precisely, not guessed at or silently dropped.
`milspec-parser` validates against a schema and surfaces every problem with
exact positions, so malformed input fails loudly and diagnosably.

## Architecture

```
include/milspec/   public interface (headers)
src/                library: field types, schema, parser, validator, result
src/cli/            the milspec CLI: validate / describe
tests/              GoogleTest suite (fetched via CMake)
```

The parser is driven by a declarative schema rather than any hardcoded layout.
See [`docs/architecture.md`](docs/architecture.md) and
[`docs/schema-format.md`](docs/schema-format.md).

## Requirements

- A C++20 compiler (GCC 11+, Clang 14+, or MSVC 2022)
- CMake 3.20+

## Build & Test

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## Run

```bash
./build/bin/milspec-cli validate schemas/dla-asset.schema data/samples/valid.dat
./build/bin/milspec-cli validate schemas/dla-asset.schema data/samples/invalid.dat
./build/bin/milspec-cli describe schemas/dla-asset.schema
```

## License

MIT — see [LICENSE](LICENSE).
