// http://www.spoj.com/problems/BRCKTS/

#include <cstdio>
#include <vector>

using namespace std;

#define forn(i,n) for(int i=0;i<(int)(n);i++)
#define forsn(i,s,n) for(int i=(s);i<(int)(n);i++)
#define dforsn(i,s,n) for(int i=(n)-1;i>=(int)(s);i--)

#include "../SegmentTreeNotebook.h"

char word[32000];

const int INF = 1000000000;

struct Sum
{
    int s;
    Sum() : s(0) {}
    Sum(int k) : s(k) {}
    void update(int i,int j, const Sum &o) {s += o.s;}
};

struct Minimum
{
    int m;
    Minimum () : m(INF) {}
    Minimum (int k) : m(k) {}
    Minimum (const Minimum &a,const Minimum &b) : m(min(a.m, b.m)) {}
    void update(int i,int j,const Sum &s) { m += s.s; }
};

const char *answer[2] = {"NO", "YES"};

int main()
{
    #ifdef ACMTUYO
        freopen("brckts.in","r",stdin);
    #endif
    forn(cases,10)
    {
        printf("Test %d:\n", cases+1);
        int n;
        scanf("%d\n", &n);
        gets(word);
        if (n % 2)
        {
            int m;
            scanf("%d\n", &m);
            forn(i,m)
            {
                int k;
                scanf("%d\n", &k);
                if (k == 0) printf("%s\n", answer[false]);
            }
        }
        else
        {
            SegmentTree<Minimum, Sum>::Node *v;
            SegmentTree<Minimum, Sum> tree(n,v);
            int sum = 0;
            forn(i,n)
            {
                word[i] = 2 * (word[i] == '(') - 1;
                sum += word[i];
                v[i].stat = sum;
            }
            tree.init();
            int m;
            scanf("%d\n", &m);
            forn(operation,m)
            {
                int k;
                scanf("%d\n", &k);
                if (k == 0)
                {
                    bool ret = ((sum == 0) && tree.get(0, n-1).m >= 0);
                    printf("%s\n", answer[ret]);
                }
                else
                {
                    k--;
                    int delta = - 2 * word[k];
                    word[k] = -word[k];
                    sum += delta;
                    tree.updateRange(k,n,delta);
                }
            }
        }
    }
    return 0;
}
