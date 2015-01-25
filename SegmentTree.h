#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

// Codigo del Segment Tree propiamente (notebook)
// Node debe implementar merge, split y update, y tener un constructor default al elemento neutro.
// La implementacion de split hace falta que sea no vacia solo en el caso de lazy-update.

const int TREE_MAXN = 1000;

template <typename Node, typename UpdateData>
struct SegmentTree
{
    int N, qA, qB;
    UpdateData qData;
    Node v[4*TREE_MAXN];
    Node *preInit(int userN) {N = 1; while (N < userN) N *= 2; return v + N;}
    void init() {dforsn(i,1, N) v[i].merge(v[2*i], v[2*i+1]);}
    Node iget(int a, int b, int i)
    {
        if (qA <= a && b <= qB) return v[i];
        if (qB <= a || b <= qA) return Node();
        v[i].split(a,b, v[2*i], v[2*i+1]);
        int c = (a+b)/2;
        Node res;
        res.merge(iget(a, c, 2*i), iget(c, b, 2*i+1));
        return res;
    }
    Node get(int a,int b) {qA = a; qB = b; return iget(0, N, 1); }
    void iupdate(int a, int b, int i)
    {
        if (qA <= a && b <= qB) {v[i].update(a,b, qData); return ;}
        if (qB <= a || b <= qA) return;
        v[i].split(a,b, v[2*i], v[2*i+1]);
        int c = (a+b)/2;
        iupdate(a, c, 2*i);
        iupdate(c, b, 2*i+1);
        v[i].merge(v[2*i], v[2*i+1]);
    }
    void updateRange(int a,int b, const UpdateData &data) {qData = data; qA = a; qB = b; iupdate(0, N, 1);}
    void update(int i, const UpdateData &data) { updateRange(i,i+1, data); }
};





#endif // SEGMENT_TREE_H
