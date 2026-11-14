"""TripWise - two-constraint tourist attraction planner GUI."""
import tkinter as tk
from tkinter import ttk,messagebox
from optimizer import Attraction,optimize_attractions,brute_force_optimize

class TripWiseApp:
    def __init__(self,root):
        self.root=root; root.title("TripWise | Tourist Attraction Planner"); root.geometry("1000x700"); root.minsize(850,600)
        self.items=[]; self._build()

    def _build(self):
        ttk.Label(self.root,text="TripWise",font=("Arial",24,"bold")).pack(pady=(12,0))
        ttk.Label(self.root,text="Choose attractions for the highest total rating within time and budget").pack(pady=(0,12))
        form=ttk.LabelFrame(self.root,text="Add attraction")
        form.pack(fill="x",padx=14,pady=6)
        self.name=tk.StringVar(); self.duration=tk.StringVar(value="60"); self.cost=tk.StringVar(value="200"); self.rating=tk.StringVar(value="5")
        for col,(label,var,width) in enumerate([("Attraction",self.name,22),("Duration (min)",self.duration,12),("Entry cost (₹)",self.cost,12),("Rating points",self.rating,10)]):
            ttk.Label(form,text=label).grid(row=0,column=col,padx=8,pady=(8,2),sticky="w")
            ttk.Entry(form,textvariable=var,width=width).grid(row=1,column=col,padx=8,pady=(0,8),sticky="w")
        ttk.Button(form,text="Add",command=self.add).grid(row=1,column=4,padx=8)
        ttk.Button(form,text="Load sample",command=self.sample).grid(row=1,column=5,padx=8)
        ttk.Button(form,text="Remove selected",command=self.remove).grid(row=1,column=6,padx=8)
        limits=ttk.LabelFrame(self.root,text="Your trip limits")
        limits.pack(fill="x",padx=14,pady=6)
        self.time_limit=tk.StringVar(value="480"); self.budget_limit=tk.StringVar(value="3000")
        ttk.Label(limits,text="Available time (minutes)").grid(row=0,column=0,padx=8,pady=(7,2),sticky="w")
        ttk.Entry(limits,textvariable=self.time_limit,width=12).grid(row=1,column=0,padx=8,pady=(0,8))
        ttk.Label(limits,text="Total budget (₹)").grid(row=0,column=1,padx=8,pady=(7,2),sticky="w")
        ttk.Entry(limits,textvariable=self.budget_limit,width=12).grid(row=1,column=1,padx=8,pady=(0,8))
        ttk.Button(limits,text="Find best plan",command=self.optimize).grid(row=1,column=2,padx=12)
        ttk.Button(limits,text="Clear all",command=self.clear).grid(row=1,column=3,padx=8)
        self.table=ttk.Treeview(self.root,columns=("name","duration","cost","rating"),show="headings",height=9)
        for c,title,w in [("name","Attraction",260),("duration","Duration (min)",150),("cost","Entry cost (₹)",150),("rating","Rating points",140)]:
            self.table.heading(c,text=title); self.table.column(c,width=w,anchor="center")
        self.table.pack(fill="both",expand=True,padx=14,pady=8)
        result=ttk.LabelFrame(self.root,text="Recommended itinerary")
        result.pack(fill="both",expand=True,padx=14,pady=6)
        self.output=tk.Text(result,height=10,wrap="word",font=("Consolas",10)); self.output.pack(fill="both",expand=True,padx=8,pady=8)
        self.status=ttk.Label(self.root,text="Add attractions or load the sample dataset.",anchor="w"); self.status.pack(fill="x",padx=16,pady=(0,10))

    def add(self):
        try:
            name=self.name.get().strip(); d,c,r=int(self.duration.get()),int(self.cost.get()),int(self.rating.get())
            if not name: raise ValueError("Enter an attraction name.")
            if d<=0 or c<0 or r<0: raise ValueError("Duration must be positive; cost and rating cannot be negative.")
            if any(x.name.casefold()==name.casefold() for x in self.items): raise ValueError("Attraction names must be unique.")
            self.items.append(Attraction(name,d,c,r)); self.refresh(); self.name.set("")
        except ValueError as e: messagebox.showerror("Invalid input",str(e))

    def refresh(self):
        for row in self.table.get_children(): self.table.delete(row)
        for x in self.items: self.table.insert("","end",values=(x.name,x.duration,x.cost,x.rating))

    def remove(self):
        selected=self.table.selection()
        names={self.table.item(i,"values")[0] for i in selected}
        self.items=[x for x in self.items if x.name not in names]; self.refresh()

    def sample(self):
        data=[("Fort",120,500,9),("Museum",90,200,7),("Garden",60,100,6),("Aquarium",120,700,9),("Temple",45,0,6),("Science Centre",150,350,8),("Lake Boating",60,300,7),("Art Gallery",75,250,7)]
        self.items=[Attraction(*x) for x in data]; self.refresh(); self.status.config(text="Sample dataset loaded.")

    def clear(self):
        self.items=[]; self.refresh(); self.output.delete("1.0","end"); self.status.config(text="All attractions cleared.")

    def optimize(self):
        try:
            if not self.items: raise ValueError("Add at least one attraction.")
            t,b=int(self.time_limit.get()),int(self.budget_limit.get())
            if t<0 or b<0: raise ValueError("Limits cannot be negative.")
            chosen,score,used_t,used_b=optimize_attractions(self.items,t,b)
            lines=["OPTIMAL SELECTION (2-CONSTRAINT 0/1 KNAPSACK)","="*52]
            lines += [f"• {x.name} — {x.duration} min — ₹{x.cost} — rating {x.rating}" for x in chosen] or ["No attraction fits the supplied limits."]
            lines += ["",f"Total rating: {score}",f"Time used: {used_t}/{t} minutes",f"Budget used: ₹{used_b}/₹{b}"]
            if len(self.items)<=22:
                bf,br,bt,bc=brute_force_optimize(self.items,t,b)
                lines += ["","BRUTE-FORCE VALIDATION", "-"*52,f"Brute-force rating: {br}",f"DP matches brute force: {'YES' if br==score else 'NO'}"]
            else: lines += ["","Brute-force validation skipped (more than 22 attractions)."]
            lines += ["","Model assumptions: ratings are additive; attractions are independent and each can be selected at most once. Travel time, opening hours and route order are not included."]
            self.output.delete("1.0","end"); self.output.insert("1.0","\n".join(lines))
            self.status.config(text="Optimization complete.")
        except (ValueError,TypeError) as e: messagebox.showerror("Cannot optimize",str(e))

if __name__=="__main__":
    root=tk.Tk(); TripWiseApp(root); root.mainloop()
