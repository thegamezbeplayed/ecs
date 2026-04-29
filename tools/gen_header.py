#!/usr/bin/env python3
import argparse, json, re, sys, pathlib

IDENT_RE = re.compile(r'[^A-Za-z0-9_]')
UPPER_NUM_RE = re.compile(r'^[A-Z_][A-Z0-9_]*$')

def to_ident(s: str) -> str:
    s = IDENT_RE.sub('_', s)
    s = re.sub(r'_+', '_', s).strip('_')
    if not s:
        s = 'KEY'
    if s[0].isdigit():
        s = '_' + s
    return s.upper()

def flatten(obj, parent_key=''):
    """Yield (path, value) for all scalar leaves; path is list[str]."""
    if isinstance(obj, dict):
        for k, v in obj.items():
            yield from flatten(v, parent_key + '.' + k if parent_key else k)
    elif isinstance(obj, list):
        # Lists are handled separately (we only emit arrays for uniform lists)
        yield (parent_key, obj)
    else:
        yield (parent_key, obj)

def is_int_list(lst):
    return all(isinstance(x, int) for x in lst)

def is_float_list(lst):
    return all(isinstance(x, (int, float)) for x in lst)

def is_str_list(lst):
    return all(isinstance(x, str) for x in lst)

def c_escape_string(s: str) -> str:
    return s.replace('\\', '\\\\').replace('"', '\\"')

def emit_value(name, val):
    lines = []
    if isinstance(val, bool):
        lines.append(f"#define {name} {1 if val else 0}")
    elif isinstance(val, int):
        lines.append(f"#define {name} {val}")
    elif isinstance(val, float):
        # Make sure it is clearly float for C
        txt = f"{val}"
        if '.' not in txt and 'e' not in txt.lower():
            txt += '.0'
        lines.append(f"#define {name} {txt}f")
    elif isinstance(val, str):
        esc = c_escape_string(val)
        lines.append(f'static const char *const {name} = "{esc}";')
    elif isinstance(val, list):
        if len(val) == 0:
            lines.append(f"/* {name}: empty array (no C symbol emitted) */")
        elif is_int_list(val):
            arr = ", ".join(str(x) for x in val)
            lines.append(f"static const int {name}[] = {{ {arr} }};")
            lines.append(f"static const unsigned int {name}_LEN = {len(val)};")
        elif is_float_list(val):
            arr = ", ".join((f'{x}.0' if isinstance(x, int) else str(x)) + ('f' if '.' in str(x) or 'e' in str(x).lower() else '.0f') for x in val)
            lines.append(f"static const float {name}[] = {{ {arr} }};")
            lines.append(f"static const unsigned int {name}_LEN = {len(val)};")
        elif is_str_list(val):
            arr = ", ".join(f'"{c_escape_string(x)}"' for x in val)
            lines.append(f"static const char *const {name}[] = {{ {arr} }};")
            lines.append(f"static const unsigned int {name}_LEN = {len(val)};")
        else:
            # Heterogeneous list → only available via embedded JSON
            lines.append(f"/* {name}: heterogeneous array; use embedded JSON string. */")
    else:
        lines.append(f"/* {name}: unsupported type {type(val)}; use embedded JSON string. */")
    return lines

def main():
    p = argparse.ArgumentParser(description="Generate a C header from a JSON file.")
    p.add_argument("input", help="path to input .json")
    p.add_argument("-o", "--output", help="path to output .h (default: <input>.h)")
    p.add_argument("--prefix", default="", help="symbol prefix (e.g., GAME_).")
    p.add_argument("--guard", default=None, help="custom include guard (auto if omitted)")
    p.add_argument("--embed", action="store_true", help="embed full JSON as a char literal")
    p.add_argument("--no-flatten", action="store_true", help="do not emit flattened constants/arrays")
    args = p.parse_args()

    in_path = pathlib.Path(args.input)
    out_path = pathlib.Path(args.output) if args.output else in_path.with_suffix(".h")
    json_text = in_path.read_text(encoding="utf-8")
    try:
        data = json.loads(json_text)
    except Exception as e:
        print(f"Failed to parse JSON: {e}", file=sys.stderr)
        sys.exit(1)

    base = to_ident(in_path.stem)
    guard = args.guard or f"{base}_GENERATED_H"
    prefix = args.prefix
    if prefix and not UPPER_NUM_RE.match(prefix):
        prefix = to_ident(prefix)

    lines = []
    lines.append(f"#ifndef {guard}")
    lines.append(f"#define {guard}")
    lines.append("")
    lines.append("/* This file is auto-generated. Do not edit by hand. */")
    lines.append("/* Source: " + in_path.name + " */")
    lines.append("")

    if args.embed:
        # Embed entire JSON as a single string constant
        esc = c_escape_string(json_text)
        lines.append(f"static const char {prefix}JSON_{base}[] =")
        # Split into manageable chunks for compilers if huge
        chunk = 8000
        for i in range(0, len(esc), chunk):
            part = esc[i:i+chunk]
            lines.append(f'    "{part}"')
        lines.append(";")
        lines.append("")

    if not args.no_flatten:
        # Emit flattened constants/arrays for leaves
        for path, val in flatten(data):
            if path == "":
                # pure scalar/array root
                name = prefix + base
            else:
                # Convert dotted path to symbol
                sym = to_ident(path.replace('.', '_'))
                name = f"{prefix}{sym}"

            lines.extend(emit_value(name, val))

    lines.append("")
    lines.append(f"#endif /* {guard} */")
    out_path.write_text("\n".join(lines), encoding="utf-8")
    print(f"Wrote {out_path}")

if __name__ == "__main__":
    main()
