#include "raylib.h"
#include "optimizer.h"
#include <stdio.h>
#include <string.h>
#define W 1280
#define H 900
static const Color UI_BG={239,244,249,255},UI_NAVY={24,43,68,255},UI_INK={35,51,70,255},UI_MUTED={101,119,139,255},UI_BLUE={47,105,173,255},UI_PALE={226,236,247,255},UI_WHITE={255,255,255,255},UI_GREEN={28,132,93,255},UI_AMBER={183,116,31,255},UI_LINE={214,224,234,255};
static Attraction items[TW_MAX_ITEMS];static int n=0,T=480,B=3000;static char namebuf[64]="",dur[12]="60",cost[12]="200",rating[12]="5",tbuf[12]="480",bbuf[12]="3000",status[256]="Add attractions or load the sample dataset.";static int focus=0,has=0;static Plan plan,brute;
static int hit(Rectangle r){return IsMouseButtonPressed(MOUSE_LEFT_BUTTON)&&CheckCollisionPointRec(GetMousePosition(),r);}
static void label(const char*s,int x,int y,int size,Color c){DrawText(s,x,y,size,c);}
static void panel(Rectangle r){DrawRectangleRounded(r,.08f,12,UI_WHITE);DrawRectangleRoundedLines(r,.08f,12,UI_LINE);}
static void button(Rectangle r,const char*s,int primary){Color c=CheckCollisionPointRec(GetMousePosition(),r)?(Color){63,119,184,255}:(primary?UI_BLUE:UI_PALE);DrawRectangleRounded(r,.18f,10,c);label(s,(int)r.x+16,(int)r.y+13,19,primary?UI_WHITE:UI_NAVY);}
static int btn(Rectangle r,const char*s,int primary){button(r,s,primary);return hit(r);}
static void field(Rectangle r,const char*title,const char*value,int active){label(title,(int)r.x,(int)r.y-29,17,UI_MUTED);DrawRectangleRounded(r,.12f,8,UI_WHITE);DrawRectangleRoundedLines(r,.12f,8,active?UI_BLUE:UI_LINE);label(value,(int)r.x+13,(int)r.y+12,21,UI_INK);}
static void add(void){int d,c,r;if(!namebuf[0]||sscanf(dur,"%d",&d)!=1||sscanf(cost,"%d",&c)!=1||sscanf(rating,"%d",&r)!=1||d<=0||c<0||r<0||n>=TW_MAX_ITEMS){strcpy(status,"Invalid data: duration > 0, cost/rating >= 0, maximum 40.");return;}for(int i=0;i<n;i++)if(!strcmp(items[i].name,namebuf)){strcpy(status,"Attraction names must be unique.");return;}strncpy(items[n].name,namebuf,63);items[n].duration=d;items[n].cost=c;items[n].rating=r;n++;namebuf[0]=0;has=0;strcpy(status,"Attraction added successfully.");}
static void sample(void){const char*names[]={"Fort","Museum","Garden","Aquarium","Temple","Science Centre","Lake Boating","Art Gallery"};int d[]={120,90,60,120,45,150,60,75},c[]={500,200,100,700,0,350,300,250},r[]={9,7,6,9,6,8,7,7};n=8;for(int i=0;i<n;i++){strcpy(items[i].name,names[i]);items[i].duration=d[i];items[i].cost=c[i];items[i].rating=r[i];}has=0;strcpy(status,"Sample attractions loaded.");}
int main(void){SetConfigFlags(FLAG_WINDOW_RESIZABLE);InitWindow(W,H,"TripWise | Tourist Attraction Planner");SetTargetFPS(60);
while(!WindowShouldClose()){
 if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){Vector2 p=GetMousePosition();Rectangle f[]={{40,235,220,54},{285,235,150,54},{460,235,170,54},{655,235,170,54},{40,360,220,54},{285,360,220,54}};for(int i=0;i<6;i++)if(CheckCollisionPointRec(p,f[i]))focus=i;}
 if(IsKeyPressed(KEY_TAB))focus=(focus+1)%6;
 char*fields[]={namebuf,dur,cost,rating,tbuf,bbuf};
 if(IsKeyPressed(KEY_BACKSPACE)){size_t l=strlen(fields[focus]);if(l)fields[focus][l-1]=0;}
 int ch=GetCharPressed();while(ch>0){if(ch>=32&&ch<127){char*p=fields[focus];int max=focus==0?63:11;size_t l=strlen(p);if((int)l<max){p[l]=(char)ch;p[l+1]=0;}}ch=GetCharPressed();}
 if(btn((Rectangle){900,235,150,54},"Add attraction",1))add();
 if(btn((Rectangle){1065,235,175,54},"Load sample",0))sample();
 if(btn((Rectangle){900,305,150,48},"Remove last",0)&&n){n--;has=0;strcpy(status,"Last attraction removed.");}
 if(btn((Rectangle){1065,305,175,48},"Clear all",0)){n=0;has=0;strcpy(status,"Attraction list cleared.");}
 if(btn((Rectangle){900,369,340,54},"Optimize plan",1)){int pt=0,pb=0;if(sscanf(tbuf,"%d",&pt)!=1||sscanf(bbuf,"%d",&pb)!=1||pt<0||pb<0){strcpy(status,"Enter non-negative numeric time and budget limits.");}else{T=pt;B=pb;if(tw_optimize(items,n,T,B,&plan)){has=1;if(n<=22&&tw_bruteforce(items,n,T,B,&brute)&&brute.rating!=plan.rating)strcpy(status,"Warning: DP and brute-force results differ.");else strcpy(status,"Optimal selection calculated.");}else strcpy(status,"Invalid limits, data, or memory cap exceeded.");}}
 BeginDrawing();ClearBackground(UI_BG);DrawRectangle(0,0,W,118,UI_NAVY);label("TRIPWISE",40,25,34,UI_WHITE);label("Tourist Attraction Planner  /  Two-Constraint 0/1 Knapsack",42,72,19,(Color){203,218,235,255});label("DAA MINI PROJECT",1050,43,16,(Color){203,218,235,255});
 panel((Rectangle){32,142,1216,294});label("1. ATTRACTION DETAILS",55,160,23,UI_NAVY);label("Add places with their visit duration, entry cost and rating.",55,190,17,UI_MUTED);
 field((Rectangle){40,235,220,54},"Attraction name",namebuf,focus==0);field((Rectangle){285,235,150,54},"Duration (min)",dur,focus==1);field((Rectangle){460,235,170,54},"Entry cost (Rs)",cost,focus==2);field((Rectangle){655,235,170,54},"Rating points",rating,focus==3);
 label("Click a field to edit. Press TAB to switch fields.",55,311,16,UI_MUTED);
 label("YOUR PLANNING LIMITS",55,344,19,UI_NAVY);field((Rectangle){40,390,220,54},"Available time (minutes)",tbuf,focus==4);field((Rectangle){285,390,220,54},"Total budget (Rs)",bbuf,focus==5);
 label(TextFormat("%d / %d attractions added",n,TW_MAX_ITEMS),555,408,19,UI_INK);
 panel((Rectangle){32,456,1216,222});label("2. ATTRACTION REGISTER",55,474,23,UI_NAVY);label("ATTRACTION",60,518,15,UI_MUTED);label("DURATION",500,518,15,UI_MUTED);label("ENTRY COST",700,518,15,UI_MUTED);label("RATING",930,518,15,UI_MUTED);DrawLine(55,544,1225,544,UI_LINE);
 for(int i=0;i<n&&i<4;i++){int y=554+i*27;label(items[i].name,60,y,18,UI_INK);label(TextFormat("%d min",items[i].duration),500,y,18,UI_MUTED);label(TextFormat("Rs %d",items[i].cost),700,y,18,UI_MUTED);label(TextFormat("%d pts",items[i].rating),930,y,18,UI_MUTED);if(i<3)DrawLine(55,y+25,1225,y+25,(Color){235,240,245,255});}
 if(n>4)label(TextFormat("Showing first 4 records  •  %d additional attractions",n-4),60,664,15,UI_MUTED);
 panel((Rectangle){32,700,1216,168});label("3. OPTIMIZED ITINERARY",55,718,23,UI_NAVY);
 if(has){DrawRectangleRounded((Rectangle){55,758,235,88},.12f,10,(Color){229,244,237,255});label("TOTAL RATING",72,770,14,UI_GREEN);label(TextFormat("%d points",plan.rating),72,795,27,UI_GREEN);
 DrawRectangleRounded((Rectangle){305,758,235,88},.12f,10,UI_PALE);label("TIME USED",322,770,14,UI_BLUE);label(TextFormat("%d / %d min",plan.used_time,T),322,795,25,UI_NAVY);
 DrawRectangleRounded((Rectangle){555,758,235,88},.12f,10,(Color){250,241,224,255});label("BUDGET USED",572,770,14,UI_AMBER);label(TextFormat("Rs %d / %d",plan.used_budget,B),572,795,24,UI_AMBER);
 label("SELECTED",830,765,14,UI_MUTED);char selected[160]="";for(int i=0;i<plan.count;i++){const char*nm=items[plan.selected[i]].name;if(i)strcat(selected,", ");if(strlen(selected)+strlen(nm)<150)strcat(selected,nm);}label(selected[0]?selected:"No attractions selected",830,790,16,UI_INK);
 if(n<=22)label(TextFormat("Brute force validation: %s",brute.rating==plan.rating?"MATCH":"CHECK"),830,820,15,brute.rating==plan.rating?UI_GREEN:UI_AMBER);
 }else{label("Your best attraction combination will appear here.",60,766,20,UI_MUTED);label("Set your time and budget limits, then select Optimize plan.",60,802,17,UI_MUTED);}
 DrawRectangle(0,878,W,22,UI_NAVY);label(status,42,879,14,UI_WHITE);EndDrawing();}
CloseWindow();return 0;}