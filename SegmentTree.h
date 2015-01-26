#ifndef SEGMENT_TREE_NOTEBOOK_H
#define SEGMENT_TREE_NOTEBOOK_H

// Es la misma idea que SegmentTree.cpp, pero compactada para notebook.

template <typename Stat, typename Update, typename Result = Stat, typename Lazy = Update>
struct SegmentTree {
    struct Node {
        Lazy lazy; // SLU
        Stat stat;
        void update(int i, int j, const Update &data) {
            lazy.update(i,j,data); // SLU
            stat.update(i,j,data); }
        void split(int i, int j, int c, Node &a, Node &b) { // SLU
            a.update(i,c, lazy); b.update(c,j, lazy); lazy = Lazy(); } }; 
    int N, qA, qB; Update qData; vector<Node> v;
    SegmentTree(int userN, Node* &userData) {N = 1; while (N < userN) N *= 2; v.resize(2*N); userData = v.data() + N;}
    #define NODE  v[i]
    #define LEFT  v[2*i]
    #define RIGHT v[2*i+1]
    Result iget(int a, int b, int i) {
        if (qA <= a && b <= qB) return NODE.stat;
        if (qB <= a || b <= qA) return Result();
        int c = (a+b)/2;
        NODE.split(a,b,c, LEFT, RIGHT); // SLU
        return Result(iget(a, c, 2*i), iget(c, b, 2*i+1)); }
    void iupdate(int a, int b, int i) {
        if (qA <= a && b <= qB) {NODE.update(a,b, qData); return ;}
        if (qB <= a || b <= qA) return;
        int c = (a+b)/2;
        NODE.split(a,b,c, LEFT, RIGHT); // SLU
        iupdate(a, c, 2*i);
        iupdate(c, b, 2*i+1);
        NODE.stat.merge(LEFT.stat, RIGHT.stat, qData); }
    void init() {dforsn(i,1, N) NODE.stat.merge(LEFT.stat, RIGHT.stat);}
    Result get(int a,int b) {qA = a; qB = b; return iget(0, N, 1); }
    void updateRange(int a,int b, const Update &data) {qData = data; qA = a; qB = b; iupdate(0, N, 1);} // SLU
    void update(int i, const Update &data) { qData = data; qA = i; qB = i+1; iupdate(0, N, 1); }
};


#endif // SEGMENT_TREE_NOTEBOOK_H

