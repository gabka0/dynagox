# How to Run the CROWN Test

## Prerequisites

1. **CMake 3.23+** installed and in PATH
2. **C++23 compatible compiler** (GCC 13+, Clang 16+, or MSVC 2022+)
3. **Build the project first**

## Build Steps

```powershell
# Navigate to implement directory
cd C:\Users\Admin\DynaGox\dynagox_cpp\implement

# Create build directory
mkdir build
cd build

# Configure CMake
cmake ..

# Build crown_exec
cmake --build . --target crown_exec

# The executable will be at: build/experiments/crown_exec.exe (Windows)
# Or: build/experiments/Debug/crown_exec.exe
```

## Test File

The test file `test_simple.txt` contains:
```
|1|2
|2|3
|3|4
|4|5
|5|6
*
```

This creates a simple path: **1 → 2 → 3 → 4 → 5 → 6**

## Run Command

```powershell
# From build directory
.\experiments\crown_exec.exe -q line5s -i ..\test_simple.txt -p

# Or with full path
.\experiments\crown_exec.exe -q line5s -i C:\Users\Admin\DynaGox\dynagox_cpp\implement\test_simple.txt -p
```

## Expected Output

```
Query: line5s
Algorithm: Crown
40.0000
Execution time: ...
Memory usage: ...
```

The result **40.0000** represents the count of 5-paths found.

## What the Algorithm Does (Summary)

For the input `1→2→3→4→5→6`:

1. **Processes each edge incrementally**
2. **Maintains path views** that track how many paths reach each node
3. **Detects complete 5-paths** when all segments connect
4. **Updates result** whenever a complete path is found

The algorithm found **40 different ways** to form a 5-path from the edges, considering all possible combinations through the 5 relations (R1, R2, R3, R4, R5).

## Detailed Trace

See `CROWN_ALGORITHM_TRACE.md` for a complete step-by-step trace of what happens when processing each edge.


