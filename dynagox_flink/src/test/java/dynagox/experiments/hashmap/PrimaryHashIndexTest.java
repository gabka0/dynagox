package dynagox.experiments.hashmap;

import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

public class PrimaryHashIndexTest {
    @Test
    @DisplayName("PrimaryHashIndex: should have default capacity after initialization")
    void test1() {
        PrimaryHashIndex<PrimaryHashIndexTestTriple> index =
                new PrimaryHashIndex<>(new PrimaryHashIndexTestTripleHash(), new PrimaryHashIndexTestTripleEqual());
        assertEquals(16384, index.getCapacity());
    }

    @Test
    @DisplayName("PrimaryHashIndex: should insert elements on put")
    void test2() {
        PrimaryHashIndex<PrimaryHashIndexTestTriple> index =
                new PrimaryHashIndex<>(new PrimaryHashIndexTestTripleHash(), new PrimaryHashIndexTestTripleEqual());
        PrimaryHashIndexTestTriple t1 = new PrimaryHashIndexTestTriple(3, 2, 7);
        int h1 = index.hash(t1);
        index.put(t1, h1);

        PrimaryHashIndexTestTriple t2 = new PrimaryHashIndexTestTriple(1, 7, 8);
        int h2 = index.hash(t2);
        index.put(t2, h2);
    }

    @Test
    @DisplayName("PrimaryHashIndex: should retrieve matching element on get")
    void test3() {
        PrimaryHashIndex<PrimaryHashIndexTestTriple> index =
                new PrimaryHashIndex<>(new PrimaryHashIndexTestTripleHash(), new PrimaryHashIndexTestTripleEqual());
        PrimaryHashIndexTestTriple t1 = new PrimaryHashIndexTestTriple(3, 2, 7);
        int h1 = index.hash(t1);
        index.put(t1, h1);
        PrimaryHashIndexTestTriple r1 = index.get(t1, h1);
        assertTrue(new PrimaryHashIndexTestTripleEqual().equal(t1, r1));

        PrimaryHashIndexTestTriple t2 = new PrimaryHashIndexTestTriple(1, 7, 8);
        int h2 = index.hash(t2);
        index.put(t2, h2);
        PrimaryHashIndexTestTriple r2 = index.get(t2, h2);
        assertTrue(new PrimaryHashIndexTestTripleEqual().equal(t2, r2));
    }

    @Test
    @DisplayName("PrimaryHashIndex: should remove element on remove")
    void test4() {
        PrimaryHashIndex<PrimaryHashIndexTestTriple> index =
                new PrimaryHashIndex<>(new PrimaryHashIndexTestTripleHash(), new PrimaryHashIndexTestTripleEqual());
        PrimaryHashIndexTestTriple t1 = new PrimaryHashIndexTestTriple(3, 2, 7);
        int h1 = index.hash(t1);
        index.put(t1, h1);
        PrimaryHashIndexTestTriple t2 = new PrimaryHashIndexTestTriple(1, 7, 8);
        int h2 = index.hash(t2);
        index.put(t2, h2);

        index.remove(t1, h1);
        PrimaryHashIndexTestTriple r1 = index.get(t1, h1);
        assertNull(r1);

        index.remove(t2, h2);
        PrimaryHashIndexTestTriple r2 = index.get(t2, h2);
        assertNull(r2);
    }

    @Test
    @DisplayName("PrimaryHashIndex: should resize when threshold is exceeded")
    void test5() {
        int capacity = 32;
        double loadFactor = 0.75;
        int threshold = (int)(capacity * loadFactor);

        PrimaryHashIndex<PrimaryHashIndexTestTriple> index =
                new PrimaryHashIndex<>(new PrimaryHashIndexTestTripleHash(), new PrimaryHashIndexTestTripleEqual(),
                        capacity, loadFactor);

        for (int i = 0; i < threshold; i++) {
            PrimaryHashIndexTestTriple t = new PrimaryHashIndexTestTriple(i, i + 1, i + 2);
            int h = index.hash(t);
            index.put(t, h);
            assertEquals(capacity, index.getCapacity());
        }

        PrimaryHashIndexTestTriple t = new PrimaryHashIndexTestTriple(threshold, threshold + 1, threshold + 2);
        int h = index.hash(t);
        index.put(t, h);
        assertEquals(capacity * 2, index.getCapacity());
    }

    @Test
    @DisplayName("PrimaryHashIndex: should track the number of keys correctly")
    void test6() {
        PrimaryHashIndex<PrimaryHashIndexTestTriple> index =
                new PrimaryHashIndex<>(new PrimaryHashIndexTestTripleHash(), new PrimaryHashIndexTestTripleEqual());
        PrimaryHashIndexTestTriple t1 = new PrimaryHashIndexTestTriple(3, 2, 7);
        int h1 = index.hash(t1);
        index.put(t1, h1);
        assertEquals(1, index.getSize());

        PrimaryHashIndexTestTriple t2 = new PrimaryHashIndexTestTriple(1, 7, 8);
        int h2 = index.hash(t2);
        index.put(t2, h2);
        assertEquals(2, index.getSize());

        PrimaryHashIndexTestTriple t3 = new PrimaryHashIndexTestTriple(3, 2, 3);
        int h3 = index.hash(t3);
        index.put(t3, h3);
        assertEquals(3, index.getSize());

        index.remove(t2, h2);
        assertEquals(2, index.getSize());
    }
}

class PrimaryHashIndexTestTriple implements MultiHashMapKey {
    int f0;
    int f1;
    int f2;
    private double annotation;
    private double approximate;
    private boolean active;
    private PrimaryHashIndexTestTriple next;
    private PrimaryHashIndexTestTriple previous;

    public PrimaryHashIndexTestTriple(int f0, int f1, int f2) {
        this.f0 = f0;
        this.f1 = f1;
        this.f2 = f2;
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
    public PrimaryHashIndexTestTriple get_next() {
        return next;
    }

    @Override
    public void set_next(MultiHashMapKey next) {
        this.next = (PrimaryHashIndexTestTriple) next;
    }

    @Override
    public PrimaryHashIndexTestTriple get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (PrimaryHashIndexTestTriple) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        PrimaryHashIndexTestTriple result = new PrimaryHashIndexTestTriple(f0, f1, f2);
        result.annotation = annotation;
        result.approximate = approximate;
        return result;
    }
}

class PrimaryHashIndexTestTripleHash implements HashFunction<PrimaryHashIndexTestTriple> {
    @Override
    public int hash(PrimaryHashIndexTestTriple triple) {
        return triple.f0 & triple.f1 & triple.f2;
    }
}

class PrimaryHashIndexTestTripleEqual implements EqualFunction<PrimaryHashIndexTestTriple> {
    @Override
    public boolean equal(PrimaryHashIndexTestTriple lhs, PrimaryHashIndexTestTriple rhs) {
        return lhs.f0 == rhs.f0 && lhs.f1 == rhs.f1 && lhs.f2 == rhs.f2;
    }
}