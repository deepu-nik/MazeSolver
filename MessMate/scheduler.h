#ifndef MESSMATE_SCHEDULER_H
#define MESSMATE_SCHEDULER_H
#define MM_MAX_STUDENTS 100
#define MM_MAX_SLOTS 200
typedef struct { char name[64]; int from_min; int until_min; int arrival_order; } Student;
typedef struct { int student_index; int slot_start; } Assignment;
typedef struct { Assignment items[MM_MAX_STUDENTS]; int count; int unserved[MM_MAX_STUDENTS]; int unserved_count; int slot_count; } Schedule;
int mm_build_schedule(const Student *students,int n,int opens,int closes,int duration,int step,Schedule *out);
int mm_fcfs_schedule(const Student *students,int n,int opens,int closes,int duration,int step,Schedule *out);
#endif
