package dynagox.experiments.common;

public class CommonEventL {
    public long key;
    public boolean isInsert;
    public int rid;
    public long f0;
    public long f1;

    public CommonEventL(long key, int rid, long f0, long f1) {
        this(key, true, rid, f0, f1);
    }

    public CommonEventL(long key, boolean isInsert, int rid, long f0, long f1) {
        this.key = key;
        this.isInsert = isInsert;
        this.rid = rid;
        this.f0 = f0;
        this.f1 = f1;
    }
}
