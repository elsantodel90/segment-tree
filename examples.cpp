#include <iostream>
#include <vector>

using namespace std;

#define forn(i,n) for(int i=0;i<(int)(n);i++)
#define forsn(i,s,n) for(int i=(s);i<(int)(n);i++)
#define dforn(i,n) for(int i=(n)-1;i>=0;i--)
#define dforsn(i,s,n) for(int i=(n)-1;i>=(int)(s);i--)

#include "SegmentTree.h"

// Codigos de nodos, no irian al notebook.

// Ejemplo complejo: Problema D Round 2, Hacker Cup 2015
// INICIO DE CODIGO DEL SEGMENT_TREE_HACKERCUP

typedef long long tint;
const tint MODU = 1000000000;

struct HackerCupUpdateData
{
    tint val; // Poner este valor, -1 para no hacer nada
    tint C,D; // Coeficientes de la lineal
    HackerCupUpdateData() : val(-1), C(0), D(0) {}
    void update(int i, int j, const HackerCupUpdateData& data)
    {
        if (data.val != -1)
        {
            val = data.val;
            C = 0;
            D = 0;
        }
        C += data.C;
        D += data.D;
    }
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

struct HackerCupStat
{
    // Statistics
    tint sum;
    tint oddAtEven, oddAtOdd; // Cantidad de impares en posiciones pares / impares.
    
    HackerCupStat() : sum(0), oddAtEven(0), oddAtOdd(0) {}
    HackerCupStat(const HackerCupStat& a, const HackerCupStat& b)
    {
        oddAtEven = a.oddAtEven + b.oddAtEven;
        oddAtOdd  = a.oddAtOdd + b.oddAtOdd;
        sum = (a.sum + b.sum) % MODU;
    }
    void update(int i, int j, const HackerCupUpdateData& data)
    {
        if (data.val != -1)
        {
            sum = ((j-i) * data.val) % MODU;
            oddAtEven = evenInRange(i,j) * (data.val % 2);
            oddAtOdd = oddInRange(i,j) * (data.val % 2);
        }
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
};

// FIN DE CODIGO DEL SEGMENT_TREE_HACKERCUP

// Nuevo ejemplo bien cortito: Esto seria todo lo que hay que agregar al codigo de notebook para un lazy-update con suma.
// Notar que el codigo de esto es el mismo usemos lazy-update o no: Si no vamos a necesitar lazy update, se pueden comentar
// varias lineas en la implementacion del segment tree, y no haria falta la operacion update de SumData, ni el constructor default del mismo.
// Con lo cual en ese caso podriamos usar de sumData directamente a un int, lo cual es comodo porque ni hay que programar este structcito.

struct SumUpdateData
{
    int lazy;
    SumUpdateData() : lazy(0) {}
    SumUpdateData(int val) : lazy(val) {}
    void update(int i,int j, const SumUpdateData &o) {lazy += o.lazy;};
};


struct SumStat
{
    int s;
    SumStat() : s(0) {}
    SumStat(int val) : s(val) {}
    SumStat(const SumStat &a, const SumStat &b) : s(a.s + b.s) {}
    void update(int i,int j, const SumUpdateData &data) {s += data.lazy * (j-i);}
};

// Ejemplo de uso.

void ejemploSumaConLazyUpdate()
{
    // Notar que al haberle puesto un constructor que toma int a LazySumStat y LazySumData, podemos asignarle ints de una,
    //  lo cual nos limpia un poco la sintaxis en este ejemplo pavo donde los structs no son nada mas que un int.
    int N = 10;
    SegmentTree<SumStat, SumUpdateData>::Node *v;
    SegmentTree<SumStat, SumUpdateData> tree(N, v); // En esta version, todo bien con la memoria local de stack
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

int main()
{
    ejemploSumaComunSinLazyUpdate(); // Para que compile este, hay que comentar las lineas correspondientes en el codigo de SegmentTree
    ejemploSumaConLazyUpdate();
    return 0;
}
