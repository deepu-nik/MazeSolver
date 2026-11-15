#include "../scheduler.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
int main(void){
 Student s[2]={{"Flexible",0,20,0},{"Constrained",0,10,1}};Schedule a,b;
 assert(mm_build_schedule(s,2,0,20,10,10,&a));assert(a.count==2);
 assert(mm_fcfs_schedule(s,2,0,20,10,10,&b));assert(b.count==1);
 Student x[1]={{"No fit",0,5,0}};assert(mm_build_schedule(x,1,0,20,10,10,&a));assert(a.count==0&&a.unserved_count==1);
 puts("MessMate C algorithm tests: PASS");return 0;
}
