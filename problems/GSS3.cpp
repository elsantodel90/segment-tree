// http://www.spoj.com/problems/GSS3/

#include <cstdio>
#include <vector>

using namespace std;

#define forn(i,n) for(int i=0;i<(int)(n);i++)
#define forsn(i,s,n) for(int i=(s);i<(int)(n);i++)
#define dforsn(i,s,n) for(int i=(n)-1;i>=(int)(s);i--)

#include "../SegmentTreeNotebook.h"

const int INF = 1000000000;

// Podriamos borrar todo esto y usar un int comun en lugar de Update, borrando las lineas correspondientes SLU en la implementacion del notebook,
// porque no queremos lazy update.
struct Update
{
    bool nop;
    int val;
    Update () : nop(true) {}
    Update (int k) : nop(false), val(k) {}
    void update(int i,int j, const Update &o) {} // No nos importa esta operacion porque no hacemos lazy update
};

struct Stat
{
    int bestPrefix, bestSuffix, best;
    int sum;
    Stat() : bestPrefix(-INF), bestSuffix(-INF), best(-INF), sum(0) {}
    Stat(int k) : bestPrefix(k), bestSuffix(k), best(k), sum(k) {}
    Stat(const Stat &a, const Stat &b) 
    {
        sum = a.sum + b.sum;
        bestPrefix = max(a.bestPrefix, a.sum + b.bestPrefix);
        bestSuffix = max(b.bestSuffix, b.sum + a.bestSuffix);
        best = max(max(a.best, b.best), a.bestSuffix + b.bestPrefix);
    }
    void update(int i, int j, const Update &up)
    {
        if (!up.nop) sum = bestPrefix = bestSuffix = best = up.val;
    }
};

int main()
{
    #ifdef ACMTUYO
        freopen("GSS3.in","r",stdin);
    #endif
    int n; scanf("%d", &n);
    SegmentTree<Stat, Update>::Node *v;
    SegmentTree<Stat, Update> tree(n, v);
    forn(i,n)
    {
        int val; scanf("%d", &val);
        v[i].stat = val;
    }
    tree.init();
    int m; scanf("%d", &m);
    forn(operation, m)
    {
        int t,x,y; scanf("%d%d%d", &t, &x, &y);
        if (t == 0)
            tree.update(x-1, y);
        else
            printf("%d\n", tree.get(x-1, y).best);
    }
    return 0;
}
