#include <iostream>
#include <vector>

using namespace std;

#define forn(i,n) for(int i=0;i<(int)(n);i++)
#define forsn(i,s,n) for(int i=(s);i<(int)(n);i++)
#define dforn(i,n) for(int i=(n)-1;i>=0;i--)
#define dforsn(i,s,n) for(int i=(n)-1;i>=(int)(s);i--)

#include "SegmentTree.h"

// Codigos de nodos, no irian al notebook.

// Nuevo ejemplo bien cortito: Esto seria todo lo que hay que agregar al codigo de notebook para un lazy-update con suma.
// Notar que el codigo de esto es el mismo usemos lazy-update o no: Si no vamos a necesitar lazy update, se pueden comentar
// varias lineas en la implementacion del segment tree, y no haria falta la operacion update de SumData, ni el constructor default del mismo.
// Con lo cual en ese caso podriamos usar de sumData directamente a un int, lo cual es comodo porque ni hay que programar este structcito.

struct SumData
{
    int lazy;
    SumData() : lazy(0) {}
    SumData(int val) : lazy(val) {}
    void update(int i,int j, const SumData &o) {lazy += o.lazy;}; // Esta solo hace falta cuando usamos Lazy Update
};


struct SumStat
{
    int s;
    SumStat() : s(0) {}
    SumStat(int val) : s(val) {}
    SumStat(const SumStat &a, const SumStat &b) {merge(a,b);}
    void merge(const SumStat &a, const SumStat &b, const SumData &data = 0) {
        s = a.s + b.s;
    }
    void update(int i,int j, const SumData &data) {s += data.lazy * (j-i);} // Podria ser simplemente s += data.lazy si no usamos lazyUpdate, ya que en ese caso j-i == 1 siempre.
};

// Ejemplo de uso.

void ejemploSumaConLazyUpdate()
{
    // Notar que al haberle puesto un constructor que toma int a LazySumStat y LazySumData, podemos asignarle ints de una,
    //  lo cual nos limpia un poco la sintaxis en este ejemplo pavo donde los structs no son nada mas que un int.
    int N = 10;
    SegmentTree<SumStat, SumData>::Node *v;
    SegmentTree<SumStat, SumData> tree(N, v); // En esta version, todo bien con la memoria local de stack
    forn(i,N) v[i].stat = i*i; // Se inicializa el v[i].stat, y los datos lazy se dejan como estan.
    tree.init();
    // Impresion del estado interno completo, a modo de DEBUG de que se construyo bien.
    forn(i,tree.N*2)
        cout << i << "   " << tree.v[i].stat.s << " " << tree.v[i].lazy.lazy << endl;
        
    cout << tree.get(2,5).s << endl;
    cout << tree.get(2,5).s << endl;
    tree.update(2,77);
    cout << tree.get(2,5).s << endl;
    cout << tree.get(2,5).s << endl;
    tree.updateRange(2,4,1);
    cout << tree.get(2,5).s << endl;
    cout << tree.get(2,5).s << endl;
    // Impresion del estado interno completo, a modo de DEBUG de que quedo lo que tenia que quedar.
    forn(i,tree.N*2)
        cout << i << "   " << tree.v[i].stat.s << " " << tree.v[i].lazy.lazy << endl;
}

// Para que compile este, hay que comentar las lineas correspondientes en el codigo de SegmentTree

/*
void ejemploSumaComunSinLazyUpdate()
{
    // Notar que al haberle puesto un constructor que toma int a LazySumStat y LazySumData, podemos asignarle ints de una,
    //  lo cual nos limpia un poco la sintaxis en este ejemplo pavo donde los structs no son nada mas que un int.
    int N = 10;
    SegmentTree<SumStat, int>::Node *v;
    SegmentTree<SumStat, int> tree(N, v); // En esta version, todo bien con la memoria local de stack
    forn(i,N) v[i].stat = i*i; // Se inicializa el v[i].stat, y los datos lazy se dejan como estan.
    tree.init();
    // Impresion del estado interno completo, a modo de DEBUG de que se construyo bien.
    forn(i,tree.N*2)
        cout << i << "   " << tree.v[i].stat.s << endl;
        
    cout << tree.get(2,5).s << endl;
    cout << tree.get(2,5).s << endl;
    tree.update(2,77);
    cout << tree.get(2,5).s << endl;
    cout << tree.get(2,5).s << endl;
    tree.update(3,10);
    cout << tree.get(2,5).s << endl;
    cout << tree.get(2,5).s << endl;
    // Impresion del estado interno completo, a modo de DEBUG de que quedo lo que tenia que quedar.
    forn(i,tree.N*2)
        cout << i << "   " << tree.v[i].stat.s << endl;
}
*/
int main()
{
    //ejemploSumaComunSinLazyUpdate(); // Para que compile este, hay que comentar las lineas correspondientes en el codigo de SegmentTree
    ejemploSumaConLazyUpdate();
    return 0;
}
