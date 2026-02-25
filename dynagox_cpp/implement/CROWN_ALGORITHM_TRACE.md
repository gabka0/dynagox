# CROWN Algorithm Step-by-Step Trace

## Test Input
```
|1|2   (Edge: 1 → 2)
|2|3   (Edge: 2 → 3)
|3|4   (Edge: 3 → 4)
|4|5   (Edge: 4 → 5)
|5|6   (Edge: 5 → 6)
*      (Milestone - print result)
```

## Query: line5s (5-path counting query)

The query counts paths of length 5: R1(x1,x2) ⋈ R2(x2,x3) ⋈ R3(x3,x4) ⋈ R4(x4,x5) ⋈ R5(x5,x6)

---

## Step 1: Process Edge |1|2

**Input:** src=1, dst=2

### insert_into_r1(1, 2):
- **Action:** Update path view pv_r1
  - `pv_r1[2] += 1` → pv_r1[2] = 1
- **Check:** Find all r2 tuples where r2.f0 = 2 (edges starting from node 2)
  - None found (r2 is empty)
- **Result:** No update

**State after:**
- r1: empty (r1 is not stored, only pv_r1)
- pv_r1: {2 → 1}
- r2: empty
- pv_r2: empty
- result: 0

### insert_into_r2(1, 2):
- **Action:** Store tuple in r2
  - `r2.update_add((1,2), 1)` → r2 contains {(1,2)}
- **Check:** Look up pv_r1[1] (check if node 1 has incoming paths)
  - pv_r1[1] = null (no paths ending at 1)
- **Result:** No path view update, no result update

**State after:**
- r2: {(1,2)}
- pv_r2: empty
- result: 0

### insert_into_r3(1, 2):
- **Action:** Store tuple in r3
  - `r3.update_add((1,2), 1)` → r3 contains {(1,2)}
- **Check:** Look up pv_r4[2]
  - pv_r4[2] = null
- **Result:** No update

**State after:**
- r3: {(1,2)}
- pv_r3: empty
- result: 0

### insert_into_r4(1, 2):
- **Action:** Store tuple in r4
  - `r4.update_add((1,2), 1)` → r4 contains {(1,2)}
- **Check:** Look up pv_r5[2]
  - pv_r5[2] = null
- **Result:** No update

**State after:**
- r4: {(1,2)}
- pv_r4: empty
- result: 0

### insert_into_r5(1, 2):
- **Action:** Update path view pv_r5
  - `pv_r5[1] += 2` → pv_r5[1] = 2
- **Check:** Find all r4 tuples where r4.f1 = 1 (edges ending at node 1)
  - None found
- **Result:** No update

**State after:**
- pv_r5: {1 → 2}
- result: 0

---

## Step 2: Process Edge |2|3

**Input:** src=2, dst=3

### insert_into_r1(2, 3):
- **Action:** `pv_r1[3] += 2` → pv_r1[3] = 2
- **Check:** Find r2 tuples where r2.f0 = 3
  - None found
- **Result:** No update

**State:**
- pv_r1: {2 → 1, 3 → 2}

### insert_into_r2(2, 3):
- **Action:** `r2.update_add((2,3), 1)` → r2 contains {(1,2), (2,3)}
- **Check:** Look up pv_r1[2]
  - pv_r1[2] = 1 ✓ (found!)
- **Propagate:** 
  - `pv_r2[3] += 1` → pv_r2[3] = 1
- **Check:** Look up pv_r3[3]
  - pv_r3[3] = null
- **Result:** No complete path yet

**State:**
- r2: {(1,2), (2,3)}
- pv_r2: {3 → 1}

### insert_into_r3(2, 3):
- **Action:** `r3.update_add((2,3), 1)` → r3 contains {(1,2), (2,3)}
- **Check:** Look up pv_r4[3]
  - pv_r4[3] = null
- **Result:** No update

### insert_into_r4(2, 3):
- **Action:** `r4.update_add((2,3), 1)` → r4 contains {(1,2), (2,3)}
- **Check:** Look up pv_r5[3]
  - pv_r5[3] = null
- **Result:** No update

### insert_into_r5(2, 3):
- **Action:** `pv_r5[2] += 3` → pv_r5[2] = 3
- **Check:** Find r4 tuples where r4.f1 = 2
  - Found: (1,2) and (2,3)
- **Propagate:**
  - `pv_r4[1] += 3` → pv_r4[1] = 3
  - `pv_r4[2] += 3` → pv_r4[2] = 3
- **Continue propagation:**
  - For pv_r4[1]: Find r3 tuples where r3.f1 = 1
    - Found: (1,2) → `pv_r3[1] += 3` → pv_r3[1] = 3
    - Check pv_r2[1]: null
  - For pv_r4[2]: Find r3 tuples where r3.f1 = 2
    - Found: (2,3) → `pv_r3[2] += 3` → pv_r3[2] = 3
    - Check pv_r2[2]: null

**State:**
- pv_r5: {1 → 2, 2 → 3}
- pv_r4: {1 → 3, 2 → 3}
- pv_r3: {1 → 3, 2 → 3}
- result: 0

---

## Step 3: Process Edge |3|4

**Input:** src=3, dst=4

### insert_into_r1(3, 4):
- **Action:** `pv_r1[4] += 3` → pv_r1[4] = 3
- **Check:** Find r2 tuples where r2.f0 = 4
  - None found
- **Result:** No update

### insert_into_r2(3, 4):
- **Action:** `r2.update_add((3,4), 1)` → r2 contains {(1,2), (2,3), (3,4)}
- **Check:** Look up pv_r1[3]
  - pv_r1[3] = 2 ✓
- **Propagate:**
  - `pv_r2[4] += 2` → pv_r2[4] = 2
- **Check:** Look up pv_r3[4]
  - pv_r3[4] = null
- **Result:** No complete path

**State:**
- pv_r2: {3 → 1, 4 → 2}

### insert_into_r3(3, 4):
- **Action:** `r3.update_add((3,4), 1)` → r3 contains {(1,2), (2,3), (3,4)}
- **Check:** Look up pv_r4[4]
  - pv_r4[4] = null
- **Result:** No update

### insert_into_r4(3, 4):
- **Action:** `r4.update_add((3,4), 1)` → r4 contains {(1,2), (2,3), (3,4)}
- **Check:** Look up pv_r5[4]
  - pv_r5[4] = null
- **Result:** No update

### insert_into_r5(3, 4):
- **Action:** `pv_r5[3] += 4` → pv_r5[3] = 4
- **Check:** Find r4 tuples where r4.f1 = 3
  - Found: (2,3) and (3,4)
- **Propagate:**
  - `pv_r4[2] += 4` → pv_r4[2] = 3 + 4 = 7
  - `pv_r4[3] += 4` → pv_r4[3] = 4
- **Continue propagation:**
  - For pv_r4[2]: Find r3 tuples where r3.f1 = 2
    - Found: (2,3) → `pv_r3[2] += 4` → pv_r3[2] = 3 + 4 = 7
    - Check pv_r2[2]: null
  - For pv_r4[3]: Find r3 tuples where r3.f1 = 3
    - Found: (3,4) → `pv_r3[3] += 4` → pv_r3[3] = 4
    - Check pv_r2[3]: pv_r2[3] = 1 ✓
    - **COMPLETE PATH FOUND!** result += (1 × 4) = 4

**State:**
- pv_r5: {1 → 2, 2 → 3, 3 → 4}
- pv_r4: {1 → 3, 2 → 7, 3 → 4}
- pv_r3: {1 → 3, 2 → 7, 3 → 4}
- pv_r2: {3 → 1, 4 → 2}
- **result: 4**

---

## Step 4: Process Edge |4|5

**Input:** src=4, dst=5

### insert_into_r1(4, 5):
- **Action:** `pv_r1[5] += 4` → pv_r1[5] = 4
- **Check:** Find r2 tuples where r2.f0 = 5
  - None found
- **Result:** No update

### insert_into_r2(4, 5):
- **Action:** `r2.update_add((4,5), 1)` → r2 contains {(1,2), (2,3), (3,4), (4,5)}
- **Check:** Look up pv_r1[4]
  - pv_r1[4] = 3 ✓
- **Propagate:**
  - `pv_r2[5] += 3` → pv_r2[5] = 3
- **Check:** Look up pv_r3[5]
  - pv_r3[5] = null
- **Result:** No complete path

**State:**
- pv_r2: {3 → 1, 4 → 2, 5 → 3}

### insert_into_r3(4, 5):
- **Action:** `r3.update_add((4,5), 1)` → r3 contains {(1,2), (2,3), (3,4), (4,5)}
- **Check:** Look up pv_r4[5]
  - pv_r4[5] = null
- **Result:** No update

### insert_into_r4(4, 5):
- **Action:** `r4.update_add((4,5), 1)` → r4 contains {(1,2), (2,3), (3,4), (4,5)}
- **Check:** Look up pv_r5[5]
  - pv_r5[5] = null
- **Result:** No update

### insert_into_r5(4, 5):
- **Action:** `pv_r5[4] += 5` → pv_r5[4] = 5
- **Check:** Find r4 tuples where r4.f1 = 4
  - Found: (3,4)
- **Propagate:**
  - `pv_r4[3] += 5` → pv_r4[3] = 4 + 5 = 9
- **Continue propagation:**
  - For pv_r4[3]: Find r3 tuples where r3.f1 = 3
    - Found: (3,4) → `pv_r3[3] += 5` → pv_r3[3] = 4 + 5 = 9
    - Check pv_r2[3]: pv_r2[3] = 1 ✓
    - **COMPLETE PATH FOUND!** result += (1 × 9) = 9
    - New result: 4 + 9 = 13

**State:**
- pv_r5: {1 → 2, 2 → 3, 3 → 4, 4 → 5}
- pv_r4: {1 → 3, 2 → 7, 3 → 9}
- pv_r3: {1 → 3, 2 → 7, 3 → 9}
- pv_r2: {3 → 1, 4 → 2, 5 → 3}
- **result: 13**

---

## Step 5: Process Edge |5|6

**Input:** src=5, dst=6

### insert_into_r1(5, 6):
- **Action:** `pv_r1[6] += 5` → pv_r1[6] = 5
- **Check:** Find r2 tuples where r2.f0 = 6
  - None found
- **Result:** No update

### insert_into_r2(5, 6):
- **Action:** `r2.update_add((5,6), 1)` → r2 contains {(1,2), (2,3), (3,4), (4,5), (5,6)}
- **Check:** Look up pv_r1[5]
  - pv_r1[5] = 4 ✓
- **Propagate:**
  - `pv_r2[6] += 4` → pv_r2[6] = 4
- **Check:** Look up pv_r3[6]
  - pv_r3[6] = null
- **Result:** No complete path

**State:**
- pv_r2: {3 → 1, 4 → 2, 5 → 3, 6 → 4}

### insert_into_r3(5, 6):
- **Action:** `r3.update_add((5,6), 1)` → r3 contains {(1,2), (2,3), (3,4), (4,5), (5,6)}
- **Check:** Look up pv_r4[6]
  - pv_r4[6] = null
- **Result:** No update

### insert_into_r4(5, 6):
- **Action:** `r4.update_add((5,6), 1)` → r4 contains {(1,2), (2,3), (3,4), (4,5), (5,6)}
- **Check:** Look up pv_r5[6]
  - pv_r5[6] = null
- **Result:** No update

### insert_into_r5(5, 6):
- **Action:** `pv_r5[5] += 6` → pv_r5[5] = 6
- **Check:** Find r4 tuples where r4.f1 = 5
  - Found: (4,5)
- **Propagate:**
  - `pv_r4[4] += 6` → pv_r4[4] = 6
- **Continue propagation:**
  - For pv_r4[4]: Find r3 tuples where r3.f1 = 4
    - Found: (3,4) and (4,5)
    - For (3,4): `pv_r3[3] += 6` → pv_r3[3] = 9 + 6 = 15
      - Check pv_r2[3]: pv_r2[3] = 1 ✓
      - **COMPLETE PATH FOUND!** result += (1 × 15) = 15
    - For (4,5): `pv_r3[4] += 6` → pv_r3[4] = 6
      - Check pv_r2[4]: pv_r2[4] = 2 ✓
      - **COMPLETE PATH FOUND!** result += (2 × 6) = 12
    - New result: 13 + 15 + 12 = 40

**State:**
- pv_r5: {1 → 2, 2 → 3, 3 → 4, 4 → 5, 5 → 6}
- pv_r4: {1 → 3, 2 → 7, 3 → 9, 4 → 6}
- pv_r3: {1 → 3, 2 → 7, 3 → 15, 4 → 6}
- pv_r2: {3 → 1, 4 → 2, 5 → 3, 6 → 4}
- **result: 40**

---

## Milestone: Print Result

**Final Result: 40.0000**

This counts all 5-paths in the graph:
- Path: 1→2→3→4→5→6 (counted multiple times through different relation combinations)

---

## Key Algorithm Insights

1. **Incremental Processing:** Each edge is processed immediately, updating all affected path views
2. **Path View Propagation:** Path views aggregate values from parent relations via semi-joins
3. **Complete Path Detection:** When all segments of a 5-path exist, the result is updated
4. **Efficiency:** Only active paths are maintained, avoiding full recomputation


