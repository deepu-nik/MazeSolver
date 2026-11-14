#include "optimizer.h"
#include <stdlib.h>
#include <string.h>
typedef struct { int score; unsigned char *take; } Cell;
int tw_optimize(const Attraction *items,int n,int T,int B,Plan *out){
 if(!items||!out||n<0||n>TW_MAX_ITEMS||T<0||B<0||T>2000||B>200000)return 0;
 size_t cols=(size_t)B+1,total=((size_t)T+1)*cols;if(total>12000000)return 0;
 Cell *dp=calloc(total,sizeof(Cell));if(!dp)return 0;
 dp[0].take=calloc((size_t)n+1,1);if(!dp[0].take){free(dp);return 0;}
 for(int i=0;i<n;i++){
  if(items[i].duration<=0||items[i].cost<0||items[i].rating<0){for(size_t q=0;q<total;q++)free(dp[q].take);free(dp);return 0;}
  for(int t=T-items[i].duration;t>=0;t--)for(int c=B-items[i].cost;c>=0;c--){
   size_t src=(size_t)t*cols+c,dst=(size_t)(t+items[i].duration)*cols+c+items[i].cost;
   if(!dp[src].take)continue;int candidate=dp[src].score+items[i].rating;
   if(!dp[dst].take||candidate>dp[dst].score){unsigned char *bits=calloc((size_t)n+1,1);if(!bits){for(size_t q=0;q<total;q++)free(dp[q].take);free(dp);return 0;}memcpy(bits,dp[src].take,(size_t)n+1);bits[i]=1;free(dp[dst].take);dp[dst].take=bits;dp[dst].score=candidate;}
  }
 }
 memset(out,0,sizeof(*out));int best=0,bt=0,bc=0;size_t bestidx=0;
 for(int t=0;t<=T;t++)for(int c=0;c<=B;c++){size_t k=(size_t)t*cols+c;if(dp[k].take&&(dp[k].score>best||(dp[k].score==best&&(t<bt||(t==bt&&c<bc))))){best=dp[k].score;bt=t;bc=c;bestidx=k;}}
 out->rating=best;out->used_time=bt;out->used_budget=bc;if(dp[bestidx].take)for(int i=0;i<n;i++)if(dp[bestidx].take[i])out->selected[out->count++]=i;
 for(size_t q=0;q<total;q++)free(dp[q].take);free(dp);return 1;
}
int tw_bruteforce(const Attraction *items,int n,int T,int B,Plan *out){
 if(!items||!out||n<0||n>22||T<0||B<0)return 0;int best=-1;memset(out,0,sizeof(*out));
 unsigned long long lim=1ULL<<n;for(unsigned long long mask=0;mask<lim;mask++){int t=0,c=0,r=0;for(int i=0;i<n;i++)if(mask&(1ULL<<i)){t+=items[i].duration;c+=items[i].cost;r+=items[i].rating;}
 if(t<=T&&c<=B&&r>best){best=r;out->rating=r;out->used_time=t;out->used_budget=c;out->count=0;for(int i=0;i<n;i++)if(mask&(1ULL<<i))out->selected[out->count++]=i;}}
 return 1;
}
