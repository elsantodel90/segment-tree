#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

// Codigo del Segment Tree propiamente (notebook)
// Node debe implementar merge, split y update, y tener un constructor default al elemento neutro.
// La implementacion de split hace falta que sea no vacia solo en el caso de lazy-update.

template <typename Node, typename UpdateData>
struct SegmentTree
{
    int N, qA, qB;
    UpdateData qData;
    vector<Node> v;
    SegmentTree(int userN, Node* &userData) {N = 1; while (N < userN) N *= 2; v.resize(2*N); userData = v.data() + N;}
    // Funciones internas (private)
    Node iget(int a, int b, int i) // Get interno
    {
        if (qA <= a && b <= qB) return v[i];
        if (qB <= a || b <= qA) return Node();
        v[i].split(a,b, v[2*i], v[2*i+1]);
        int c = (a+b)/2;
        Node res;
        res.merge(iget(a, c, 2*i), iget(c, b, 2*i+1));
        return res;
    }
    void iupdate(int a, int b, int i) // Update interno
    {
        if (qA <= a && b <= qB) {v[i].update(a,b, qData); return ;}
        if (qB <= a || b <= qA) return;
        v[i].split(a,b, v[2*i], v[2*i+1]);
        int c = (a+b)/2;
        iupdate(a, c, 2*i);
        iupdate(c, b, 2*i+1);
        v[i].merge(v[2*i], v[2*i+1]);
    }
    // Funciones que puede llamar el usuario (public)
    void init() {dforsn(i,1, N) v[i].merge(v[2*i], v[2*i+1]);} // Se inicializa luego de llenar el arreglo userData con los valores iniciales.
    Node get(int a,int b) {qA = a; qB = b; return iget(0, N, 1); } // Get que llama el usuario
    void updateRange(int a,int b, const UpdateData &data) {qData = data; qA = a; qB = b; iupdate(0, N, 1);} // updateRange que puede llamar el usuario
    void update(int i, const UpdateData &data) { updateRange(i,i+1, data); } // update que puede llamar el usuario
};





#endif // SEGMENT_TREE_H
