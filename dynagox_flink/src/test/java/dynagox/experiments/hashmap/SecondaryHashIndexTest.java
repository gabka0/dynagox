package dynagox.experiments.hashmap;

import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import java.util.ArrayList;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

public class SecondaryHashIndexTest {
    @Test
    @DisplayName("SecondaryHashIndex: should have default capacity after initialization")
    void test1() {
        SecondaryHashIndex<SecondaryHashIndexTestTriple> index =
                new SecondaryHashIndex<>(new SecondaryHashIndexTestTripleHash01(), new SecondaryHashIndexTestTripleEqual01());
        assertEquals(16384, index.getCapacity());
    }

    @Test
    @DisplayName("SecondaryHashIndex: should insert elements on put")
    void test2() {
        SecondaryHashIndex<SecondaryHashIndexTestTriple> index =
                new SecondaryHashIndex<>(new SecondaryHashIndexTestTripleHash01(), new SecondaryHashIndexTestTripleEqual01());
        SecondaryHashIndexTestTriple t1 = new SecondaryHashIndexTestTriple(0, 1, 1);
        index.put(t1);
        SecondaryHashIndexTestTriple t2 = new SecondaryHashIndexTestTriple(0, 2, 2);
        index.put(t2);
        SecondaryHashIndexTestTriple t3 = new SecondaryHashIndexTestTriple(0, 3, 3);
        index.put(t3);
        SecondaryHashIndexTestTriple t4 = new SecondaryHashIndexTestTriple(0, 2, 4);
        index.put(t4);
        SecondaryHashIndexTestTriple t5 = new SecondaryHashIndexTestTriple(0, 2, 5);
        index.put(t5);
    }

    @Test
    @DisplayName("SecondaryHashIndex: should retrieve matching elements on find")
    void test3() {
        SecondaryHashIndex<SecondaryHashIndexTestTriple> index =
                new SecondaryHashIndex<>(new SecondaryHashIndexTestTripleHash01(), new SecondaryHashIndexTestTripleEqual01());
        SecondaryHashIndexTestTriple t1 = new SecondaryHashIndexTestTriple(0, 1, 1);
        index.put(t1);
        SecondaryHashIndexTestTriple t2 = new SecondaryHashIndexTestTriple(0, 2, 2);
        index.put(t2);
        SecondaryHashIndexTestTriple t3 = new SecondaryHashIndexTestTriple(0, 3, 3);
        index.put(t3);
        SecondaryHashIndexTestTriple t4 = new SecondaryHashIndexTestTriple(0, 2, 4);
        index.put(t4);
        SecondaryHashIndexTestTriple t5 = new SecondaryHashIndexTestTriple(0, 2, 5);
        index.put(t5);

        List<SecondaryHashIndexTestTriple> actual = collect(index.find(new SecondaryHashIndexTestTriple(0, 2, 0)));
        assertEquals(3, actual.size());
        assertTrue(actual.stream().anyMatch(t -> t == t2));
        assertTrue(actual.stream().anyMatch(t -> t == t4));
        assertTrue(actual.stream().anyMatch(t -> t == t5));
    }

    @Test
    @DisplayName("SecondaryHashIndex: should remove element on remove")
    void test4() {
        SecondaryHashIndex<SecondaryHashIndexTestTriple> index =
                new SecondaryHashIndex<>(new SecondaryHashIndexTestTripleHash01(), new SecondaryHashIndexTestTripleEqual01());
        SecondaryHashIndexTestTriple t1 = new SecondaryHashIndexTestTriple(0, 1, 1);
        index.put(t1);
        SecondaryHashIndexTestTriple t2 = new SecondaryHashIndexTestTriple(0, 2, 2);
        index.put(t2);
        SecondaryHashIndexTestTriple t3 = new SecondaryHashIndexTestTriple(0, 3, 3);
        index.put(t3);
        SecondaryHashIndexTestTriple t4 = new SecondaryHashIndexTestTriple(0, 2, 4);
        index.put(t4);
        SecondaryHashIndexTestTriple t5 = new SecondaryHashIndexTestTriple(0, 2, 5);
        index.put(t5);

        index.remove(t2);

        List<SecondaryHashIndexTestTriple> actual = collect(index.find(new SecondaryHashIndexTestTriple(0, 2, 0)));
        assertEquals(2, actual.size());
        assertTrue(actual.stream().anyMatch(t -> t == t4));
        assertTrue(actual.stream().anyMatch(t -> t == t5));
    }

    @Test
    @DisplayName("SecondaryHashIndex: should resize when threshold is exceeded")
    void test5() {
        int capacity = 32;
        double loadFactor = 0.75;
        int threshold = (int) (capacity * loadFactor);

        SecondaryHashIndex<SecondaryHashIndexTestTriple> index =
                new SecondaryHashIndex<>(new SecondaryHashIndexTestTripleHash01(), new SecondaryHashIndexTestTripleEqual01(),
                        capacity, loadFactor);

        for (int i = 0; i < threshold; i++) {
            SecondaryHashIndexTestTriple t = new SecondaryHashIndexTestTriple(1, i, i + 2);
            index.put(t);
            assertEquals(capacity, index.getCapacity());
        }

        SecondaryHashIndexTestTriple t1 = new SecondaryHashIndexTestTriple(1, 1, 2);
        index.put(t1);
        assertEquals(capacity, index.getCapacity());

        SecondaryHashIndexTestTriple t2 = new SecondaryHashIndexTestTriple(1, threshold, threshold + 2);
        index.put(t2);
        assertEquals(capacity * 2, index.getCapacity());
    }

    @Test
    @DisplayName("SecondaryHashIndex: should track the number of distinct keys correctly")
    void test6() {
        SecondaryHashIndex<SecondaryHashIndexTestTriple> index =
                new SecondaryHashIndex<>(new SecondaryHashIndexTestTripleHash01(), new SecondaryHashIndexTestTripleEqual01());
        SecondaryHashIndexTestTriple t1 = new SecondaryHashIndexTestTriple(3, 2, 7);
        index.put(t1);
        assertEquals(1, index.getSize());

        SecondaryHashIndexTestTriple t2 = new SecondaryHashIndexTestTriple(1, 7, 8);
        index.put(t2);
        assertEquals(2, index.getSize());

        SecondaryHashIndexTestTriple t3 = new SecondaryHashIndexTestTriple(3, 2, 3);
        index.put(t3);
        assertEquals(2, index.getSize());

        index.remove(t2);
        assertEquals(1, index.getSize());
    }

    private List<SecondaryHashIndexTestTriple> collect(LinkedListNode<SecondaryHashIndexTestTriple> node) {
        List<SecondaryHashIndexTestTriple> result = new ArrayList<>();
        while (node != null) {
            result.add(node.object);
            node = node.next;
        }
        return result;
    }
}

class SecondaryHashIndexTestTriple implements MultiHashMapKey {
    int f0;
    int f1;
    int f2;
    private double annotation;
    private double approximate;
    private boolean active;
    private SecondaryHashIndexTestTriple next;
    private SecondaryHashIndexTestTriple previous;

    public SecondaryHashIndexTestTriple(int f0, int f1, int f2) {
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
    public SecondaryHashIndexTestTriple get_next() {
        return next;
    }

    @Override
    public void set_next(MultiHashMapKey next) {
        this.next = (SecondaryHashIndexTestTriple) next;
    }

    @Override
    public SecondaryHashIndexTestTriple get_previous() {
        return previous;
    }

    @Override
    public void set_previous(MultiHashMapKey previous) {
        this.previous = (SecondaryHashIndexTestTriple) previous;
    }

    @Override
    public MultiHashMapKey copy() {
        SecondaryHashIndexTestTriple result = new SecondaryHashIndexTestTriple(f0, f1, f2);
        result.annotation = annotation;
        result.approximate = approximate;
        return result;
    }
}

class SecondaryHashIndexTestTripleHash01 implements HashFunction<SecondaryHashIndexTestTriple> {
    @Override
    public int hash(SecondaryHashIndexTestTriple triple) {
        return triple.f0 & triple.f1;
    }
}

class SecondaryHashIndexTestTripleEqual01 implements EqualFunction<SecondaryHashIndexTestTriple> {
    @Override
    public boolean equal(SecondaryHashIndexTestTriple lhs, SecondaryHashIndexTestTriple rhs) {
        return lhs.f0 == rhs.f0 && lhs.f1 == rhs.f1;
    }
}