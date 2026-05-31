# 1. Declarative, data-driven schema

- Status: Accepted
- Date: 2026-05

## Context

Federal records come in many fixed layouts. Hardcoding any single layout would
make the tool a one-off rather than a reusable validator.

## Decision

Model the record format as a `Schema` value — an ordered set of field
specifications plus a format kind — loaded from a declarative text spec. The
parser and validator operate against the schema, not against any specific
layout.

## Consequences

- New formats are added as data, with no code changes.
- The same engine and tests cover all formats uniformly.
- The schema spec itself becomes a documented, shareable artifact.
