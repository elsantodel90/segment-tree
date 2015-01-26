// Ejemplo complejo: Problema D Round 2, Hacker Cup 2015
// https://www.facebook.com/hackercup/problems.php?pid=1521672018093383&round=323882677799153

#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <algorithm>
#include <queue>
#include <vector>
#include <set>
#include <map>
#include <cassert>

using namespace std;

#define forn(i,n) for(int i=0;i<(int)(n);i++)
#define forsn(i,s,n) for(int i=(s);i<(int)(n);i++)
#define dforn(i,n) for(int i=(n)-1;i>=0;i--)
#define dforsn(i,s,n) for(int i=(n)-1;i>=(int)(s);i--)

#define forall(it,c) for(typeof((c).begin()) it = (c).begin(); it != (c).end(); it++)
#define dforall(it,c) for(typeof((c).rbegin()) it = (c).rbegin(); it != (c).rend(); it++)

#define esta(x,c) ((c).find(x) != (c).end())

#define all(c) (c).begin(), (c).end()

typedef long long tint;
typedef pair<int,int> pint;
typedef vector<int> vint;

// INICIO DE CODIGO DEL SEGMENT_TREE_HACKERCUP

typedef long long tint;
const tint MODU = 1000000000;

struct HackerCupUpdate
{
    tint val; // Poner este valor, -1 para no hacer nada
    tint C,D; // Coeficientes de la lineal
    HackerCupUpdate() : val(-1), C(0), D(0) {}
    void update(int i, int j, const HackerCupUpdate& data)
    {
        if (data.val != -1)
        {
            val = data.val;
            C = 0;
            D = 0;
        }
        C += data.C;
        C %= MODU;
        D += data.D;
        D %= MODU;
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
    HackerCupStat(const HackerCupStat& a, const HackerCupStat& b) { merge(a,b); }
    void merge(const HackerCupStat& a, const HackerCupStat& b, const HackerCupUpdate &lazy = HackerCupUpdate())
    {
        oddAtEven = a.oddAtEven + b.oddAtEven;
        oddAtOdd  = a.oddAtOdd + b.oddAtOdd;
        sum = (a.sum + b.sum) % MODU;
    }
    void update(int i, int j, const HackerCupUpdate& data)
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

#include "../SegmentTree.h"


const int MAXN = 1100000;

tint S[MAXN],O[MAXN],A[MAXN],B[MAXN],C[MAXN],D[MAXN];

int N, M;

void fillArray(tint v[], tint MOD, tint INC, tint TOP)
{
    tint X,Y,Z;
    cin >> v[0] >> v[1] >> X >> Y >> Z;
    forsn(i, 2, TOP)
    {
        v[i] = X * v[i-2];
        v[i] %= MOD;
        v[i] += Y * v[i-1];
        v[i] %= MOD;
        v[i] += Z;
        v[i] %= MOD;
        
        v[i] += INC;
    }
}

/* "Operaciones atomicas" que nos proveera el rmq.
 * Consultar suma de un rango
   Chantar valor a rango de prepo
   Sumar funcion lineal a un rango
   Dame la cantidad de impares en un rango (te la da divida segun esten en posiciones pares / impares)
 */

int main()
{   
    int TT; cin >> TT;
    forn(tt,TT)
    {
        cin >> N >> M;
        fillArray(S, 1000000000, 0, N);
        fillArray(O, 4, 1, M);
        fillArray(A, N, 1, M);
        fillArray(B, N, 1, M);
        fillArray(C, 1000000000, 0, M);
        fillArray(D, 1000000000, 0, M);
        SegmentTree<HackerCupStat, HackerCupUpdate>::Node *nodes;
        SegmentTree<HackerCupStat, HackerCupUpdate> tree(N, nodes);
        forn(i,N)
        {
            nodes[i].stat.sum = S[i];
            nodes[i].stat.oddAtEven = (i % 2 == 0) * (S[i] % 2);
            nodes[i].stat.oddAtOdd  = (i % 2 == 1) * (S[i] % 2);
        }
        tree.init();
        tint ret = 0;
        forn(i, M)
        {
            int a = A[i]-1;
            int b = B[i];
            HackerCupUpdate data;
            if (O[i] == 1)
            {
                ret += (D[i] * integral(0, b)) % MODU;
                ret %= MODU;
                ret += C[i] * b;
                ret %= MODU;
                data.val = -1;
                data.C = ((C[i] - a * D[i]) % MODU + MODU) % MODU;
                data.D = D[i];
                if (a+b <= N)
                    tree.updateRange(a,a+b,data);
                else
                {
                    tree.updateRange(a,N,data);
                    data.C = ((C[i] + (N-a) * D[i]) % MODU + MODU) % MODU;
                    tree.updateRange(0,b - (N - a),data);
                }
            }
            else if (O[i] == 2)
            {
                ret += C[i] * b;
                ret %= MODU;
                data.val = C[i];
                data.C = 0;
                data.D = 0;
                if (a+b <= N)
                {
                    ret += tree.get(a,a+b).sum;
                    tree.updateRange(a,a+b, data);
                }
                else
                {
                    ret += tree.get(a,N).sum;
                    tree.updateRange(a,N, data);
                    ret %= MODU;
                    ret += tree.get(0, b - (N-a)).sum;
                    tree.updateRange(0, b - (N-a), data);
                }
                ret %= MODU;
            }
            else if (O[i] == 3)
            {
                if (a+b <= N)
                {
                    ret += tree.get(a,a+b).sum;
                }
                else
                {
                    ret += tree.get(a,N).sum;
                    ret %= MODU;
                    ret += tree.get(0, b - (N-a)).sum;
                }
                ret %= MODU;
            }
            else if (O[i] == 4)
            {
                if (a+b <= N)
                {
                    HackerCupStat node = tree.get(a,a+b);
                    ret += node.oddAtEven + node.oddAtOdd;
                }
                else
                {
                    HackerCupStat node = tree.get(a,N);
                    ret += node.oddAtEven + node.oddAtOdd;
                    ret %= MODU;
                    node = tree.get(0,b - (N-a));
                    ret += node.oddAtEven + node.oddAtOdd;
                }
                ret %= MODU;
            }
            else
            {
                assert(false);
            }
        }
        cout << "Case #" << tt+1 << ": " << ret << endl;
    }
    return 0;
}

