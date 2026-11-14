#ifndef TRIPWISE_OPTIMIZER_H
#define TRIPWISE_OPTIMIZER_H
#define TW_MAX_ITEMS 40
typedef struct { char name[64]; int duration; int cost; int rating; } Attraction;
typedef struct { int selected[TW_MAX_ITEMS]; int count; int rating; int used_time; int used_budget; } Plan;
int tw_optimize(const Attraction *items,int n,int time_limit,int budget_limit,Plan *out);
int tw_bruteforce(const Attraction *items,int n,int time_limit,int budget_limit,Plan *out);
#endif
