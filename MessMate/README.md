# MessMate — College Mess Meal Scheduler

A Python/Tkinter desktop GUI that schedules the maximum possible number of students at one serving counter within their personal meal-availability windows.

## DAA strategy
The application uses **maximum bipartite matching (Graph Algorithms)**. Time is divided into configurable fixed-length service slots. A student is adjacent to a slot only if the complete service interval fits inside that student's availability window. An augmenting-path matcher finds a maximum-cardinality matching.

This is exact for the discretized-slot model implemented here. It assumes one counter, a common fixed service duration, and service starts on the selected slot grid. It does not claim to solve arbitrary continuous-time scheduling or different service durations.

## Run
Python 3.10+ recommended. Tkinter is included with most standard Python installations.

macOS / Windows:
```bash
cd MessMate
python app.py
```
On some macOS installations use `python3 app.py`.

## Tests
```bash
python -m unittest discover -s tests -v
```

## Included
- Add/remove student availability windows
- Configurable opening/closing time, service duration and slot interval
- Optimized schedule and unserved list
- FCFS baseline comparison
- Sample dataset and input validation
