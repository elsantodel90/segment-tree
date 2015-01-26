// http://www.spoj.com/problems/LIS2/

#include <iostream>
#include <cstdio>
#include <vector>
#include <map>

using namespace std;

#define forn(i,n) for(int i=0;i<(int)(n);i++)
#define forsn(i,s,n) for(int i=(s);i<(int)(n);i++)
#define dforsn(i,s,n) for(int i=(n)-1;i>=(int)(s);i--)
#define forall(it,c) for(typeof((c).begin()) it = (c).begin(); it != (c).end(); it++)

// Para que esta version compile, hay que comentar las lineas SLU en el codigo.
#include "../SegmentTree.h"

const int INF = 2000000000;

typedef pair<int,int> pint;

struct Stat
{
    map<int,int> yToLen;
    Stat()
    {
        yToLen[-INF] = 0; // Hace que la busqueda nunca de vacio mas adelante y evita ese if.
    }
    void merge(const Stat &a, const Stat &b, const pint& q)
    {
        update(0,0,q);
    }
    void update(int i, int j, const pint &q)
    {
        int queryY = q.first;
        int value  = q.second;
        map<int,int>::iterator it = yToLen.lower_bound(queryY);
        while (it != yToLen.end() && it->second <= value)
        {
            map<int,int>::iterator deleteIt = it;
            it++;
            yToLen.erase(deleteIt);
        }
        if (it == yToLen.end() || it->first > queryY)
            yToLen[queryY] = value;
    }
};

int queryY; // La query pregunta por los MENOREs que queryY

struct Result
{
    int res;
    Result() : res(0) {}
    Result(const Stat &s) {
        // La query real 2 en 2D: Del global queryY sacamos la y de la query
        //  Ese truco tipico piola se puede usar en general para armar cosas 2D sin tocar el codigo del SegmentTree
        map<int,int>::const_iterator it = s.yToLen.lower_bound(queryY);        
        it--;
        res = it->second;
    }
    Result(const Result &a, const Result &b) : res(max(a.res, b.res)) {}
};

const int MAXN = 110000;

int main()
{
    static pint a[MAXN];
    #ifdef ACMTUYO
        freopen("LIS2.in","r",stdin);
    #endif
    int n; scanf("%d", &n);
    map<int,int> cx;
    forn(i,n)
    {
        scanf("%d%d", &a[i].first, &a[i].second);
        cx[a[i].first] = 0;
    }
    int CX = 0;
    forall(it, cx) it->second = CX++;
    SegmentTree<Stat, pint, Result >::Node *v;
    SegmentTree<Stat, pint, Result > tree(CX, v);
    // tree.init(); Como queremos que todos los nodos arranquen con el constructor default (elemento neutro)
    //               ni siquiera necesitamos llamar init, o inicializar especialmente nada, en este caso.
    int res = 0;
    forn(i,n)
    {
        queryY = a[i].second;
        int queryX = cx[a[i].first];
        int dpI = 1 + tree.get(0, queryX).res;
        res = max(res, dpI);
        tree.update(queryX,make_pair(queryY, dpI));
    }
    printf("%d\n", res);
    return 0;
}
