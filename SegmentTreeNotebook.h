#ifndef SEGMENT_TREE_NOTEBOOK_H
#define SEGMENT_TREE_NOTEBOOK_H

// Es la misma idea que SegmentTree.cpp, pero compactada para notebook.

template <typename Stats, typename UpdateData>
struct SegmentTree {
    struct Node {
        UpdateData lazy; // SLU
        Stats stat;
        void merge(const Node &a, const Node &b) {stat = Stats(a.stat,b.stat);}
        void update(int i, int j, const UpdateData &data) {
            lazy.update(i,j,data); // SLU
            stat.update(i,j,data); }
        void split(int i, int j, int c, Node &a, Node &b) { // SLU
            a.update(i,c, lazy); b.update(c,j, lazy); lazy = UpdateData(); } };
    int N, qA, qB; UpdateData qData; vector<Node> v;
    SegmentTree(int userN, Node* &userData) {N = 1; while (N < userN) N *= 2; v.resize(2*N); userData = v.data() + N;}
    #define NODE  v[i]
    #define LEFT  v[2*i]
    #define RIGHT v[2*i+1]
    Stats iget(int a, int b, int i) {
        if (qA <= a && b <= qB) return NODE.stat;
        if (qB <= a || b <= qA) return Stats();
        int c = (a+b)/2;
        NODE.split(a,b,c, LEFT, RIGHT); // SLU
        return Stats(iget(a, c, 2*i), iget(c, b, 2*i+1)); }
    void iupdate(int a, int b, int i) {
        if (qA <= a && b <= qB) {NODE.update(a,b, qData); return ;}
        if (qB <= a || b <= qA) return;
        int c = (a+b)/2;
        NODE.split(a,b,c, LEFT, RIGHT); // SLU
        iupdate(a, c, 2*i);
        iupdate(c, b, 2*i+1);
        NODE.merge(LEFT, RIGHT); }
    void init() {dforsn(i,1, N) NODE.merge(LEFT, RIGHT);}
    Stats get(int a,int b) {qA = a; qB = b; return iget(0, N, 1); }
    void updateRange(int a,int b, const UpdateData &data) {qData = data; qA = a; qB = b; iupdate(0, N, 1);} // SLU
    void update(int i, const UpdateData &data) { qData = data; qA = i; qB = i+1; iupdate(0, N, 1); }
};


#endif // SEGMENT_TREE_NOTEBOOK_H

