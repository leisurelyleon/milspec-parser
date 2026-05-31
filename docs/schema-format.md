# Schema Format

A schema is a UTF-8, line-oriented text file. Blank lines and lines beginning
with `#` are ignored.

## Format directive (required, once)

```text
format fixed
format delimited <delimiter-char>
```

`fixed` selects fixed-width parsing; `delimited` selects delimited parsing with
the given single-character delimiter (e.g. `,`).

## Field directives (one or more)

For delimited formats:

```text
field <name> <type> <required|optional> fixed <start> <width>
```

- `<type>` is one of `text`, `integer`, `decimal`, `date` (date is `YYYYMMDD`).
- `<index>` is the 0-based field position (delimited).
- `<start>`/`<width>` are 0-based column placement (fixed-width).

## Example

```text
format delimited ,
field nsn text required delim 0
field quantity integer required delim 2
```
