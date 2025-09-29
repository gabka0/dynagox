package dynagox.experiments.hashmap;

public class SecondaryHashIndex<T> implements AbstractSecondaryHashIndex<T> {
    private static final int DEFAULT_CAPACITY = 16384;
    private static final double DEFAULT_LOAD_FACTOR = 0.7;

    private final HashFunction<T> keyHash;
    private final EqualFunction<T> keyEqual;
    private SecondaryHashIndexNode<T>[] buckets;
    private int size;
    private int capacity;
    private int mask;
    private int threshold;
    private final double loadFactor;

    public SecondaryHashIndex(HashFunction<T> keyHash, EqualFunction<T> keyEqual) {
        this(keyHash, keyEqual, DEFAULT_CAPACITY, DEFAULT_LOAD_FACTOR);
    }

    public SecondaryHashIndex(HashFunction<T> keyHash, EqualFunction<T> keyEqual, int capacity, double loadFactor) {
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
        SecondaryHashIndexNode<T>[] b = buckets;
        long c = capacity;
        capacity = expected;
        mask = capacity - 1;
        threshold = (int) (capacity * loadFactor);

        @SuppressWarnings("unchecked")
        SecondaryHashIndexNode<T>[] newBuckets = new SecondaryHashIndexNode[(int) capacity];
        buckets = newBuckets;

        for (int i = 0; i < c; i++) {
            SecondaryHashIndexNode<T> node1 = b[i];
            if (node1 != null && node1.linkedListNode.object != null) {
                SecondaryHashIndexNode<T> node2 = buckets[(node1.hashValue & mask)];
                if (node2 != null && node2.linkedListNode.object != null) {
                    SecondaryHashIndexNode<T> newNode = new SecondaryHashIndexNode<>(node1.linkedListNode.object, node1.hashValue);
                    newNode.linkedListNode.next = node1.linkedListNode.next;
                    newNode.next = node2.next;
                    node2.next = newNode;
                } else {
                    node2 = new SecondaryHashIndexNode<>(node1.linkedListNode.object, node1.hashValue);
                    node2.linkedListNode.next = node1.linkedListNode.next;
                    buckets[(node1.hashValue & mask)] = node2;
                }

                node1 = node1.next;
                while (node1 != null) {
                    SecondaryHashIndexNode<T> node3 = buckets[(node1.hashValue & mask)];
                    SecondaryHashIndexNode<T> p = node1;
                    node1 = node1.next;

                    if (node3 != null && node3.linkedListNode.object != null) {
                        p.next = node3.next;
                        node3.next = p;
                    } else {
                        node3 = new SecondaryHashIndexNode<>(p.linkedListNode.object, p.hashValue);
                        node3.linkedListNode.next = p.linkedListNode.next;
                        buckets[(p.hashValue & mask)] = node3;
                    }
                }
            }
        }
    }

    @Override
    public void put(T object) {
        int hashValue = keyHash.hash(object);
        int index = (hashValue & mask);
        SecondaryHashIndexNode<T> node = buckets[index];
        if (node != null && node.linkedListNode.object != null) {
            SecondaryHashIndexNode<T> p = node;
            do {
                if (p.linkedListNode.object != null && p.hashValue == hashValue && keyEqual.equal(object, p.linkedListNode.object)) {
                    LinkedListNode<T> newNode = new LinkedListNode<>(object);
                    newNode.next = p.linkedListNode.next;
                    p.linkedListNode.next = newNode;
                    return;
                }
                p = p.next;
            } while (p != null);

            SecondaryHashIndexNode<T> newNode = new SecondaryHashIndexNode<>(object, hashValue);
            newNode.next = node.next;
            node.next = newNode;
            size++;
            if (size > threshold) {
                resize(capacity * 2);
            }
        } else {
            buckets[index] = new SecondaryHashIndexNode<>(object, hashValue);
            size++;
            if (size > threshold) {
                resize(capacity * 2);
            }
        }
    }

    @Override
    public LinkedListNode<T> find(T key) {
        int hashValue = keyHash.hash(key);
        SecondaryHashIndexNode<T> node = buckets[(hashValue & mask)];
        while (node != null) {
            if (node.linkedListNode.object != null && node.hashValue == hashValue && keyEqual.equal(key, node.linkedListNode.object)) {
                return node.linkedListNode;
            }
            node = node.next;
        }
        return null;
    }

    @Override
    public void remove(T object) {
        int hashValue = keyHash.hash(object);
        int index = (hashValue & mask);
        SecondaryHashIndexNode<T> current = buckets[index];
        SecondaryHashIndexNode<T> previous = null;

        while (current != null) {
            if (current.linkedListNode.object != null && current.hashValue == hashValue && keyEqual.equal(object, current.linkedListNode.object)) {
                if (current.linkedListNode.object == object) {
                    if (current.linkedListNode.next != null) {
                        LinkedListNode<T> p = current.linkedListNode.next;
                        current.linkedListNode.object = p.object;
                        current.linkedListNode.next = p.next;
                    } else {
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
                    }
                } else {
                    LinkedListNode<T> previousLinkedNode = current.linkedListNode;
                    LinkedListNode<T> currentLinkedNode = previousLinkedNode.next;
                    while (currentLinkedNode != null) {
                        if (currentLinkedNode.object == object) {
                            previousLinkedNode.next = currentLinkedNode.next;
                            break;
                        }
                        previousLinkedNode = currentLinkedNode;
                        currentLinkedNode = currentLinkedNode.next;
                    }
                }
                return;
            }
            previous = current;
            current = current.next;
        }
    }

    public long getSize() {
        return size;
    }

    public long getCapacity() {
        return capacity;
    }
}
