#include "../optimizer.h"
#include <assert.h>
#include <stdio.h>
int main(void){Attraction a[4]={{"A",120,500,9},{"B",90,200,7},{"C",60,100,6},{"D",150,350,8}};Plan dp,bf;assert(tw_optimize(a,4,240,700,&dp));assert(tw_bruteforce(a,4,240,700,&bf));assert(dp.rating==bf.rating);Attraction x[2]={{"Free",30,0,4},{"Paid",30,1,9}};assert(tw_optimize(x,2,60,0,&dp));assert(dp.rating==4&&dp.used_budget==0);puts("TripWise C algorithm tests: PASS");return 0;}
