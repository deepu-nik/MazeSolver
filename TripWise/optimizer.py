"""TripWise: exact two-constraint 0/1 knapsack dynamic programming."""
from dataclasses import dataclass
from typing import List

@dataclass(frozen=True)
class Attraction:
    name: str
    duration: int
    cost: int
    rating: int

def optimize_attractions(items: List[Attraction], time_limit: int, budget_limit: int):
    """Return selected items and (rating, duration, cost). All resources are integer units.

    Sparse DP maps (used_time, used_cost) to (rating, tuple of selected indices).
    For identical resource use, retain the higher rating.
    """
    if time_limit < 0 or budget_limit < 0:
        raise ValueError("Time and budget limits cannot be negative.")
    for item in items:
        if not item.name.strip() or item.duration <= 0 or item.cost < 0 or item.rating < 0:
            raise ValueError("Attractions need a name, positive duration, and non-negative cost/rating.")
    states={(0,0):(0,())}
    for i,item in enumerate(items):
        updated=dict(states)
        for (used_time,used_cost),(score,chosen) in states.items():
            nt,nc=used_time+item.duration,used_cost+item.cost
            if nt<=time_limit and nc<=budget_limit:
                candidate=(score+item.rating,chosen+(i,))
                current=updated.get((nt,nc))
                if current is None or candidate[0]>current[0]:
                    updated[(nt,nc)]=candidate
        states=updated
    (used_time,used_cost),(score,chosen)=max(states.items(),key=lambda pair:(pair[1][0],-pair[0][0],-pair[0][1]))
    return [items[i] for i in chosen],score,used_time,used_cost

def brute_force_optimize(items: List[Attraction], time_limit: int, budget_limit: int):
    if len(items)>22:
        raise ValueError("Brute-force comparison is limited to 22 attractions.")
    best=(0,0,0,())
    n=len(items)
    for mask in range(1<<n):
        t=c=r=0; chosen=[]
        for i,item in enumerate(items):
            if mask & (1<<i):
                t+=item.duration; c+=item.cost; r+=item.rating; chosen.append(i)
        if t<=time_limit and c<=budget_limit and (r>best[0] or (r==best[0] and (t,c)<(best[1],best[2]))):
            best=(r,t,c,tuple(chosen))
    return [items[i] for i in best[3]],best[0],best[1],best[2]
