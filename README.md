# Immutant Bytecode Interpreter
Immutant is a general purpose programming language designed by Rolands Fridemanis with focus on enforcing immutability at design level. 

Extensive specification and original AST-walk interpreter can be fond here: https://github.com/rolandsfr/immutant

## Instruction set

| Opcode | Size | Description |
|--------|------|-------------|
| `OP_RETURN` | 1 | Return from the current function |
| `OP_CONST` | 2 | Load constant at 1-byte pool index |
| `OP_CONST_LONG` | 4 | Load constant at 3-byte pool index |

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

## Testing

After changing tests (rebuild first, then run):

```bash
cmake --build build
ctest --test-dir build --output-on-failure
```

Re-run `cmake -S . -B build` if you add a new test file to `CMakeLists.txt`.
