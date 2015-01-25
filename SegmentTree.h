#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

// Codigo del Segment Tree con Lazy Update propiamente (notebook)

// Contrato:
// Stats tiene que tener merge, update y constructor por defecto al elemento neutro de Stats respecto del merge.
// UpdateData tiene que tener merge y constructor por defecto al elemento neutro de UpdateData respecto del merge, neutro que ademas no deberia modificar Stats al hacer update.
// En otras palabras, se espera que UpdateData sea un monoide no necesariamente conmutativo con el merge, y constructor por defecto al elemento neutro.
// De la misma manera se espera que Stats con su merge y constructor por defecto tambien sea un monoide, no necesariamente conmutativo.
// Y ademas se espera que UpdateData sea una accion sobre Stats, es decir, con un poco de abuso de notacion (porque esos metodos son void, pero hacemos de cuenta que devuelven el objeto que queda luego de modificar):
//  -->  stat.update(neutro) == stat
//  --> (stat.update(data1)).update(data2) == stat.update(data1.merge(data2))

template <typename Stats, typename UpdateData>
struct SegmentTree
{
    struct Node
    {
        UpdateData lazy;
        Stats stat;
        void merge(const Node &a, const Node &b) {stat.merge(a.stat,b.stat);}
        void update(int i, int j, const UpdateData &data) {lazy.update(i,j,data); stat.update(i,j,data);}
        void split(int i, int j, int c, Node &a, Node &b)
        {
            a.update(i,c, lazy);
            b.update(c,j, lazy);
            lazy = UpdateData();
        }
    };
    int N, qA, qB;
    UpdateData qData;
    vector<Node> v;
    SegmentTree(int userN, Node* &userData) {N = 1; while (N < userN) N *= 2; v.resize(2*N); userData = v.data() + N;}
    // Funciones internas (private)
    #define NODE  v[i]
    #define LEFT  v[2*i]
    #define RIGHT v[2*i+1]
    Stats iget(int a, int b, int i) // Get interno
    {
        if (qA <= a && b <= qB) return NODE.stat;
        if (qB <= a || b <= qA) return Stats();
        int c = (a+b)/2;
        NODE.split(a,b,c, LEFT, RIGHT);
        Stats res;
        res.merge(iget(a, c, 2*i), iget(c, b, 2*i+1));
        return res;
    }
    void iupdate(int a, int b, int i) // Update interno
    {
        if (qA <= a && b <= qB) {NODE.update(a,b, qData); return ;}
        if (qB <= a || b <= qA) return;
        int c = (a+b)/2;
        NODE.split(a,b,c, LEFT, RIGHT);
        iupdate(a, c, 2*i);
        iupdate(c, b, 2*i+1);
        NODE.merge(LEFT, RIGHT);
    }
    // Funciones que puede llamar el usuario (public)
    void init() {dforsn(i,1, N) NODE.merge(LEFT, RIGHT);} // Se inicializa luego de llenar el arreglo userData con los valores iniciales.
    Stats get(int a,int b) {qA = a; qB = b; return iget(0, N, 1); } // Get que llama el usuario
    void updateRange(int a,int b, const UpdateData &data) {qData = data; qA = a; qB = b; iupdate(0, N, 1);} // updateRange que puede llamar el usuario
    void update(int i, const UpdateData &data) { updateRange(i,i+1, data); } // update que puede llamar el usuario
};





#endif // SEGMENT_TREE_H
