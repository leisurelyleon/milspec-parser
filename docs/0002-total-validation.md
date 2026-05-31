# 2. Total validation (report all violations)

- Status: Accepted
- Date: 2026-05

## Context

When validating a record, stopping at the first error forces operators to fix
and re-run repeatedly to discover all problems — slow and error-prone for
batch federal data.

## Decision

Validate every field of every record and collect all violations, rather than
stopping at the first failure. The report contains the complete set.

## Consequences

- An operator sees every problem in a single pass.
- Reports are larger but far more actionable.
- Validation has no early-exit, so cost is proportional to record size.
