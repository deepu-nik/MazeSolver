#include "optimizer.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
typedef struct { int score; uint64_t mask; unsigned char reachable; } Cell;
int tw_optimize(const Attraction *items,int n,int T,int B,Plan *out){
 if(!items||!out||n<0||n>TW_MAX_ITEMS||T<0||B<0||T>2000||B>200000)return 0;
 size_t cols=(size_t)B+1,total=((size_t)T+1)*cols;if(total>2000000)return 0;
 Cell *dp=calloc(total,sizeof(Cell));if(!dp)return 0;dp[0].reachable=1;
 for(int i=0;i<n;i++){
  if(items[i].duration<=0||items[i].cost<0||items[i].rating<0){free(dp);return 0;}
  for(int t=T-items[i].duration;t>=0;t--)for(int c=B-items[i].cost;c>=0;c--){
   size_t src=(size_t)t*cols+c,dst=(size_t)(t+items[i].duration)*cols+c+items[i].cost;
   if(!dp[src].reachable)continue;
   int candidate=dp[src].score+items[i].rating;
   if(!dp[dst].reachable||candidate>dp[dst].score){dp[dst].reachable=1;dp[dst].score=candidate;dp[dst].mask=dp[src].mask|(UINT64_C(1)<<i);}
  }
 }
 memset(out,0,sizeof(*out));int best=-1,bt=0,bc=0;
 for(int t=0;t<=T;t++)for(int c=0;c<=B;c++){size_t k=(size_t)t*cols+c;if(dp[k].reachable&&(dp[k].score>best||(dp[k].score==best&&(t<bt||(t==bt&&c<bc))))){best=dp[k].score;bt=t;bc=c;}}
 out->rating=best<0?0:best;out->used_time=bt;out->used_budget=bc;
 uint64_t mask=dp[(size_t)bt*cols+bc].mask;for(int i=0;i<n;i++)if(mask&(UINT64_C(1)<<i))out->selected[out->count++]=i;
 free(dp);return 1;
}
int tw_bruteforce(const Attraction *items,int n,int T,int B,Plan *out){
 if(!items||!out||n<0||n>22||T<0||B<0)return 0;
 int best=-1;memset(out,0,sizeof(*out));unsigned long long lim=1ULL<<n;
 for(unsigned long long mask=0;mask<lim;mask++){int t=0,c=0,r=0;for(int i=0;i<n;i++)if(mask&(1ULL<<i)){t+=items[i].duration;c+=items[i].cost;r+=items[i].rating;}
 if(t<=T&&c<=B&&r>best){best=r;out->rating=r;out->used_time=t;out->used_budget=c;out->count=0;for(int i=0;i<n;i++)if(mask&(1ULL<<i))out->selected[out->count++]=i;}}
 return 1;
}
