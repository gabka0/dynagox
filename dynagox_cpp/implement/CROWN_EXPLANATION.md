# CROWN Algorithm - Simple Explanation

## What CROWN Does

CROWN counts **5-paths** in a graph incrementally. A 5-path is a sequence of 5 edges:
```
x1 → x2 → x3 → x4 → x5 → x6
```

## The Query Structure

The query represents 5 relations joined together:
- **R1(x1, x2)**: First edge
- **R2(x2, x3)**: Second edge  
- **R3(x3, x4)**: Third edge
- **R4(x4, x5)**: Fourth edge
- **R5(x5, x6)**: Fifth edge

## How It Works (Simple View)

### 1. **Relation Tables (r1, r2, r3, r4, r5)**
   - Store actual edges: `(src, dst)`
   - Example: `r2` contains `{(1,2), (2,3), (3,4)}`

### 2. **Path Views (pv_r1, pv_r2, pv_r3, pv_r4, pv_r5)**
   - Store aggregated path values per node
   - `pv_r2[node]` = sum of all paths that can reach `node` through R1 and R2
   - Example: `pv_r2[3] = 1` means there's 1 path ending at node 3 via R1→R2

### 3. **The Algorithm Flow**

When a new edge `(src, dst)` arrives:

```
For each relation Ri:
  1. Store the edge in relation table ri
  2. Check parent relations (semi-join)
  3. Update path views (propagate values)
  4. Check if complete path exists → update result
```

## Example: Processing Edge (3, 4)

**Graph so far:** 1→2, 2→3

**Processing:** Add edge 3→4

### Step-by-Step:

1. **insert_into_r1(3, 4)**
   - Update: `pv_r1[4] += 3` (node 4 can be reached from source 3)
   - Check: Are there any r2 edges starting from node 4? No.

2. **insert_into_r2(3, 4)**
   - Store: `r2` now contains `(3,4)`
   - Check: Does `pv_r1[3]` exist? Yes! (value = 2, from previous edges)
   - Propagate: `pv_r2[4] += 2` (2 paths can reach node 4 via R1→R2)
   - Check: Does `pv_r3[4]` exist? No (no complete path yet)

3. **insert_into_r3(3, 4)**
   - Store: `r3` now contains `(3,4)`
   - Check: Does `pv_r4[4]` exist? No

4. **insert_into_r4(3, 4)**
   - Store: `r4` now contains `(3,4)`
   - Check: Does `pv_r5[4]` exist? No

5. **insert_into_r5(3, 4)**
   - Update: `pv_r5[3] += 4` (node 3 can reach destination with value 4)
   - Check: Are there r4 edges ending at node 3? Yes: `(2,3)`
   - Propagate: `pv_r4[2] += 4`
   - Continue: Check r3 edges ending at node 2? Yes: `(2,3)`
   - Propagate: `pv_r3[2] += 4`
   - Check: Does `pv_r2[2]` exist? No
   - **No complete path yet**

## When a Complete Path is Found

A **complete 5-path** exists when: all path views are connected!

Example: After processing several edges, we might have:
- `pv_r2[3] = 1` (1 path reaches node 3 via R1→R2)
- `pv_r3[3] = 4` (4 paths reach node 3 via R1→R2→R3→R4→R5)

When both exist, we found a complete path! 
- **Result += 1 × 4 = 4**

## Key Insight

The algorithm maintains **incremental aggregations**:
- Instead of recomputing all paths from scratch
- It maintains "how many paths reach each node" in path views
- When a new edge connects existing paths, it immediately updates the result

This is **much faster** than naive approaches that would:
1. Store all edges
2. For each query, enumerate all possible 5-paths
3. Count them

CROWN does it **incrementally** and **efficiently**!


