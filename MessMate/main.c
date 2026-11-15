#include "raylib.h"
#include "scheduler.h"
#include <stdio.h>
#include <string.h>

#define W 1280
#define H 900
static const Color BG={239,244,249,255}, NAVY={24,43,68,255}, INK={35,51,70,255}, MUTED={101,119,139,255}, BLUE={47,105,173,255}, PALE={226,236,247,255}, WHITE={255,255,255,255}, GREEN={28,132,93,255}, AMBER={183,116,31,255}, LINE={214,224,234,255};
static Student students[MM_MAX_STUDENTS]; static int n=0,duration=10,step=5,opens=690,closes=840;
static char namebuf[64]="",frombuf[8]="11:30",untilbuf[8]="12:30",msg[256]="Add students or load the sample dataset.";
static int focus=0,has_result=0; static Schedule best,fcfs;
static int parse_time(const char*s){int h,m;if(sscanf(s,"%d:%d",&h,&m)!=2||h<0||h>23||m<0||m>59)return -1;return h*60+m;}
static void time_text(int t,char*b){sprintf(b,"%02d:%02d",t/60,t%60);}
static int hit(Rectangle r){return IsMouseButtonPressed(MOUSE_LEFT_BUTTON)&&CheckCollisionPointRec(GetMousePosition(),r);}
static void panel(Rectangle r){DrawRectangleRounded(r,0.08f,12,WHITE);DrawRectangleRoundedLines(r,0.08f,12,LINE);}
static void label(const char*s,int x,int y,int size,Color c){DrawText(s,x,y,size,c);}
static void button(Rectangle r,const char*s,int primary){Color c=CheckCollisionPointRec(GetMousePosition(),r)?(Color){63,119,184,255}:(primary?BLUE:PALE);DrawRectangleRounded(r,0.18f,10,c);DrawText(s,(int)r.x+16,(int)r.y+13,19,primary?WHITE:NAVY);}
static int btn(Rectangle r,const char*s,int primary){button(r,s,primary);return hit(r);}
static void field(Rectangle r,const char*title,const char*value,int active){label(title,(int)r.x,(int)r.y-29,17,MUTED);DrawRectangleRounded(r,0.12f,8,WHITE);DrawRectangleRoundedLines(r,0.12f,8,active?BLUE:LINE);label(value,(int)r.x+13,(int)r.y+12,21,INK);}
static void add_student(void){int a=parse_time(frombuf),b=parse_time(untilbuf);if(!namebuf[0]||a<0||b<=a||n>=MM_MAX_STUDENTS){strcpy(msg,"Check the name and HH:MM availability window (maximum 100 students).");return;}for(int i=0;i<n;i++)if(!strcmp(students[i].name,namebuf)){strcpy(msg,"Student names must be unique.");return;}strncpy(students[n].name,namebuf,63);students[n].from_min=a;students[n].until_min=b;students[n].arrival_order=n;n++;namebuf[0]=0;has_result=0;strcpy(msg,"Student added successfully.");}
static void load_sample(void){const char*names[]={"Aarav","Diya","Kabir","Mira","Rohan","Sara","Ishaan"};const char*a[]={"11:30","11:30","12:00","12:00","12:30","11:45","12:15"};const char*b[]={"12:00","12:30","12:30","13:00","13:00","12:15","13:15"};n=7;for(int i=0;i<n;i++){strcpy(students[i].name,names[i]);students[i].from_min=parse_time(a[i]);students[i].until_min=parse_time(b[i]);students[i].arrival_order=i;}has_result=0;strcpy(msg,"Sample dataset loaded.");}
int main(void){SetConfigFlags(FLAG_WINDOW_RESIZABLE);InitWindow(W,H,"MessMate | College Mess Scheduler");SetTargetFPS(60);
while(!WindowShouldClose()){
 if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){Vector2 p=GetMousePosition();if(CheckCollisionPointRec(p,(Rectangle){40,190,300,54}))focus=0;else if(CheckCollisionPointRec(p,(Rectangle){365,190,220,54}))focus=1;else if(CheckCollisionPointRec(p,(Rectangle){610,190,220,54}))focus=2;}
 if(IsKeyPressed(KEY_TAB))focus=(focus+1)%3;
 if(IsKeyPressed(KEY_BACKSPACE)){char*p=focus==0?namebuf:focus==1?frombuf:untilbuf;size_t l=strlen(p);if(l)p[l-1]=0;}
 int ch=GetCharPressed();while(ch>0){if(ch>=32&&ch<127){char*p=focus==0?namebuf:focus==1?frombuf:untilbuf;int max=focus==0?63:7;size_t l=strlen(p);if((int)l<max){p[l]=(char)ch;p[l+1]=0;}}ch=GetCharPressed();}
 if(btn((Rectangle){900,190,150,54},"Add student",1))add_student();
 if(btn((Rectangle){1065,190,175,54},"Load sample",0))load_sample();
 if(btn((Rectangle){900,260,150,48},"Remove last",0)&&n){n--;has_result=0;strcpy(msg,"Last student removed.");}
 if(btn((Rectangle){1065,260,175,48},"Clear all",0)){n=0;has_result=0;strcpy(msg,"Student list cleared.");}
 if(btn((Rectangle){900,324,340,54},"Optimize schedule",1)){if(mm_build_schedule(students,n,opens,closes,duration,step,&best)&&mm_fcfs_schedule(students,n,opens,closes,duration,step,&fcfs)){has_result=1;strcpy(msg,"Maximum matching schedule calculated.");}else strcpy(msg,"Unable to calculate. Check the input data.");}
 BeginDrawing();ClearBackground(BG);
 DrawRectangle(0,0, W,118,NAVY);label("MESSMATE",40,25,34,WHITE);label("College Mess Meal Scheduler  /  Maximum Bipartite Matching",42,72,19,(Color){203,218,235,255});label("DAA MINI PROJECT",1050,43,16,(Color){203,218,235,255});
 panel((Rectangle){32,142,1216,250});label("1. STUDENT INPUT",55,160,23,NAVY);label("Enter each student's available time window.",55,190,17,MUTED);
 field((Rectangle){40,235,300,54},"Student name",namebuf,focus==0);field((Rectangle){365,235,220,54},"Available from (HH:MM)",frombuf,focus==1);field((Rectangle){610,235,220,54},"Available until (HH:MM)",untilbuf,focus==2);
 label("Tip: click a field to type. Use TAB to move between fields.",55,320,16,MUTED);label(TextFormat("%d / %d students added  •  %d-minute service  •  %d-minute slot interval",n,MM_MAX_STUDENTS,duration,step),55,354,18,INK);
 panel((Rectangle){32,414,1216,248});label("2. STUDENT REGISTER",55,432,23,NAVY);label("STUDENT",60,477,15,MUTED);label("AVAILABLE WINDOW",440,477,15,MUTED);label("NO.",1120,477,15,MUTED);DrawLine(55,503,1225,503,LINE);
 for(int i=0;i<n&&i<5;i++){char a[8],b[8];time_text(students[i].from_min,a);time_text(students[i].until_min,b);int y=514+i*27;label(students[i].name,60,y,19,INK);label(TextFormat("%s  —  %s",a,b),440,y,19,MUTED);label(TextFormat("%02d",i+1),1120,y,18,MUTED);if(i<4)DrawLine(55,y+25,1225,y+25,(Color){235,240,245,255});}
 if(n>5)label(TextFormat("Showing first 5 records  •  %d additional students",n-5),60,635,15,MUTED);
 panel((Rectangle){32,684,1216,184});label("3. OPTIMIZED RESULTS",55,702,23,NAVY);
 if(has_result){DrawRectangleRounded((Rectangle){55,744,255,96},0.12f,10,(Color){229,244,237,255});label("MAXIMUM MATCHING",72,758,14,GREEN);label(TextFormat("%d served",best.count),72,784,28,GREEN);
 DrawRectangleRounded((Rectangle){325,744,255,96},0.12f,10,PALE);label("FCFS BASELINE",342,758,14,BLUE);label(TextFormat("%d served",fcfs.count),342,784,28,NAVY);
 DrawRectangleRounded((Rectangle){595,744,255,96},0.12f,10,(Color){250,241,224,255});label("IMPROVEMENT",612,758,14,AMBER);label(TextFormat("+%d students",best.count-fcfs.count),612,784,26,AMBER);
 label(TextFormat("Unserved: %d",best.unserved_count),890,758,18,INK);label("Exact optimum for the 5-minute slot model",890,790,16,MUTED);
 }else{label("Your optimized schedule will appear here.",60,758,20,MUTED);label("Add students or load the sample dataset, then select Optimize schedule.",60,794,17,MUTED);}
 DrawRectangle(0,878,W,22,NAVY);label(msg,42,879,14,WHITE);
 EndDrawing();}
CloseWindow();return 0;}