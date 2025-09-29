package dynagox.experiments.common;

public class CommonEvent {
    public int key;
    public boolean isInsert;
    public int rid;
    public int f0;
    public int f1;

    public CommonEvent(int key, int rid, int f0, int f1) {
        this(key, true, rid, f0, f1);
    }

    public CommonEvent(int key, boolean isInsert, int rid, int f0, int f1) {
        this.key = key;
        this.isInsert = isInsert;
        this.rid = rid;
        this.f0 = f0;
        this.f1 = f1;
    }
}
