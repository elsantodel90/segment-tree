// http://www.spoj.com/problems/LIS2/

#include <iostream>
#include <cstdio>
#include <vector>
#include <cassert>
#include <algorithm>

using namespace std;

#define forn(i,n) for(int i=0;i<(int)(n);i++)
#define forsn(i,s,n) for(int i=(s);i<(int)(n);i++)
#define dforsn(i,s,n) for(int i=(n)-1;i>=(int)(s);i--)
#define forall(it,c) for(typeof((c).begin()) it = (c).begin(); it != (c).end(); it++)

// Para que esta version compile, hay que comentar las lineas SLU en el codigo.
template <typename Stat, typename Update, typename Result = Stat, typename Lazy = Update>
struct SegmentTree {
    struct Node {
        //Lazy lazy; // SLU
        Stat stat;
        void update(int i, int j, const Update &data) {
            //lazy.update(i,j,data); // SLU
            stat.update(i,j,data); }
        //void split(int i, int j, int c, Node &a, Node &b) { // SLU
        //    a.update(i,c, lazy); b.update(c,j, lazy); lazy = Lazy(); } }; 
    };
    int N, qA, qB; Update qData; vector<Node> v;
    SegmentTree(int userN, Node* &userData) {N = 1; while (N < userN) N *= 2; v.resize(2*N); userData = v.data() + N;}
    #define NODE  v[i]
    #define LEFT  v[2*i]
    #define RIGHT v[2*i+1]
    Result iget(int a, int b, int i) {
        if (qA <= a && b <= qB) return NODE.stat;
        if (qB <= a || b <= qA) return Result();
        int c = (a+b)/2;
        //NODE.split(a,b,c, LEFT, RIGHT); // SLU
        return Result(iget(a, c, 2*i), iget(c, b, 2*i+1)); }
    void iupdate(int a, int b, int i) {
        if (qA <= a && b <= qB) {NODE.update(a,b, qData); return ;}
        if (qB <= a || b <= qA) return;
        int c = (a+b)/2;
        //NODE.split(a,b,c, LEFT, RIGHT); // SLU
        iupdate(a, c, 2*i);
        iupdate(c, b, 2*i+1);
        NODE.stat.merge(LEFT.stat, RIGHT.stat, qData); }
    void init() {dforsn(i,1, N) NODE.stat.merge(LEFT.stat, RIGHT.stat);}
    Result get(int a,int b) {qA = a; qB = b; return iget(0, N, 1); }
    //void updateRange(int a,int b, const Update &data) {qData = data; qA = a; qB = b; iupdate(0, N, 1);} // SLU
    void update(int i, const Update &data) { qData = data; qA = i; qB = i+1; iupdate(0, N, 1); }
};


const int INF = 2000000000;

typedef pair<int,int> pint;

struct Maximum
{
    int val;
    Maximum() : val(0) {}
    Maximum(const Maximum &a, const Maximum &b) : val(max(a.val, b.val)) {}
    void merge(const Maximum &a, const Maximum &b, int q = 0)
    {
        val = max(a.val, b.val);
        // Implementacion alternativa para no duplicar codigo: *this = Maximum(a,b);
    }
    void update(int i,int j, int value)
    {
        val = max(val, value);
    }
};

SegmentTree<Maximum, int>::Node *dummyVar;

int buscar(int v[], int N,  int k)
{
    int a = -1;
    int b = N;
    while (b - a > 1)
    {
        int c = (a+b)/2;
        if (v[c] < k) a = c;
        else b = c;
    }
    return b;
}

struct Stat
{
    SegmentTree<Maximum, int> tree;
    vector<int> yValues;
    Stat() : tree(0,dummyVar) {}
    void merge(const Stat &a, const Stat &b)
    {
        // Merge de los conjuntitos ordenados
        yValues.reserve(a.yValues.size() + b.yValues.size());
        int i = 0, j = 0;
        while (i < (int)a.yValues.size() && j < (int)b.yValues.size())
        {
            int x = a.yValues[i];
            int y = b.yValues[j];
            if (x < y)
            {
                i++;
                yValues.push_back(x);
            }
            else if (x > y)
            {
                j++;
                yValues.push_back(y);
            }
            else
            {
                i++; j++;
                yValues.push_back(x);
            }
        }
        while (i < (int)a.yValues.size())
            yValues.push_back(a.yValues[i++]);
        while (j < (int)b.yValues.size())
            yValues.push_back(b.yValues[j++]);
        // Ahora tengo el tamanio posta
        tree = SegmentTree<Maximum, int>(yValues.size(), dummyVar);
    }
    void merge(const Stat &a, const Stat &b, const pint& q)
    {
        update(0,0,q);
    }
    void update(int i, int j, const pint &q)
    {
        int queryY = index(q.first);
        int value  = q.second;
        tree.update(queryY, value);
    }
    int index(int Y) // Hace un lowerbound
    {
        return buscar(yValues.data(), yValues.size(), Y);
    }
};

int queryY; // La query pregunta por los MENORES que queryY.

struct Result
{
    int res;
    Result() : res(0) {}
    Result(Stat &s) {
        // La query real 2 en 2D: Del global queryY sacamos la y de la query
        //  Ese truco tipico piola se puede usar en general para armar cosas 2D sin tocar el codigo del SegmentTree
        res = s.tree.get(0, s.index(queryY)).val;
    }
    Result(const Result &a, const Result &b) : res(max(a.res, b.res)) {}
};

const int MAXN = 110000;

int main()
{
    static pint a[MAXN];
    static int cx[MAXN];
    #ifdef ACMTUYO
        freopen("LIS2.in","r",stdin);
    #endif
    int n; scanf("%d", &n);
    forn(i,n)
    {
        scanf("%d%d", &a[i].first, &a[i].second);
        cx[i] = a[i].first;
    }
    sort(cx, cx + n);
    int CX = unique(cx, cx + n) - cx;
    SegmentTree<Stat, pint, Result >::Node *v;
    SegmentTree<Stat, pint, Result > tree(CX, v);
    forn(i,n)
        v[buscar(cx,CX,a[i].first)].stat.yValues.push_back(a[i].second);
    forn(i,CX)
    {
        vector<int> &yValues = v[i].stat.yValues;
        sort(yValues.begin(),  yValues.end());
        yValues.resize(unique(yValues.begin(), yValues.end()) - yValues.begin());
        v[i].stat.tree = SegmentTree<Maximum, int>(yValues.size(), dummyVar);
    }
    tree.init(); 
    int res = 0;
    forn(i,n)
    {
        queryY = a[i].second;
        int queryX = buscar(cx,CX, a[i].first);
        int dpI = 1 + tree.get(0, queryX).res;
        res = max(res, dpI);
        tree.update(queryX,make_pair(queryY, dpI));
    }
    printf("%d\n", res);
    return 0;
}
