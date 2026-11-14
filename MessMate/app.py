"""MessMate - GUI for maximum-cardinality college mess scheduling."""
import tkinter as tk
from tkinter import ttk, messagebox
from scheduler import Student, maximum_matching_schedule, fcfs_schedule, format_time

class MessMateApp:
    def __init__(self, root):
        self.root = root
        root.title("MessMate | College Mess Scheduler")
        root.geometry("1000x700")
        root.minsize(850, 600)
        self.students = []
        self._build()

    def _build(self):
        style = ttk.Style()
        try: style.theme_use("clam")
        except tk.TclError: pass
        ttk.Label(self.root, text="MessMate", font=("Arial", 24, "bold")).pack(pady=(12, 0))
        ttk.Label(self.root, text="Maximum students served within their available meal windows").pack(pady=(0, 12))
        form = ttk.LabelFrame(self.root, text="Student availability (24-hour HH:MM)")
        form.pack(fill="x", padx=14, pady=6)
        self.name = tk.StringVar()
        self.start = tk.StringVar(value="12:00")
        self.end = tk.StringVar(value="13:00")
        for col, (label, var, width) in enumerate([("Student name", self.name, 22), ("Available from", self.start, 12), ("Available until", self.end, 12)]):
            ttk.Label(form, text=label).grid(row=0, column=col, padx=8, pady=(8,2), sticky="w")
            ttk.Entry(form, textvariable=var, width=width).grid(row=1, column=col, padx=8, pady=(0,8), sticky="w")
        ttk.Button(form, text="Add student", command=self.add_student).grid(row=1, column=3, padx=8)
        ttk.Button(form, text="Load sample data", command=self.sample).grid(row=1, column=4, padx=8)
        ttk.Button(form, text="Remove selected", command=self.remove).grid(row=1, column=5, padx=8)
        settings = ttk.LabelFrame(self.root, text="Mess operating settings")
        settings.pack(fill="x", padx=14, pady=6)
        self.opening = tk.StringVar(value="11:30")
        self.closing = tk.StringVar(value="14:00")
        self.duration = tk.StringVar(value="10")
        self.step = tk.StringVar(value="5")
        for col, (label, var, width) in enumerate([("Opens",self.opening,10),("Closes",self.closing,10),("Service duration (min)",self.duration,8),("Slot interval (min)",self.step,8)]):
            ttk.Label(settings,text=label).grid(row=0,column=col,padx=8,pady=(7,2),sticky="w")
            ttk.Entry(settings,textvariable=var,width=width).grid(row=1,column=col,padx=8,pady=(0,8),sticky="w")
        ttk.Button(settings,text="Optimize schedule",command=self.optimize).grid(row=1,column=4,padx=12)
        ttk.Button(settings,text="Clear all",command=self.clear).grid(row=1,column=5,padx=8)
        self.table = ttk.Treeview(self.root, columns=("name","from","until","order"), show="headings", height=9)
        for c, title, w in [("name","Student",240),("from","Available from",150),("until","Available until",150),("order","Arrival order",120)]:
            self.table.heading(c,text=title); self.table.column(c,width=w,anchor="center")
        self.table.pack(fill="both",expand=True,padx=14,pady=8)
        results = ttk.LabelFrame(self.root,text="Optimization results")
        results.pack(fill="both",expand=True,padx=14,pady=6)
        self.output = tk.Text(results,height=10,wrap="word",font=("Consolas",10))
        self.output.pack(fill="both",expand=True,padx=8,pady=8)
        self.status = ttk.Label(self.root,text="Add students or load the sample dataset.",anchor="w")
        self.status.pack(fill="x",padx=16,pady=(0,10))

    @staticmethod
    def parse_time(value):
        parts=value.strip().split(":")
        if len(parts)!=2: raise ValueError("Use HH:MM time format.")
        h,m=map(int,parts)
        if not (0<=h<=23 and 0<=m<60): raise ValueError("Time must be between 00:00 and 23:59.")
        return h*60+m

    def add_student(self):
        name=self.name.get().strip()
        try:
            a,b=self.parse_time(self.start.get()),self.parse_time(self.end.get())
            if not name: raise ValueError("Enter a student name.")
            if b<=a: raise ValueError("Availability end must be after start.")
            if any(s.name.casefold()==name.casefold() for s in self.students): raise ValueError("Student names must be unique.")
            self.students.append(Student(name,a,b,len(self.students)+1))
            self.refresh()
            self.name.set("")
            self.status.config(text=f"Added {name}.")
        except ValueError as e: messagebox.showerror("Invalid input",str(e))

    def refresh(self):
        for item in self.table.get_children(): self.table.delete(item)
        for s in self.students:
            self.table.insert("", "end", values=(s.name,format_time(s.available_from),format_time(s.available_until),s.arrival_order))

    def remove(self):
        selected=self.table.selection()
        if not selected: return
        names={self.table.item(i,"values")[0] for i in selected}
        self.students=[s for s in self.students if s.name not in names]
        self.students=[Student(s.name,s.available_from,s.available_until,i+1) for i,s in enumerate(self.students)]
        self.refresh()

    def sample(self):
        data=[("Aarav","11:30","12:00"),("Diya","11:30","12:30"),("Kabir","12:00","12:30"),("Mira","12:00","13:00"),("Rohan","12:30","13:00"),("Sara","11:45","12:15"),("Ishaan","12:15","13:15")]
        self.students=[Student(n,self.parse_time(a),self.parse_time(b),i+1) for i,(n,a,b) in enumerate(data)]
        self.refresh()
        self.status.config(text="Sample dataset loaded.")

    def clear(self):
        self.students=[]; self.refresh(); self.output.delete("1.0","end")
        self.status.config(text="All student records cleared.")

    def optimize(self):
        try:
            if not self.students: raise ValueError("Add at least one student.")
            opening,closing=self.parse_time(self.opening.get()),self.parse_time(self.closing.get())
            duration,step=int(self.duration.get()),int(self.step.get())
            best,missed,slot_count=maximum_matching_schedule(self.students,opening,closing,duration,step)
            fcfs,fcfs_missed=fcfs_schedule(self.students,opening,closing,duration,step)
            lines=["MAXIMUM BIPARTITE MATCHING SCHEDULE", "="*46]
            for name,minute in sorted(best.items(),key=lambda x:x[1]): lines.append(f"{format_time(minute)} - {format_time(minute+duration)}   {name}")
            lines += ["",f"Served: {len(best)} / {len(self.students)}",f"Not scheduled: {', '.join(missed) if missed else 'None'}",
                      "", "FCFS BASELINE", "-"*46,f"Served: {len(fcfs)} / {len(self.students)}",f"Not scheduled: {', '.join(fcfs_missed) if fcfs_missed else 'None'}",
                      "",f"Matching result serves {len(best)-len(fcfs)} more student(s) than FCFS.",
                      "", "Method: each feasible service slot is a vertex; each student is connected to slots that fit their complete availability window. Augmenting paths find a maximum-cardinality matching."]
            self.output.delete("1.0","end"); self.output.insert("1.0","\n".join(lines))
            self.status.config(text=f"Optimization complete: {len(best)} students scheduled.")
        except (ValueError,TypeError) as e: messagebox.showerror("Cannot optimize",str(e))

if __name__=="__main__":
    root=tk.Tk(); MessMateApp(root); root.mainloop()
