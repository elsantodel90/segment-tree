// http://www.spoj.com/problems/GSS3/

#include <cstdio>
#include <vector>

using namespace std;

#define forn(i,n) for(int i=0;i<(int)(n);i++)
#define forsn(i,s,n) for(int i=(s);i<(int)(n);i++)
#define dforsn(i,s,n) for(int i=(n)-1;i>=(int)(s);i--)

// Para que esta version compile, hay que comentar las lineas SLU en el codigo.
#include "../SegmentTreeNotebook.h"

const int INF = 1000000000;

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
    void update(int i, int j, int val)
    {
        sum = bestPrefix = bestSuffix = best = val;
    }
};

int main()
{
    #ifdef ACMTUYO
        freopen("GSS3.in","r",stdin);
    #endif
    int n; scanf("%d", &n);
    SegmentTree<Stat, int>::Node *v;
    SegmentTree<Stat, int> tree(n, v);
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
