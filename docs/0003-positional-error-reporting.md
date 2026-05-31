# 3. Positional error reporting

- Status: Accepted
- Date: 2026-05

## Context

"Record 5 is invalid" is not actionable. Auditable pipelines need to know
exactly where a problem is.

## Decision

Every violation carries a 1-based line and column (for fixed-width, the field's
start column; for delimited, its field index), plus the field name and a
specific message.

## Consequences

- Problems are located precisely, aiding correction and audit.
- The report format is uniform across formats.
- Positions are 1-based for human readability, converted from 0-based internals.
