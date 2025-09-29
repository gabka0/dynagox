package dynagox.experiments.hashmap;

import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

public class MultiHashMapTest {
    private static final double DELTA = 0.000001;

    @Test
    @DisplayName("MultiHashMap: should return null on get for non-existent key")
    void test1() {
        PrimaryHashIndex<MultiHashMapTestTriple> primaryHashIndex = new PrimaryHashIndex<>(
                new MultiHashMapTestTripleHash(), new MultiHashMapTestTripleEqual());
        SecondaryHashIndex<MultiHashMapTestTriple> secondaryHashIndex = new SecondaryHashIndex<>(
                new MultiHashMapTestTripleHash01(), new MultiHashMapTestTripleEqual01());
        MultiHashMap<MultiHashMapTestTriple> map = new MultiHashMap<>(primaryHashIndex, List.of(secondaryHashIndex));
        MultiHashMapTestTriple t = new MultiHashMapTestTriple();
        assertNull(map.get(t));
    }

    @Test
    @DisplayName("MultiHashMap: should add new key with annotation on update_add")
    void test2() {
        PrimaryHashIndex<MultiHashMapTestTriple> primaryHashIndex = new PrimaryHashIndex<>(
                new MultiHashMapTestTripleHash(), new MultiHashMapTestTripleEqual());
        SecondaryHashIndex<MultiHashMapTestTriple> secondaryHashIndex = new SecondaryHashIndex<>(
                new MultiHashMapTestTripleHash01(), new MultiHashMapTestTripleEqual01());
        MultiHashMap<MultiHashMapTestTriple> map = new MultiHashMap<>(primaryHashIndex, List.of(secondaryHashIndex));
        MultiHashMapTestTriple t = new MultiHashMapTestTriple();
        map.update_add(t, 3);
        MultiHashMapTestTriple r = map.get(t);
        assertNotNull(r);
        assertEquals(3, r.get_annotation(), DELTA);
    }

    @Test
    @DisplayName("MultiHashMap: should increase annotation on repeated update_add")
    void test3() {
        PrimaryHashIndex<MultiHashMapTestTriple> primaryHashIndex = new PrimaryHashIndex<>(
                new MultiHashMapTestTripleHash(), new MultiHashMapTestTripleEqual());
        SecondaryHashIndex<MultiHashMapTestTriple> secondaryHashIndex = new SecondaryHashIndex<>(
                new MultiHashMapTestTripleHash01(), new MultiHashMapTestTripleEqual01());
        MultiHashMap<MultiHashMapTestTriple> map = new MultiHashMap<>(primaryHashIndex, List.of(secondaryHashIndex));
        MultiHashMapTestTriple t = new MultiHashMapTestTriple();
        map.update_add(t, 3);
        map.update_add(t, 1);
        map.update_add(t, 4);
        MultiHashMapTestTriple r = map.get(t);
        assertNotNull(r);
        assertEquals(8, r.get_annotation(), DELTA);
    }

    @Test
    @DisplayName("MultiHashMap: should add new key with annotation and approximate on update_add_approx")
    void test4() {
        PrimaryHashIndex<MultiHashMapTestTriple> primaryHashIndex = new PrimaryHashIndex<>(
                new MultiHashMapTestTripleHash(), new MultiHashMapTestTripleEqual());
        SecondaryHashIndex<MultiHashMapTestTriple> secondaryHashIndex = new SecondaryHashIndex<>(
                new MultiHashMapTestTripleHash01(), new MultiHashMapTestTripleEqual01());
        MultiHashMap<MultiHashMapTestTriple> map = new MultiHashMap<>(primaryHashIndex, List.of(secondaryHashIndex));
        MultiHashMapTestTriple t = new MultiHashMapTestTriple();
        map.update_add_approx(t, 5, 1.1);
        MultiHashMapTestTriple r = map.get(t);
        assertNotNull(r);
        assertEquals(5, r.get_annotation(), DELTA);
        assertEquals(5.5, r.get_approximate(), DELTA);
    }

    @Test
    @DisplayName("MultiHashMap: should increase annotation and maintain approximate on repeated update_add_approx")
    void test5() {
        PrimaryHashIndex<MultiHashMapTestTriple> primaryHashIndex = new PrimaryHashIndex<>(
                new MultiHashMapTestTripleHash(), new MultiHashMapTestTripleEqual());
        SecondaryHashIndex<MultiHashMapTestTriple> secondaryHashIndex = new SecondaryHashIndex<>(
                new MultiHashMapTestTripleHash01(), new MultiHashMapTestTripleEqual01());
        MultiHashMap<MultiHashMapTestTriple> map = new MultiHashMap<>(primaryHashIndex, List.of(secondaryHashIndex));
        MultiHashMapTestTriple t = new MultiHashMapTestTriple();
        map.update_add_approx(t, 5, 1.1);
        map.update_add_approx(t, 8, 1.1);
        MultiHashMapTestTriple r1 = map.get(t);
        assertNotNull(r1);
        assertEquals(13, r1.get_annotation(), DELTA);
        assertEquals(14.3, r1.get_approximate(), DELTA);
        map.update_add_approx(t, 4, 1.1);
        MultiHashMapTestTriple r2 = map.get(t);
        assertNotNull(r2);
        assertEquals(17, r2.get_annotation(), DELTA);
        assertEquals(18.7, r2.get_approximate(), DELTA);
    }

    @Test
    @DisplayName("MultiHashMap: should return delta in approximate value on update_add_approx")
    void test6() {
        PrimaryHashIndex<MultiHashMapTestTriple> primaryHashIndex = new PrimaryHashIndex<>(
                new MultiHashMapTestTripleHash(), new MultiHashMapTestTripleEqual());
        SecondaryHashIndex<MultiHashMapTestTriple> secondaryHashIndex = new SecondaryHashIndex<>(
                new MultiHashMapTestTripleHash01(), new MultiHashMapTestTripleEqual01());
        MultiHashMap<MultiHashMapTestTriple> map = new MultiHashMap<>(primaryHashIndex, List.of(secondaryHashIndex));
        MultiHashMapTestTriple t = new MultiHashMapTestTriple();
        double r1 = map.update_add_approx(t, 17, 1.1);
        double e1 = 18.7 - 0;
        assertEquals(e1, r1, DELTA);
        double r2 = map.update_add_approx(t, 1, 1.1);
        double e2 = 18.7 - 18.7;
        assertEquals(e2, r2, DELTA);
        double r3 = map.update_add_approx(t, 1, 1.1);
        double e3 = 20.9 - 18.7;
        assertEquals(e3, r3, DELTA);
    }

    @Test
    @DisplayName("MultiHashMap: should activate an inactive key on update_activate")
    void test7() {
        PrimaryHashIndex<MultiHashMapTestTriple> primaryHashIndex = new PrimaryHashIndex<>(
                new MultiHashMapTestTripleHash(), new MultiHashMapTestTripleEqual());
        SecondaryHashIndex<MultiHashMapTestTriple> secondaryHashIndex = new SecondaryHashIndex<>(
                new MultiHashMapTestTripleHash01(), new MultiHashMapTestTripleEqual01());
        MultiHashMap<MultiHashMapTestTriple> map = new MultiHashMap<>(primaryHashIndex, List.of(secondaryHashIndex));
        MultiHashMapTestTriple t = new MultiHashMapTestTriple();
        map.update_add(t, 1);
        MultiHashMapTestTriple r1 = map.get(t);
        assertFalse(r1.is_active());
        map.update_activate(t);
        MultiHashMapTestTriple r2 = map.get(t);
        assertTrue(r2.is_active());
    }

    @Test
    @DisplayName("MultiHashMap: should return true when activating inactive key on update_activate")
    void test8() {
        PrimaryHashIndex<MultiHashMapTestTriple> primaryHashIndex = new PrimaryHashIndex<>(
                new MultiHashMapTestTripleHash(), new MultiHashMapTestTripleEqual());
        SecondaryHashIndex<MultiHashMapTestTriple> secondaryHashIndex = new SecondaryHashIndex<>(
                new MultiHashMapTestTripleHash01(), new MultiHashMapTestTripleEqual01());
        MultiHashMap<MultiHashMapTestTriple> map = new MultiHashMap<>(primaryHashIndex, List.of(secondaryHashIndex));
        MultiHashMapTestTriple t = new MultiHashMapTestTriple();
        map.update_add(t, 1);
        boolean r = map.update_activate(t);
        assertTrue(r);
    }

    @Test
    @DisplayName("MultiHashMap: should return false when key is already active on update_activate")
    void test9() {
        PrimaryHashIndex<MultiHashMapTestTriple> primaryHashIndex = new PrimaryHashIndex<>(
                new MultiHashMapTestTripleHash(), new MultiHashMapTestTripleEqual());
        SecondaryHashIndex<MultiHashMapTestTriple> secondaryHashIndex = new SecondaryHashIndex<>(
                new MultiHashMapTestTripleHash01(), new MultiHashMapTestTripleEqual01());
        MultiHashMap<MultiHashMapTestTriple> map = new MultiHashMap<>(primaryHashIndex, List.of(secondaryHashIndex));
        MultiHashMapTestTriple t = new MultiHashMapTestTriple();
        map.update_add(t, 1);
        map.update_activate(t);
        boolean r = map.update_activate(t);
        assertFalse(r);
    }

    @Test
    @DisplayName("MultiHashMap: should add new key with annotation and set active on update_add_and_activate")
    void test10() {
        PrimaryHashIndex<MultiHashMapTestTriple> primaryHashIndex = new PrimaryHashIndex<>(
                new MultiHashMapTestTripleHash(), new MultiHashMapTestTripleEqual());
        SecondaryHashIndex<MultiHashMapTestTriple> secondaryHashIndex = new SecondaryHashIndex<>(
                new MultiHashMapTestTripleHash01(), new MultiHashMapTestTripleEqual01());
        MultiHashMap<MultiHashMapTestTriple> map = new MultiHashMap<>(primaryHashIndex, List.of(secondaryHashIndex));
        MultiHashMapTestTriple t = new MultiHashMapTestTriple();
        map.update_add_and_activate(t, 1);
        MultiHashMapTestTriple r = map.get(t);
        assertNotNull(r);
        assertEquals(1, r.get_annotation(), DELTA);
        assertTrue(r.is_active());
    }

    @Test
    @DisplayName("MultiHashMap: should increase annotation on repeated update_add_and_activate")
    void test11() {
        PrimaryHashIndex<MultiHashMapTestTriple> primaryHashIndex = new PrimaryHashIndex<>(
                new MultiHashMapTestTripleHash(), new MultiHashMapTestTripleEqual());
        SecondaryHashIndex<MultiHashMapTestTriple> secondaryHashIndex = new SecondaryHashIndex<>(
                new MultiHashMapTestTripleHash01(), new MultiHashMapTestTripleEqual01());
        MultiHashMap<MultiHashMapTestTriple> map = new MultiHashMap<>(primaryHashIndex, List.of(secondaryHashIndex));
        MultiHashMapTestTriple t = new MultiHashMapTestTriple();
        map.update_add_and_activate(t, 3);
        map.update_add_and_activate(t, 1);
        map.update_add_and_activate(t, 4);
        MultiHashMapTestTriple r = map.get(t);
        assertNotNull(r);
        assertEquals(8, r.get_annotation(), DELTA);
        assertTrue(r.is_active());
    }

    @Test
    @DisplayName("MultiHashMap: should return true when activating inactive key on update_add_and_activate")
    void test12() {
        PrimaryHashIndex<MultiHashMapTestTriple> primaryHashIndex = new PrimaryHashIndex<>(
                new MultiHashMapTestTripleHash(), new MultiHashMapTestTripleEqual());
        SecondaryHashIndex<MultiHashMapTestTriple> secondaryHashIndex = new SecondaryHashIndex<>(
                new MultiHashMapTestTripleHash01(), new MultiHashMapTestTripleEqual01());
        MultiHashMap<MultiHashMapTestTriple> map = new MultiHashMap<>(primaryHashIndex, List.of(secondaryHashIndex));
        MultiHashMapTestTriple t = new MultiHashMapTestTriple();
        map.update_add(t, 1);
        boolean r = map.update_add_and_activate(t, 3);
        assertTrue(r);
    }

    @Test
    @DisplayName("MultiHashMap: should return false when key is already active on update_add_and_activate")
    void test13() {
        PrimaryHashIndex<MultiHashMapTestTriple> primaryHashIndex = new PrimaryHashIndex<>(
                new MultiHashMapTestTripleHash(), new MultiHashMapTestTripleEqual());
        SecondaryHashIndex<MultiHashMapTestTriple> secondaryHashIndex = new SecondaryHashIndex<>(
                new MultiHashMapTestTripleHash01(), new MultiHashMapTestTripleEqual01());
        MultiHashMap<MultiHashMapTestTriple> map = new MultiHashMap<>(primaryHashIndex, List.of(secondaryHashIndex));
        MultiHashMapTestTriple t = new MultiHashMapTestTriple();
        map.update_add(t, 1);
        map.update_activate(t);
        boolean r = map.update_add_and_activate(t, 3);
        assertFalse(r);
    }

    @Test
    @DisplayName("MultiHashMap: should decrease annotation on update_subtract")
    void test14() {
        PrimaryHashIndex<MultiHashMapTestTriple> primaryHashIndex = new PrimaryHashIndex<>(
                new MultiHashMapTestTripleHash(), new MultiHashMapTestTripleEqual());
        SecondaryHashIndex<MultiHashMapTestTriple> secondaryHashIndex = new SecondaryHashIndex<>(
                new MultiHashMapTestTripleHash01(), new MultiHashMapTestTripleEqual01());
        MultiHashMap<MultiHashMapTestTriple> map = new MultiHashMap<>(primaryHashIndex, List.of(secondaryHashIndex));
        MultiHashMapTestTriple t = new MultiHashMapTestTriple();
        map.update_add(t, 100);
        map.update_subtract(t, 3);
        MultiHashMapTestTriple r1 = map.get(t);
        assertNotNull(r1);
        assertEquals(97, r1.get_annotation(), DELTA);
        map.update_subtract(t, 27);
        MultiHashMapTestTriple r2 = map.get(t);
        assertNotNull(r2);
        assertEquals(70, r2.get_annotation(), DELTA);
    }

    @Test
    @DisplayName("MultiHashMap: should remove key when annotation reaches zero on update_subtract")
    void test15() {
        PrimaryHashIndex<MultiHashMapTestTriple> primaryHashIndex = new PrimaryHashIndex<>(
                new MultiHashMapTestTripleHash(), new MultiHashMapTestTripleEqual());
        SecondaryHashIndex<MultiHashMapTestTriple> secondaryHashIndex = new SecondaryHashIndex<>(
                new MultiHashMapTestTripleHash01(), new MultiHashMapTestTripleEqual01());
        MultiHashMap<MultiHashMapTestTriple> map = new MultiHashMap<>(primaryHashIndex, List.of(secondaryHashIndex));
        MultiHashMapTestTriple t = new MultiHashMapTestTriple();
        map.update_add(t, 100);
        map.update_subtract(t, 3);
        map.update_subtract(t, 27);
        map.update_subtract(t, 70);
        MultiHashMapTestTriple r = map.get(t);
        assertNull(r);
    }

    @Test
    @DisplayName("MultiHashMap: should return true only when annotation is reduced to zero on update_subtract")
    void test16() {
        PrimaryHashIndex<MultiHashMapTestTriple> primaryHashIndex = new PrimaryHashIndex<>(
                new MultiHashMapTestTripleHash(), new MultiHashMapTestTripleEqual());
        SecondaryHashIndex<MultiHashMapTestTriple> secondaryHashIndex = new SecondaryHashIndex<>(
                new MultiHashMapTestTripleHash01(), new MultiHashMapTestTripleEqual01());
        MultiHashMap<MultiHashMapTestTriple> map = new MultiHashMap<>(primaryHashIndex, List.of(secondaryHashIndex));
        MultiHashMapTestTriple t = new MultiHashMapTestTriple();
        map.update_add(t, 100);
        boolean r1 = map.update_subtract(t, 3);
        assertFalse(r1);
        boolean r2 = map.update_subtract(t, 27);
        assertFalse(r2);
        boolean r3 = map.update_subtract(t, 70);
        assertTrue(r3);
    }

    @Test
    @DisplayName("MultiHashMap: should decrease annotation and maintain approximate on update_subtract_approx")
    void test17() {
        PrimaryHashIndex<MultiHashMapTestTriple> primaryHashIndex = new PrimaryHashIndex<>(
                new MultiHashMapTestTripleHash(), new MultiHashMapTestTripleEqual());
        SecondaryHashIndex<MultiHashMapTestTriple> secondaryHashIndex = new SecondaryHashIndex<>(
                new MultiHashMapTestTripleHash01(), new MultiHashMapTestTripleEqual01());
        MultiHashMap<MultiHashMapTestTriple> map = new MultiHashMap<>(primaryHashIndex, List.of(secondaryHashIndex));
        MultiHashMapTestTriple t = new MultiHashMapTestTriple();
        map.update_add_approx(t, 90, 1.1);
        map.update_add_approx(t, 7, 1.1);
        map.update_subtract_approx(t, 5, 1.1);
        MultiHashMapTestTriple r1 = map.get(t);
        assertNotNull(r1);
        assertEquals(92, r1.get_annotation(), DELTA);
        assertEquals(99, r1.get_approximate(), DELTA);
        map.update_subtract_approx(t, 22, 1.1);
        MultiHashMapTestTriple r2 = map.get(t);
        assertNotNull(r2);
        assertEquals(70, r2.get_annotation(), DELTA);
        assertEquals(77, r2.get_approximate(), DELTA);
        map.update_subtract_approx(t, 27, 1.1);
        MultiHashMapTestTriple r3 = map.get(t);
        assertNotNull(r3);
        assertEquals(43, r3.get_annotation(), DELTA);
        assertEquals(47.3, r3.get_approximate(), DELTA);
    }

    @Test
    @DisplayName("MultiHashMap: should remove key when annotation reaches zero on update_subtract_approx")
    void test18() {
        PrimaryHashIndex<MultiHashMapTestTriple> primaryHashIndex = new PrimaryHashIndex<>(
                new MultiHashMapTestTripleHash(), new MultiHashMapTestTripleEqual());
        SecondaryHashIndex<MultiHashMapTestTriple> secondaryHashIndex = new SecondaryHashIndex<>(
                new MultiHashMapTestTripleHash01(), new MultiHashMapTestTripleEqual01());
        MultiHashMap<MultiHashMapTestTriple> map = new MultiHashMap<>(primaryHashIndex, List.of(secondaryHashIndex));
        MultiHashMapTestTriple t = new MultiHashMapTestTriple();
        map.update_add_approx(t, 100, 1.1);
        map.update_subtract_approx(t, 3, 1.1);
        map.update_subtract_approx(t, 27, 1.1);
        map.update_subtract_approx(t, 70, 1.1);
        MultiHashMapTestTriple r = map.get(t);
        assertNull(r);
    }

    @Test
    @DisplayName("MultiHashMap: should return delta in approximate value on update_subtract_approx")
    void test19() {
        PrimaryHashIndex<MultiHashMapTestTriple> primaryHashIndex = new PrimaryHashIndex<>(
                new MultiHashMapTestTripleHash(), new MultiHashMapTestTripleEqual());
        SecondaryHashIndex<MultiHashMapTestTriple> secondaryHashIndex = new SecondaryHashIndex<>(
                new MultiHashMapTestTripleHash01(), new MultiHashMapTestTripleEqual01());
        MultiHashMap<MultiHashMapTestTriple> map = new MultiHashMap<>(primaryHashIndex, List.of(secondaryHashIndex));
        MultiHashMapTestTriple t = new MultiHashMapTestTriple();
        map.update_add_approx(t, 90, 1.1);
        map.update_add_approx(t, 7, 1.1);
        double r1 = map.update_subtract_approx(t, 5, 1.1);
        double e1 = 99 - 99;
        assertEquals(e1, r1, DELTA);
        double r2 = map.update_subtract_approx(t, 22, 1.1);
        double e2 = 99 - 77;
        assertEquals(e2, r2, DELTA);
        double r3 = map.update_subtract_approx(t, 27, 1.1);
        double e3 = 77 - 47.3;
        assertEquals(e3, r3, DELTA);
    }

    @Test
    @DisplayName("MultiHashMap: should deactivate an active key on update_deactivate")
    void test20() {
        PrimaryHashIndex<MultiHashMapTestTriple> primaryHashIndex = new PrimaryHashIndex<>(
                new MultiHashMapTestTripleHash(), new MultiHashMapTestTripleEqual());
        SecondaryHashIndex<MultiHashMapTestTriple> secondaryHashIndex = new SecondaryHashIndex<>(
                new MultiHashMapTestTripleHash01(), new MultiHashMapTestTripleEqual01());
        MultiHashMap<MultiHashMapTestTriple> map = new MultiHashMap<>(primaryHashIndex, List.of(secondaryHashIndex));
        MultiHashMapTestTriple t = new MultiHashMapTestTriple();
        map.update_add_and_activate(t, 1);
        MultiHashMapTestTriple r1 = map.get(t);
        assertTrue(r1.is_active());
        map.update_deactivate(t);
        MultiHashMapTestTriple r2 = map.get(t);
        assertFalse(r2.is_active());
    }

    @Test
    @DisplayName("MultiHashMap: should return true when deactivating active key on update_deactivate")
    void test21() {
        PrimaryHashIndex<MultiHashMapTestTriple> primaryHashIndex = new PrimaryHashIndex<>(
                new MultiHashMapTestTripleHash(), new MultiHashMapTestTripleEqual());
        SecondaryHashIndex<MultiHashMapTestTriple> secondaryHashIndex = new SecondaryHashIndex<>(
                new MultiHashMapTestTripleHash01(), new MultiHashMapTestTripleEqual01());
        MultiHashMap<MultiHashMapTestTriple> map = new MultiHashMap<>(primaryHashIndex, List.of(secondaryHashIndex));
        MultiHashMapTestTriple t = new MultiHashMapTestTriple();
        map.update_add_and_activate(t, 1);
        boolean r = map.update_deactivate(t);
        assertTrue(r);
    }

    @Test
    @DisplayName("MultiHashMap: should return false when key is already inactive on update_deactivate")
    void test22() {
        PrimaryHashIndex<MultiHashMapTestTriple> primaryHashIndex = new PrimaryHashIndex<>(
                new MultiHashMapTestTripleHash(), new MultiHashMapTestTripleEqual());
        SecondaryHashIndex<MultiHashMapTestTriple> secondaryHashIndex = new SecondaryHashIndex<>(
                new MultiHashMapTestTripleHash01(), new MultiHashMapTestTripleEqual01());
        MultiHashMap<MultiHashMapTestTriple> map = new MultiHashMap<>(primaryHashIndex, List.of(secondaryHashIndex));
        MultiHashMapTestTriple t = new MultiHashMapTestTriple();
        map.update_add(t, 1);
        map.update_deactivate(t);
        boolean r = map.update_deactivate(t);
        assertFalse(r);
    }

    @Test
    @DisplayName("MultiHashMap: should add new key with annotation on update_max")
    void test23() {
        PrimaryHashIndex<MultiHashMapTestTriple> primaryHashIndex = new PrimaryHashIndex<>(
                new MultiHashMapTestTripleHash(), new MultiHashMapTestTripleEqual());
        SecondaryHashIndex<MultiHashMapTestTriple> secondaryHashIndex = new SecondaryHashIndex<>(
                new MultiHashMapTestTripleHash01(), new MultiHashMapTestTripleEqual01());
        MultiHashMap<MultiHashMapTestTriple> map = new MultiHashMap<>(primaryHashIndex, List.of(secondaryHashIndex));
        MultiHashMapTestTriple t = new MultiHashMapTestTriple();
        map.update_max(t, 3);
        MultiHashMapTestTriple r = map.get(t);
        assertNotNull(r);
        assertEquals(3, r.get_annotation(), DELTA);
    }

    @Test
    @DisplayName("MultiHashMap: should retain maximum annotation on repeated update_max")
    void test24() {
        PrimaryHashIndex<MultiHashMapTestTriple> primaryHashIndex = new PrimaryHashIndex<>(
                new MultiHashMapTestTripleHash(), new MultiHashMapTestTripleEqual());
        SecondaryHashIndex<MultiHashMapTestTriple> secondaryHashIndex = new SecondaryHashIndex<>(
                new MultiHashMapTestTripleHash01(), new MultiHashMapTestTripleEqual01());
        MultiHashMap<MultiHashMapTestTriple> map = new MultiHashMap<>(primaryHashIndex, List.of(secondaryHashIndex));
        MultiHashMapTestTriple t = new MultiHashMapTestTriple();
        map.update_max(t, 3);
        MultiHashMapTestTriple r1 = map.get(t);
        assertNotNull(r1);
        assertEquals(3, r1.get_annotation(), DELTA);
        map.update_max(t, 1);
        MultiHashMapTestTriple r2 = map.get(t);
        assertNotNull(r2);
        assertEquals(3, r2.get_annotation(), DELTA);
        map.update_max(t, 4);
        MultiHashMapTestTriple r3 = map.get(t);
        assertNotNull(r3);
        assertEquals(4, r3.get_annotation(), DELTA);
    }

    @Test
    @DisplayName("MultiHashMap: should return true on update_max only if annotation was updated")
    void test25() {
        PrimaryHashIndex<MultiHashMapTestTriple> primaryHashIndex = new PrimaryHashIndex<>(
                new MultiHashMapTestTripleHash(), new MultiHashMapTestTripleEqual());
        SecondaryHashIndex<MultiHashMapTestTriple> secondaryHashIndex = new SecondaryHashIndex<>(
                new MultiHashMapTestTripleHash01(), new MultiHashMapTestTripleEqual01());
        MultiHashMap<MultiHashMapTestTriple> map = new MultiHashMap<>(primaryHashIndex, List.of(secondaryHashIndex));
        MultiHashMapTestTriple t = new MultiHashMapTestTriple();
        boolean r1 = map.update_max(t, 3);
        assertTrue(r1);
        boolean r2 = map.update_max(t, 1);
        assertFalse(r2);
        boolean r3 = map.update_max(t, 4);
        assertTrue(r3);
    }

    @Test
    @DisplayName("MultiHashMap: should add new key with annotation on update_max_approx")
    void test26() {
        PrimaryHashIndex<MultiHashMapTestTriple> primaryHashIndex = new PrimaryHashIndex<>(
                new MultiHashMapTestTripleHash(), new MultiHashMapTestTripleEqual());
        SecondaryHashIndex<MultiHashMapTestTriple> secondaryHashIndex = new SecondaryHashIndex<>(
                new MultiHashMapTestTripleHash01(), new MultiHashMapTestTripleEqual01());
        MultiHashMap<MultiHashMapTestTriple> map = new MultiHashMap<>(primaryHashIndex, List.of(secondaryHashIndex));
        MultiHashMapTestTriple t = new MultiHashMapTestTriple();
        map.update_max_approx(t, 3, 1.1);
        MultiHashMapTestTriple r = map.get(t);
        assertNotNull(r);
        assertEquals(3, r.get_annotation(), DELTA);
        assertEquals(3.3, r.get_approximate(), DELTA);
    }

    @Test
    @DisplayName("MultiHashMap: should retain maximum annotation on repeated update_max_approx")
    void test27() {
        PrimaryHashIndex<MultiHashMapTestTriple> primaryHashIndex = new PrimaryHashIndex<>(
                new MultiHashMapTestTripleHash(), new MultiHashMapTestTripleEqual());
        SecondaryHashIndex<MultiHashMapTestTriple> secondaryHashIndex = new SecondaryHashIndex<>(
                new MultiHashMapTestTripleHash01(), new MultiHashMapTestTripleEqual01());
        MultiHashMap<MultiHashMapTestTriple> map = new MultiHashMap<>(primaryHashIndex, List.of(secondaryHashIndex));
        MultiHashMapTestTriple t = new MultiHashMapTestTriple();
        map.update_max_approx(t, 3, 1.1);
        MultiHashMapTestTriple r1 = map.get(t);
        assertNotNull(r1);
        assertEquals(3, r1.get_annotation(), DELTA);
        assertEquals(3.3, r1.get_approximate(), DELTA);
        map.update_max_approx(t, 1, 1.1);
        MultiHashMapTestTriple r2 = map.get(t);
        assertNotNull(r2);
        assertEquals(3, r2.get_annotation(), DELTA);
        assertEquals(3.3, r2.get_approximate(), DELTA);
        map.update_max_approx(t, 4, 1.1);
        MultiHashMapTestTriple r3 = map.get(t);
        assertNotNull(r3);
        assertEquals(4, r3.get_annotation(), DELTA);
        assertEquals(4.4, r3.get_approximate(), DELTA);
    }

    @Test
    @DisplayName("MultiHashMap: should return delta in approximate value on update_max_approx")
    void test28() {
        PrimaryHashIndex<MultiHashMapTestTriple> primaryHashIndex = new PrimaryHashIndex<>(
                new MultiHashMapTestTripleHash(), new MultiHashMapTestTripleEqual());
        SecondaryHashIndex<MultiHashMapTestTriple> secondaryHashIndex = new SecondaryHashIndex<>(
                new MultiHashMapTestTripleHash01(), new MultiHashMapTestTripleEqual01());
        MultiHashMap<MultiHashMapTestTriple> map = new MultiHashMap<>(primaryHashIndex, List.of(secondaryHashIndex));
        MultiHashMapTestTriple t = new MultiHashMapTestTriple();
        double r1 = map.update_max_approx(t, 17, 1.1);
        double e1 = 18.7;
        assertEquals(e1, r1, DELTA);
        double r2 = map.update_max_approx(t, 16, 1.1);
        double e2 = 0;
        assertEquals(e2, r2, DELTA);
        double r3 = map.update_max_approx(t, 18, 1.1);
        double e3 = 0;
        assertEquals(e3, r3, DELTA);
        double r4 = map.update_max_approx(t, 19, 1.1);
        double e4 = 20.9;
        assertEquals(e4, r4, DELTA);
    }
}

class MultiHashMapTestTriple implements MultiHashMapKey {
    int f0;
    int f1;
    int f2;
    private double annotation;
    private double approximate;
    private boolean active;
    private MultiHashMapTestTriple previous;
    private MultiHashMapTestTriple next;

    public MultiHashMapTestTriple() {
        this.f0 = 0;
        this.f1 = 0;
        this.f2 = 0;
        this.annotation = 0;
        this.approximate = 0;
        this.active = false;
        this.previous = null;
        this.next = null;
    }

    @Override
    public boolean equals(Object other) {
        if (this == other) return true;
        if (!(other instanceof MultiHashMapTestTriple)) return false;
        MultiHashMapTestTriple that = (MultiHashMapTestTriple) other;
        return f0 == that.f0 && f1 == that.f1 && f2 == that.f2;
    }

    @Override
    public double get_annotation() {
        return annotation;
    }

    @Override
    public void set_annotation(double annotation) {
        this.annotation = annotation;
    }

    @Override
    public double get_approximate() {
        return approximate;
    }

    @Override
    public void set_approximate(double approximate) {
        this.approximate = approximate;
    }

    @Override
    public boolean is_active() {
        return active;
    }

    @Override
    public void set_active(boolean active) {
        this.active = active;
    }

    @Override
    public MultiHashMapTestTriple get_next() {
        return next;
    }

    @Override
    public void set_next(MultiHashMapKey next) {
        this.next = (MultiHashMapTestTriple) next;
    }

    @Override
    public MultiHashMapTestTriple get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (MultiHashMapTestTriple) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        MultiHashMapTestTriple result = new MultiHashMapTestTriple();
        result.f0 = f0;
        result.f1 = f1;
        result.f2 = f2;
        result.annotation = annotation;
        result.approximate = approximate;
        return result;
    }
}

class MultiHashMapTestTripleHash implements HashFunction<MultiHashMapTestTriple> {
    @Override
    public int hash(MultiHashMapTestTriple triple) {
        return triple.f0 & triple.f1 & triple.f2;
    }
}

class MultiHashMapTestTripleEqual implements EqualFunction<MultiHashMapTestTriple> {
    @Override
    public boolean equal(MultiHashMapTestTriple lhs, MultiHashMapTestTriple rhs) {
        return lhs.equals(rhs);
    }
}

class MultiHashMapTestTripleHash01 implements HashFunction<MultiHashMapTestTriple> {
    @Override
    public int hash(MultiHashMapTestTriple triple) {
        return triple.f0 & triple.f1;
    }
}

class MultiHashMapTestTripleEqual01 implements EqualFunction<MultiHashMapTestTriple> {
    @Override
    public boolean equal(MultiHashMapTestTriple lhs, MultiHashMapTestTriple rhs) {
        return lhs.f0 == rhs.f0 && lhs.f1 == rhs.f1;
    }
}