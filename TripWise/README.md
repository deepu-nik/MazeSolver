# TripWise — Tourist Attraction Planner

A Python/Tkinter desktop application that selects attractions with the maximum total rating under both a time limit and a budget limit.

## DAA strategy
**Dynamic Programming: 0/1 Knapsack with two constraints.** Each attraction can be selected at most once. The DP tracks reachable (used-time, used-budget) states and the best rating for each state. A brute-force subset enumeration is included as a validation baseline for up to 22 attractions.

The model assumes additive ratings and independent attractions. Travel time, route order, opening hours and subjective rating interactions are outside the current model.

## Run
Python 3.10+ recommended. Tkinter is included with most standard Python installations.

macOS / Windows:
```bash
cd TripWise
python app.py
```
On some macOS installations use `python3 app.py`.

## Tests
```bash
python -m unittest discover -s tests -v
```
