#include "raylib.h"
#include "optimizer.h"
#include <stdio.h>
#include <string.h>
#define W 1100
#define H 760
static Attraction items[TW_MAX_ITEMS];static int n=0,T=480,B=3000;static char namebuf[64]="",dur[12]="60",cost[12]="200",rating[12]="5",tbuf[12]="480",bbuf[12]="3000",status[256]="Add attractions or load the sample dataset.";static int focus=0,has=0;static Plan plan,brute;
static void add(void){int d,c,r;if(!namebuf[0]||sscanf(dur,"%d",&d)!=1||sscanf(cost,"%d",&c)!=1||sscanf(rating,"%d",&r)!=1||d<=0||c<0||r<0||n>=TW_MAX_ITEMS){strcpy(status,"Invalid data: duration > 0, cost/rating >= 0, max 40.");return;}for(int i=0;i<n;i++)if(!strcmp(items[i].name,namebuf)){strcpy(status,"Attraction names must be unique.");return;}strncpy(items[n].name,namebuf,63);items[n].duration=d;items[n].cost=c;items[n].rating=r;n++;namebuf[0]=0;has=0;strcpy(status,"Attraction added.");}
static void sample(void){const char *names[]={"Fort","Museum","Garden","Aquarium","Temple","Science Centre","Lake Boating","Art Gallery"};int d[]={120,90,60,120,45,150,60,75},c[]={500,200,100,700,0,350,300,250},r[]={9,7,6,9,6,8,7,7};n=8;for(int i=0;i<n;i++){strcpy(items[i].name,names[i]);items[i].duration=d[i];items[i].cost=c[i];items[i].rating=r[i];}has=0;strcpy(status,"Sample attractions loaded.");}
static int btn(Rectangle r,const char *s){DrawRectangleRounded(r,.16f,8,CheckCollisionPointRec(GetMousePosition(),r)?(Color){65,110,175,255}:(Color){43,76,125,255});DrawText(s,r.x+12,r.y+10,18,RAYWHITE);return IsMouseButtonPressed(MOUSE_LEFT_BUTTON)&&CheckCollisionPointRec(GetMousePosition(),r);}
int main(void){SetConfigFlags(FLAG_WINDOW_RESIZABLE);InitWindow(W,H,"TripWise | Tourist Attraction Planner");SetTargetFPS(60);
while(!WindowShouldClose()){
 if(IsKeyPressed(KEY_TAB))focus=(focus+1)%6;if(IsKeyPressed(KEY_BACKSPACE)){char *fields[]={namebuf,dur,cost,rating,tbuf,bbuf};char *p=fields[focus];size_t l=strlen(p);if(l)p[l-1]=0;}
 int ch=GetCharPressed();while(ch>0){if(ch>=32&&ch<127){char *fields[]={namebuf,dur,cost,rating,tbuf,bbuf};char *p=fields[focus];int max=focus==0?63:11;size_t l=strlen(p);if((int)l<max){p[l]=ch;p[l+1]=0;}}ch=GetCharPressed();}
 if(btn((Rectangle){850,110,180,42},"Add attraction"))add();if(btn((Rectangle){850,160,180,42},"Load sample"))sample();if(btn((Rectangle){850,210,180,42},"Remove last")&&n){n--;has=0;strcpy(status,"Last attraction removed.");}
 if(btn((Rectangle){850,260,180,42},"Optimize")){int parsedT=0,parsedB=0;if(sscanf(tbuf,"%d",&parsedT)!=1||sscanf(bbuf,"%d",&parsedB)!=1||parsedT<0||parsedB<0){strcpy(status,"Enter non-negative numeric time and budget limits.");}else{T=parsedT;B=parsedB;if(tw_optimize(items,n,T,B,&plan)){has=1;if(n<=22&&tw_bruteforce(items,n,T,B,&brute)&&brute.rating!=plan.rating)strcpy(status,"Warning: DP and brute force differ.");else strcpy(status,"Optimal selection calculated.");}else strcpy(status,"Invalid limits, data, or memory cap exceeded.");}}
 if(btn((Rectangle){850,310,180,42},"Clear all")){n=0;has=0;strcpy(status,"All attractions cleared.");}
 BeginDrawing();ClearBackground((Color){18,24,36,255});DrawText("TripWise",30,20,36,RAYWHITE);DrawText("Tourist Attraction Planner | C + Raylib",32,64,18,(Color){170,190,215,255});
 DrawText("ATTRACTION INPUT",32,112,18,(Color){130,190,255,255});const char *labels[]={"Attraction","Duration min","Entry cost Rs","Rating points"};char *fields[]={namebuf,dur,cost,rating};int xs[]={32,235,430,625},ws[]={180,170,170,170};
 for(int i=0;i<4;i++){DrawText(labels[i],xs[i],151,15,LIGHTGRAY);DrawRectangleLines(xs[i],174,ws[i],38,focus==i?SKYBLUE:GRAY);DrawText(fields[i],xs[i]+8,183,18,RAYWHITE);}
 DrawText("Click a field to type; TAB switches fields.",32,220,14,GRAY);DrawText(TextFormat("Attractions: %d / %d",n,TW_MAX_ITEMS),32,255,17,RAYWHITE);DrawText("YOUR LIMITS",32,290,18,(Color){130,190,255,255});
 DrawText(TextFormat("Available time: %d minutes",T),32,320,17,LIGHTGRAY);DrawText(TextFormat("Budget: Rs %d",B),350,320,17,LIGHTGRAY);
 DrawText("ATTRACTION LIST",32,390,18,(Color){130,190,255,255});DrawRectangle(32,418,770,130,(Color){28,37,53,255});
 for(int i=0;i<n&&i<6;i++)DrawText(TextFormat("%-18s %4d min   Rs %-5d   rating %d",items[i].name,items[i].duration,items[i].cost,items[i].rating),48,430+i*19,14,LIGHTGRAY);
 DrawText("OPTIMIZED PLAN",32,565,18,(Color){130,190,255,255});
 if(has){DrawText(TextFormat("Rating: %d   Time: %d/%d min   Budget: Rs %d/%d",plan.rating,plan.used_time,T,plan.used_budget,B),32,594,18,GREEN);for(int i=0;i<plan.count&&i<4;i++)DrawText(items[plan.selected[i]].name,48,622+i*22,15,RAYWHITE);if(n<=22)DrawText(TextFormat("Brute force rating: %d | DP matches: %s",brute.rating,brute.rating==plan.rating?"YES":"NO"),520,622,15,brute.rating==plan.rating?GREEN:RED);
 }else DrawText("Click Optimize to find the best combination under both limits.",32,600,16,GRAY);
 DrawText(status,32,H-34,16,(Color){220,205,140,255});EndDrawing();}
CloseWindow();return 0;}
