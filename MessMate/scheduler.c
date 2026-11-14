#include "scheduler.h"
#include <string.h>
static int slots[MM_MAX_SLOTS], slot_count;
static int edges[MM_MAX_STUDENTS][MM_MAX_SLOTS];
static int owner[MM_MAX_SLOTS], seen[MM_MAX_SLOTS];
static const Student *g_students;
static int g_n;
static int augment(int u) {
    for(int s=0;s<slot_count;s++) if(edges[u][s] && !seen[s]) {
        seen[s]=1;
        if(owner[s]<0 || augment(owner[s])) { owner[s]=u; return 1; }
    }
    return 0;
}
static int make_slots(int opens,int closes,int duration,int step) {
    if(duration<=0 || step<=0 || closes<=opens) return -1;
    int n=0;
    for(int t=opens;t+duration<=closes;t+=step) {
        if(n>=MM_MAX_SLOTS) return -1;
        slots[n++]=t;
    }
    return n;
}
int mm_build_schedule(const Student *students,int n,int opens,int closes,int duration,int step,Schedule *out) {
    if(!students||!out||n<0||n>MM_MAX_STUDENTS) return 0;
    slot_count=make_slots(opens,closes,duration,step); if(slot_count<0) return 0;
    g_students=students; g_n=n; memset(edges,0,sizeof(edges));
    for(int i=0;i<n;i++) for(int s=0;s<slot_count;s++)
        if(slots[s]>=students[i].from_min && slots[s]+duration<=students[i].until_min) edges[i][s]=1;
    for(int s=0;s<slot_count;s++) owner[s]=-1;
    int order[MM_MAX_STUDENTS], degree[MM_MAX_STUDENTS];
    for(int i=0;i<n;i++){order[i]=i;degree[i]=0;for(int s=0;s<slot_count;s++)degree[i]+=edges[i][s];}
    for(int i=0;i<n;i++) for(int j=i+1;j<n;j++)
        if(degree[order[j]]<degree[order[i]] || (degree[order[j]]==degree[order[i]] && students[order[j]].arrival_order<students[order[i]].arrival_order)){int x=order[i];order[i]=order[j];order[j]=x;}
    for(int k=0;k<n;k++){memset(seen,0,sizeof(seen));augment(order[k]);}
    memset(out,0,sizeof(*out)); out->slot_count=slot_count;
    for(int s=0;s<slot_count;s++) if(owner[s]>=0){out->items[out->count].student_index=owner[s];out->items[out->count++].slot_start=slots[s];}
    for(int i=0;i<n;i++){int found=0;for(int a=0;a<out->count;a++)if(out->items[a].student_index==i)found=1;if(!found)out->unserved[out->unserved_count++]=i;}
    for(int i=0;i<out->count;i++)for(int j=i+1;j<out->count;j++)if(out->items[j].slot_start<out->items[i].slot_start){Assignment x=out->items[i];out->items[i]=out->items[j];out->items[j]=x;}
    return 1;
}
int mm_fcfs_schedule(const Student *students,int n,int opens,int closes,int duration,int step,Schedule *out) {
    if(!students||!out||n<0||n>MM_MAX_STUDENTS)return 0;
    int count=make_slots(opens,closes,duration,step);if(count<0)return 0;
    int used[MM_MAX_SLOTS]={0},order[MM_MAX_STUDENTS];
    for(int i=0;i<n;i++)order[i]=i;
    for(int i=0;i<n;i++)for(int j=i+1;j<n;j++)if(students[order[j]].arrival_order<students[order[i]].arrival_order){int x=order[i];order[i]=order[j];order[j]=x;}
    memset(out,0,sizeof(*out));out->slot_count=count;
    for(int k=0;k<n;k++){int i=order[k];for(int s=0;s<count;s++)if(!used[s]&&slots[s]>=students[i].from_min&&slots[s]+duration<=students[i].until_min){used[s]=1;out->items[out->count++]=(Assignment){i,slots[s]};break;}}
    for(int i=0;i<n;i++){int found=0;for(int a=0;a<out->count;a++)if(out->items[a].student_index==i)found=1;if(!found)out->unserved[out->unserved_count++]=i;}
    return 1;
}
