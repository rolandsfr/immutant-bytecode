---
name: write-unit-tests
description: Writes Google Test unit tests for the Immutant bytecode interpreter (C++17). Asserts correct specified behavior, not whatever the current implementation does. Use when adding tests, writing test cases, improving coverage, linking test targets in CMake, or running ctest in this repository. Do not use to weaken failing tests to match buggy code.
---

# Write Unit Tests

## Quick start

1. Read the component under test (`src/*.hpp`, `src/*.cpp`) and any README or spec before writing tests.
2. Add or extend `tests/<Component>.test.cpp` for the module being tested.
3. Register the test file and link the production sources under test in `immutant_test` (`CMakeLists.txt`).
4. Build and run: `cmake --build build && ctest --test-dir build --output-on-failure`
5. Re-run `cmake -S . -B build` when adding a new test file or changing the test target.

If tests fail, report failures — do not alter tests to match the implementation.

## Scope

**Follow the user's request, not full component coverage.**

- Write only the tests the user asked for — specific methods, behaviors, files, or counts (e.g. "one test for `write`", "boundary cases for `writeConstant` only").
- **Extend** means add to the named test file; do not rewrite unrelated tests or expand to other APIs unless asked.
- Use **Deriving coverage** below only when the user asks for broad coverage (e.g. "test Chunk", "improve coverage") or gives no scope. Otherwise, read the API for context but implement only the requested cases.
- Do not replace placeholder or unrelated tests unless the user targets that file for cleanup or asks to fix placeholders.
- Change `CMakeLists.txt` only as needed for the tests you add (new file, missing source link, include path).

If the request is ambiguous, prefer a small focused change over a comprehensive suite. Mention gaps the user did not ask for instead of filling them unprompted.

## Test file layout

```cpp
#include <gtest/gtest.h>
#include "Component.hpp"

TEST(ComponentTest, DescribesOneBehavior) {
    // arrange, act, assert
    EXPECT_EQ(/* expected */, /* actual */);
}
```

Conventions:

- **File name**: `tests/<Component>.test.cpp`
- **Suite name**: `<Component>Test` matching the class or module under test
- **Test name**: PascalCase describing one behavior (e.g. `EmptyStateReturnsZero`, `OutOfRangeThrows`)
- **Includes**: headers from `src/` using quoted includes (`"Chunk.hpp"`, `"common.hpp"`)

## CMake setup

The test executable is `immutant_test`. When testing production code, add the test file, link the relevant sources, and expose headers:

```cmake
add_executable(
  immutant_test
  tests/Component.test.cpp
  src/Component.cpp
)

target_include_directories(immutant_test PRIVATE src)
```

- Add new `tests/*.test.cpp` files to the `add_executable` list.
- Link only the production sources needed for the code under test.
- Do not link `src/main.cpp`.
- Add `target_include_directories(immutant_test PRIVATE src)` when tests `#include` headers from `src/`.

## Deriving coverage

Use this section when the user requests broad coverage or does not narrow scope. Do not treat it as a mandatory checklist for every request.

For each component, derive cases from the code and spec:

1. **Read first** — public API (`*.hpp`), implementation (`*.cpp`), `README.md`, opcode tables, and existing tests in `tests/`.
2. **Happy path** — each public method or opcode path produces the expected observable result.
3. **State transitions** — empty → populated, append, reset, and multi-step sequences.
4. **Boundaries** — off-by-one limits, encoding size thresholds, min/max indices, first/last valid values.
5. **Errors** — invalid input, out-of-range access, and documented exceptions (e.g. `vector::at`).
6. **Invariants** — counts, indices, and encodings stay consistent after operations.

Skip trivial compile-only checks unless they guard a real regression.

## Assertions

Prefer `EXPECT_*` for most checks; use `ASSERT_*` only when later assertions depend on the result.

| Assertion | Use for |
|-----------|---------|
| `EXPECT_EQ(a, b)` | Exact equality (integers, enums, pointers) |
| `EXPECT_NE(a, b)` | Inequality |
| `EXPECT_DOUBLE_EQ(a, b)` | Floating-point comparisons |
| `EXPECT_THROW(stmt, exc)` | Expected exceptions |
| `ASSERT_EQ(a, b)` | Fatal check; stop test on failure |

Use `EXPECT_DOUBLE_EQ` for floating-point values, not `EXPECT_EQ`.

## Correct behavior over current behavior

Tests are the specification. Write assertions for what the code **should** do — from public API, README, opcode tables, naming, and reasonable domain expectations — not what the implementation currently returns.

**When tests fail after writing them:**

- Treat failures as potential bugs in production code, not bad tests.
- Report which tests failed and why; do **not** change expected values, remove assertions, or rewrite tests to make them pass.
- Do **not** "fix" failing tests by aligning expectations with observed (incorrect) output.
- Only change a test if the user explicitly corrects the intended behavior or the specification was wrong.

**Do not:**

- Run the code, observe wrong output, and encode that output as the expected value.
- Delete or skip failing tests to get a green build.
- Add comments like "implementation currently returns X" and assert X.

**Do:**

- Derive expected results independently (manual trace, spec, API contract).
- Leave failing tests in place when they expose incorrect implementation.
- Tell the user which tests fail and what the correct behavior should be.

## Test design principles

- Test **observable behavior** through public APIs; avoid testing private helpers directly.
- One logical behavior per test; multiple `EXPECT_*` calls in one test are fine when they verify the same action.
- Use Arrange–Act–Assert structure without labeling it in comments.
- Replace placeholder or broken tests only when the user asks or that file is the explicit target of the request.
- Do not add tests that only assert compile-time or trivial truths unless they guard a regression.

## Running tests

```bash
cmake --build build
ctest --test-dir build --output-on-failure
```

Run a single test:

```bash
./build/immutant_test --gtest_filter=ComponentTest.DescribesOneBehavior
```

## Checklist before finishing

- [ ] Scope matches the user's request; no unrequested tests or file churn
- [ ] Read the component under test (and derived broader coverage only if scope was open-ended)
- [ ] Test file follows naming conventions
- [ ] Production sources under test are linked in `CMakeLists.txt`
- [ ] `target_include_directories(... PRIVATE src)` is set when tests include `src/` headers
- [ ] Tests compile and run locally
- [ ] Assertions reflect specified correct behavior, not observed implementation quirks
- [ ] Failing tests were reported, not rewritten to pass
- [ ] New test file registered in `add_executable` if created
