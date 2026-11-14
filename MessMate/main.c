#include "raylib.h"
#include "scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define W 1100
#define H 760
static Student students[MM_MAX_STUDENTS]; static int n=0, duration=10, step=5, opens=690, closes=840;
static char namebuf[64]="", frombuf[8]="11:30", untilbuf[8]="12:30", msg[256]="Add students or load sample data.";
static int focus=0; static Schedule best,fcfs; static int has_result=0;
static int parse_time(const char *s){int h,m;if(sscanf(s,"%d:%d",&h,&m)!=2||h<0||h>23||m<0||m>59)return -1;return h*60+m;}
static void time_text(int t,char *b){sprintf(b,"%02d:%02d",t/60,t%60);}
static void draw_button(Rectangle r,const char *label,int enabled){DrawRectangleRounded(r,0.18f,8,enabled? (CheckCollisionPointRec(GetMousePosition(),r)?(Color){64,111,180,255}:(Color){43,76,125,255}):(Color){80,80,80,255});DrawText(label,(int)r.x+12,(int)r.y+10,18,RAYWHITE);}
static int button(Rectangle r,const char *label){draw_button(r,label,1);return IsMouseButtonPressed(MOUSE_LEFT_BUTTON)&&CheckCollisionPointRec(GetMousePosition(),r);}
static void add_student(void){int a=parse_time(frombuf),b=parse_time(untilbuf);if(!namebuf[0]||a<0||b<=a||n>=MM_MAX_STUDENTS){strcpy(msg,"Check name and HH:MM availability window (max 100 students).");return;}for(int i=0;i<n;i++)if(!strcmp(students[i].name,namebuf)){strcpy(msg,"Student names must be unique.");return;}strncpy(students[n].name,namebuf,63);students[n].from_min=a;students[n].until_min=b;students[n].arrival_order=n;n++;namebuf[0]=0;has_result=0;strcpy(msg,"Student added.");}
static void load_sample(void){const char *names[]={"Aarav","Diya","Kabir","Mira","Rohan","Sara","Ishaan"};const char *a[]={"11:30","11:30","12:00","12:00","12:30","11:45","12:15"};const char *b[]={"12:00","12:30","12:30","13:00","13:00","12:15","13:15"};n=7;for(int i=0;i<n;i++){strcpy(students[i].name,names[i]);students[i].from_min=parse_time(a[i]);students[i].until_min=parse_time(b[i]);students[i].arrival_order=i;}has_result=0;strcpy(msg,"Sample dataset loaded.");}
int main(void){SetConfigFlags(FLAG_WINDOW_RESIZABLE);InitWindow(W,H,"MessMate | College Mess Scheduler");SetTargetFPS(60);
while(!WindowShouldClose()){
 if(IsKeyPressed(KEY_TAB))focus=(focus+1)%3;
 if(IsKeyPressed(KEY_BACKSPACE)){char *p=focus==0?namebuf:focus==1?frombuf:untilbuf;size_t l=strlen(p);if(l)p[l-1]=0;}
 int ch=GetCharPressed();while(ch>0){if(ch>=32&&ch<127){char *p=focus==0?namebuf:focus==1?frombuf:untilbuf;int max=focus==0?63:7;size_t l=strlen(p);if((int)l<max){p[l]=(char)ch;p[l+1]=0;}}ch=GetCharPressed();}
 if(button((Rectangle){850,110,180,42},"Add student"))add_student();
 if(button((Rectangle){850,160,180,42},"Load sample"))load_sample();
 if(button((Rectangle){850,210,180,42},"Remove last")&&n>0){n--;has_result=0;strcpy(msg,"Last student removed.");}
 if(button((Rectangle){850,260,180,42},"Optimize")){if(mm_build_schedule(students,n,opens,closes,duration,step,&best)&&mm_fcfs_schedule(students,n,opens,closes,duration,step,&fcfs)){has_result=1;strcpy(msg,"Maximum matching schedule calculated.");}else strcpy(msg,"Invalid settings or too many time slots.");}
 if(button((Rectangle){850,310,180,42},"Clear all")){n=0;has_result=0;strcpy(msg,"All records cleared.");}
 BeginDrawing();ClearBackground((Color){18,24,36,255});DrawText("MessMate",30,20,36,RAYWHITE);DrawText("College Mess Meal Scheduler | C + Raylib",32,64,18,(Color){170,190,215,255});
 DrawText("STUDENT INPUT",32,112,18,(Color){130,190,255,255});
 DrawText("Name",32,151,16,LIGHTGRAY);DrawRectangleLines(32,174,235,38,focus==0?SKYBLUE:GRAY);DrawText(namebuf,42,183,18,RAYWHITE);
 DrawText("Available from (HH:MM)",285,151,16,LIGHTGRAY);DrawRectangleLines(285,174,210,38,focus==1?SKYBLUE:GRAY);DrawText(frombuf,295,183,18,RAYWHITE);
 DrawText("Available until (HH:MM)",510,151,16,LIGHTGRAY);DrawRectangleLines(510,174,210,38,focus==2?SKYBLUE:GRAY);DrawText(untilbuf,520,183,18,RAYWHITE);
 DrawText("Tip: click a field, type; press TAB to switch fields.",32,220,14,GRAY);
 DrawText(TextFormat("Students: %d / %d    |    Counter service: %d min    |    Slot step: %d min",n,MM_MAX_STUDENTS,duration,step),32,255,17,RAYWHITE);
 DrawText("STUDENT LIST",32,300,18,(Color){130,190,255,255});DrawRectangle(32,330,770,210,(Color){28,37,53,255});
 for(int i=0;i<n&&i<8;i++){char a[8],b[8];time_text(students[i].from_min,a);time_text(students[i].until_min,b);DrawText(TextFormat("%-3d %-18s %s - %s",i+1,students[i].name,a,b),48,342+i*23,16,LIGHTGRAY);}
 if(n>8)DrawText(TextFormat("... and %d more",n-8),48,525,14,GRAY);
 DrawText("RESULTS",32,565,18,(Color){130,190,255,255});
 if(has_result){DrawText(TextFormat("Maximum matching: %d served   |   FCFS: %d served   |   Improvement: %d",best.count,fcfs.count,best.count-fcfs.count),32,594,18,GREEN);
 for(int i=0;i<best.count&&i<5;i++){char t[8];time_text(best.items[i].slot_start,t);DrawText(TextFormat("%s  %s",t,students[best.items[i].student_index].name),48,622+i*22,15,RAYWHITE);}
 DrawText(TextFormat("Unserved by matching: %d",best.unserved_count),520,622,15,ORANGE);
 }else DrawText("Click Optimize to calculate a maximum-cardinality assignment.",32,600,16,GRAY);
 DrawText(msg,32,H-34,16,(Color){220,205,140,255});EndDrawing();
}
CloseWindow();return 0;}
