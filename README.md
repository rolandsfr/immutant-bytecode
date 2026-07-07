# Immutant Bytecode Interpreter
Immutant is a general purpose programming language designed by Rolands Fridemanis with focus on enforcing immutability at design level. 

Extensive specification and original AST-walk interpreter can be fond here: https://github.com/rolandsfr/immutant

## Instruction set

| Opcode | Size | Description |
|--------|------|-------------|
| `OP_RETURN` | 1 | Pop top stack value, print it, and halt |
| `OP_CONST` | 2 | Push constant at 1-byte pool index |
| `OP_CONST_LONG` | 4 | Push constant at 3-byte big-endian pool index |
| `OP_NEGATE` | 1 | Pop top value, push its negation |
| `OP_ADD` | 1 | Pop `b`, pop `a`, push `a + b` |
| `OP_SUB` | 1 | Pop `b`, pop `a`, push `a - b` |
| `OP_MUL` | 1 | Pop `b`, pop `a`, push `a * b` |
| `OP_DIV` | 1 | Pop `b`, pop `a`, push `a / b` |

Binary ops use stack order: the first value pushed becomes `a`, the second becomes `b`.

## Building

First time, or after changing `CMakeLists.txt`:

```bash
cmake -S . -B build
```

After changing source files:

```bash
cmake --build build
./build/immutant
```

To print a disassembly trace and stack dump on each instruction during execution, configure with `DEBUG_TRACE_EXECUTION`:

```bash
cmake -S . -B build -DCMAKE_CXX_FLAGS="-DDEBUG_TRACE_EXECUTION"
cmake --build build
```

Re-run `cmake -S . -B build` without that flag (or delete `build/`) to turn tracing off.

## Testing

After changing tests (rebuild first, then run):

```bash
cmake --build build
ctest --test-dir build --output-on-failure
```

Re-run `cmake -S . -B build` if you add a new test file to `CMakeLists.txt`.

## License

Licensed under the Apache License, Version 2.0. See [LICENSE](LICENSE) for details.
