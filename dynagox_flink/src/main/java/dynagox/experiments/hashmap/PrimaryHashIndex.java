package dynagox.experiments.hashmap;

import java.io.Serializable;

public class PrimaryHashIndex<T> implements Serializable {
    private static final int DEFAULT_CAPACITY = 16384;
    private static final double DEFAULT_LOAD_FACTOR = 0.7;

    private final HashFunction<T> keyHash;
    private final EqualFunction<T> keyEqual;
    private PrimaryHashIndexNode<T>[] buckets;
    private int size;
    private int capacity;
    private int mask;
    private int threshold;
    private final double loadFactor;

    public PrimaryHashIndex(HashFunction<T> keyHash, EqualFunction<T> keyEqual) {
        this(keyHash, keyEqual, DEFAULT_CAPACITY, DEFAULT_LOAD_FACTOR);
    }

    public PrimaryHashIndex(HashFunction<T> keyHash, EqualFunction<T> keyEqual, int capacity, double loadFactor) {
        this.keyHash = keyHash;
        this.keyEqual = keyEqual;
        this.capacity = 0;
        this.loadFactor = loadFactor;
        this.size = 0;
        this.mask = 0;
        this.threshold = 0;
        this.buckets = null;

        resize(capacity);
    }

    private void resize(int expected) {
        PrimaryHashIndexNode<T>[] b = buckets;
        long c = capacity;
        capacity = expected;
        mask = capacity - 1;
        threshold = (int) (capacity * loadFactor);

        @SuppressWarnings("unchecked")
        PrimaryHashIndexNode<T>[] newBuckets = new PrimaryHashIndexNode[capacity];
        buckets = newBuckets;

        for (int i = 0; i < c; i++) {
            PrimaryHashIndexNode<T> node1 = b[i];
            if (node1 != null && node1.object != null) {
                PrimaryHashIndexNode<T> node2 = buckets[(node1.hashValue & mask)];
                if (node2 != null && node2.object != null) {
                    // TODO: no need to new PrimaryHashIndexNode?
                    PrimaryHashIndexNode<T> newNode = new PrimaryHashIndexNode<>(node1.object, node1.hashValue);
                    newNode.next = node2.next;
                    node2.next = newNode;
                } else {
                    node2 = new PrimaryHashIndexNode<>(node1.object, node1.hashValue);
                    buckets[(node1.hashValue & mask)] = node2;
                }

                node1 = node1.next;
                while (node1 != null) {
                    PrimaryHashIndexNode<T> node3 = buckets[(node1.hashValue & mask)];
                    PrimaryHashIndexNode<T> p = node1;
                    node1 = node1.next;

                    if (node3 != null && node3.object != null) {
                        p.next = node3.next;
                        node3.next = p;
                    } else {
                        node3 = new PrimaryHashIndexNode<>(p.object, p.hashValue);
                        buckets[(p.hashValue & mask)] = node3;
                    }
                }
            }
        }
    }

    public T get(T key, int hashValue) {
        PrimaryHashIndexNode<T> node = buckets[(hashValue & mask)];
        while (node != null) {
            if (node.object != null && node.hashValue == hashValue && keyEqual.equal(key, node.object)) {
                return node.object;
            }
            node = node.next;
        }
        return null;
    }

    public void put(T object, int hashValue) {
        if (size >= threshold) {
            resize(capacity * 2);
        }

        int index = (hashValue & mask);
        PrimaryHashIndexNode<T> node = buckets[index];
        if (node != null && node.object != null) {
            PrimaryHashIndexNode<T> newNode = new PrimaryHashIndexNode<>(object, hashValue);
            newNode.next = node.next;
            node.next = newNode;
        } else {
            buckets[index] = new PrimaryHashIndexNode<>(object, hashValue);
        }
        size++;
    }

    public void remove(T object, long hashValue) {
        int index = (int) (hashValue & mask);
        PrimaryHashIndexNode<T> current = buckets[index];
        PrimaryHashIndexNode<T> previous = null;

        while (current != null) {
            if (object == current.object) {
                if (previous == null) {
                    if (current.next != null) {
                        buckets[index] = current.next;
                    } else {
                        buckets[index] = null;
                    }
                } else {
                    previous.next = current.next;
                }
                size--;
                return;
            }
            previous = current;
            current = current.next;
        }
    }

    public int getSize() {
        return size;
    }

    public int getCapacity() {
        return capacity;
    }

    public int hash(T key) {
        return keyHash.hash(key);
    }
}
