#include <iostream>
#include <vector>

using namespace std;

#define forn(i,n) for(int i=0;i<(int)(n);i++)
#define forsn(i,s,n) for(int i=(s);i<(int)(n);i++)
#define dforn(i,n) for(int i=(n)-1;i>=0;i--)
#define dforsn(i,s,n) for(int i=(n)-1;i>=(int)(s);i--)

#include "SegmentTree.h"

// Codigo del nodo, no iria al notebook.

// Ejemplo de nodo para usar el minimo.
// Notar que las funciones update y split reciben los i,j que indican el intervalo representado por el nodo.
// En muchos casos, por ejemplo si no se hace lazy update, esos indices no importan, y en los casos mas comunes
// de lazy update solo importa el tamanio del intervalo. Es muy facil cambiar el codigo borrando esos parametros,
// y sacandolos de la llamada en el codigo de segment_tree, pero esta bueno que este para dejar clara esa posibilidad.
// Tambien se puede cambiar para que pase la longitud solamente, cuando eso es todo lo que se necesita.
struct MinNode
{
    // Estos typedef van solo para que les quede claro los tipos del template, donde encajan en esta implementacion,
    // y la signatura que se espera tengan las funciones merge, update y split usadas por el segment tree.
    // Podria usar los tipos concretos de una y listo aca, sin ningun typedef.
    typedef MinNode Node;
    typedef int UpdateData;
    int minimum;
    MinNode() : minimum(0x7FFFFFFF) {}
    void merge(const Node &a, const Node &b)
    // La idea de la operacion merge, es asumiendo que los dos hijos estan perfectamente calculados, calcular el nodo actual.
    // Si estamos haciendo merge, no tenemos informacion lazy (ya se hizo el split).
    {
        minimum = min(a.minimum, b.minimum);
    }
    void update(int i,int j, UpdateData x) // El nodo que se updatea no necesariamente es una hoja cuando se usa directamente updateRange (caso lazy).
    {
        minimum = x;
    }
    void split(int i,int j, Node &a, Node &b) // Notar la signatura: Esta modifica los hijos.
    // La idea de la operacion split es propagar la informacion lazy que tengamos a los dos hijos.
    // Deberiamos borrar esta informacion para no propagarla de nuevo en otro split.
    // Es decir, la implementacion del segment tree piensa a split como una operacion
    // idempotente, que si la hacemos N > 1 veces seguidas o 1 sola da igual.
    // Y por lo tanto si se hacen 5 queries seguidas identicas, en todas ellas se llaman split en los mismos nodos.
    // Pero normalmente solo la primera propagaria, en las siguientes los split encontrarian ya todo propagado.
    {
        // NO-OP: No hay informacion lazy que propagar.
    }
};

struct SumNode
{
    int s;
    SumNode() : s(0) {}
    void merge(const SumNode &a, const SumNode &b)
    {
        s = a.s + b.s;
    }
    void update(int i,int j, int L) {s = L;}
    void split(int i, int j, SumNode &a, SumNode &b) {}
};

struct LazySumNode
{
    int s,lazy;
    LazySumNode() : s(0), lazy(0) {}
    void merge(const LazySumNode &a, const LazySumNode &b)
    {
        s = a.s + b.s;
        lazy = 0;
    }
    void update(int i,int j, int L) {lazy += L; s += L * (j-i);}
    void split(int i,int j, LazySumNode &a, LazySumNode &b)
    {
        int c = (i+j)/2;
        a.update(i,c,lazy);
        b.update(c,j,lazy);
        lazy = 0;
    }
};

// INICIO DE CODIGO DEL SEGMENT_TREE_HACKERCUP

typedef long long tint;
const tint MODU = 1000000000;

struct HackerCupUpdateData
{
    tint val; // Poner este valor, -1 para no hacer nada
    tint C,D; // Coeficientes de la lineal
};

tint oddInRange(tint a,tint b)
{
    return (b-a)/2 + ((a%2) * (1 - (b%2)) );
}

tint evenInRange(tint a,tint b)
{
    return b - a - oddInRange(a,b);
}

tint integral(tint i, tint j)
{
    return ((j * (j-1) - i * (i-1)) / 2) % MODU;
}

struct HackerCupNode
{
    // Lazy
    tint lC, lD; // Coeficientes de la lineal del lazy
    tint lVal; // Si es != -1, todo lo que cuelga de este arbol debe ser seteado a este valor, antes de aplicarle lC,lD
    // Statistics
    tint sum;
    tint oddAtEven, oddAtOdd; // Cantidad de impares en posiciones pares / impares.
    
    HackerCupNode() : lC(0), lD(0), lVal(-1), sum(0), oddAtEven(0), oddAtOdd(0) {}
    void merge(const HackerCupNode& a, const HackerCupNode& b)
    {
        oddAtEven = a.oddAtEven + b.oddAtEven;
        oddAtOdd  = a.oddAtOdd + b.oddAtOdd;
        sum = (a.sum + b.sum) % MODU;
        lC = 0;
        lD = 0;
        lVal = -1;
    }
    void update(int i, int j, const HackerCupUpdateData& data)
    {
        if (data.val != -1)
        {
            lVal = data.val;
            lC = 0;
            lD = 0;
            sum = ((j-i) * data.val) % MODU;
            oddAtEven = evenInRange(i,j) * (data.val % 2);
            oddAtOdd = oddInRange(i,j) * (data.val % 2);
        }
        lC += data.C;
        lD += data.D;
        sum += data.C * (j-i);
        sum %= MODU;
        sum += data.D * integral(i, j);
        sum %= MODU;
        if (data.C % 2)
        {
            oddAtEven = evenInRange(i,j) - oddAtEven;
            oddAtOdd  = oddInRange(i,j) - oddAtOdd;
        }
        if (data.D % 2)
        {
            oddAtOdd  = oddInRange(i,j) - oddAtOdd;
        }
    }
    void split(int i, int j, HackerCupNode &a, HackerCupNode &b)
    {
        int c = (i+j)/2;
        HackerCupUpdateData sd;
        sd.C = lC;
        sd.D = lD;
        sd.val = lVal;
        a.update(i,c, sd);
        b.update(c,j, sd);
        lC = 0;
        lD = 0;
        lVal = -1;
    }
};

// FIN DE CODIGO DEL SEGMENT_TREE_HACKERCUP

// Nuevo ejemplo bien cortito: Esto seria todo lo que hay que agregar al codigo de notebook para un lazy-update con suma.

struct LazySumData
{
    int lazy;
    LazySumData() : lazy(0) {}
    void update(int i,int j, const LazySumData &o) {lazy += o.lazy;};
};


struct LazySumStat
{
    int s;
    LazySumStat() : s(0) {}
    void merge(const LazySumStat &a, const LazySumStat &b)
    {
        s = a.s + b.s;
    }
    void update(int i,int j, const LazySumData &data) {s += data.lazy * (j-i);}
};

// Ejemplo de uso

ostream & operator <<(ostream &o, const MinNode&m)
{
    return o << m.minimum;
}

ostream & operator <<(ostream &o, const LazySumNode&m)
{
    return o << m.s << " " << m.lazy;
}

ostream & operator <<(ostream &o, const LazySumStat& m)
{
    return o << m.s;
}


int main()
{
    int N = 10;
    SegmentTree<LazySumStat, LazySumData>::Node *v;
    SegmentTree<LazySumStat, LazySumData> tree(N, v); // En esta version, todo bien con la memoria local de stack
    forn(i,N) v[i].stat.s = i*i;
    tree.init();
    //forn(i,tree.N*2)
    //    cout << i << "   " << tree.v[i].s << " " << tree.v[i].lazy << endl;
    cout << tree.get(2,5) << endl;
    cout << tree.get(2,5) << endl;
    LazySumData data; data.lazy = 77;
    tree.update(2,data);
    cout << tree.get(2,5) << endl;
    cout << tree.get(2,5) << endl;
    data.lazy = 1;
    tree.updateRange(2,4,data);
    cout << tree.get(2,5) << endl;
    cout << tree.get(2,5) << endl;
    return 0;
}
