# TripWise — Tourist Attraction Planner
## DAA Mini Project Report Template | SY BTech CSE | AY 2026–27

> Complete the team and guide fields and insert screenshots/output captured from the final running application before printing.

## 1. Team Details
- Student 1: [Name] | PRN: [PRN]
- Student 2: [Name] | PRN: [PRN]
- Student 3: [Name] | PRN: [PRN]
- Faculty Guide: [Name]

## 2. Problem Statement
A tourist has a limited amount of time and a fixed budget. Each attraction has a visit duration, entry cost, and rating. The objective is to select a subset of attractions that maximizes the sum of ratings without exceeding either the total time limit or the total budget.

## 3. Objectives
- Add attractions with duration, cost and rating.
- Accept a total available-time limit and a budget limit.
- Select the best subset under both constraints simultaneously.
- Display selected attractions and total rating, time and cost.
- Validate the DP result against brute force for small datasets.

## 4. Existing / Basic Approach
A simple approach may choose attractions greedily by rating or rating-to-cost ratio. Such a local choice can miss a better combination because both time and money are limited. Dynamic Programming evaluates feasible resource-use states and retains the best rating for each state.

## 5. Selected DAA Strategy
**Dynamic Programming — 0/1 Knapsack with two constraints.**

Each attraction is an item that may be selected once or not selected. A DP state is identified by (used time, used budget) and stores the best total rating reachable using the attractions processed so far. For each attraction, states are copied (skip the attraction) and extended (take the attraction) only when both resource limits remain satisfied.

### Assumptions and scope
- Ratings are additive integer points.
- Attractions are independent and each can be selected at most once.
- Duration and entry cost are non-negative integer units; duration must be positive.
- Travel time between attractions, opening hours, route order, meal breaks and rating interactions are not modelled.
- The current implementation uses sparse reachable states, while the conventional upper-bound analysis below uses the full time-budget state grid.

## 6. Algorithm / Pseudocode
```text
DP = {(time=0, cost=0): (rating=0, chosen=empty)}
FOR each attraction i:
    NEXT = copy(DP)                    // skip attraction i
    FOR each state (t, c) in DP:
        nt = t + duration[i]
        nc = c + cost[i]
        IF nt <= time_limit AND nc <= budget_limit:
            candidate_rating = DP[t,c].rating + rating[i]
            UPDATE NEXT[nt,nc] if candidate is better
    DP = NEXT
RETURN feasible state with maximum rating
```

### Flowchart
```mermaid
flowchart TD
 A([Start]) --> B[Enter attractions, time limit and budget]
 B --> C[Initialize DP state (0,0)]
 C --> D[Process next attraction]
 D --> E[Copy states to represent skipping]
 E --> F[Add attraction to every feasible state]
 F --> G[Discard states exceeding either limit]
 G --> H{More attractions?}
 H -- Yes --> D
 H -- No --> I[Choose state with highest rating]
 I --> J[Display itinerary, time, cost and rating]
 J --> K[Compare with brute force when small]
 K --> L([End])
```

## 7. Implementation Details
- Language: Python 3
- GUI: Tkinter / ttk
- Algorithm module: `optimizer.py`
- GUI module: `app.py`
- Tests: `tests/test_optimizer.py`
- Inputs: attraction name, duration, cost, rating, total time limit, total budget.
- Outputs: selected attraction list, total rating, time used, budget used, brute-force validation.

## 8. Complexity Analysis
Let (n) be the number of attractions, (T) the time limit in integer units, and (B) the budget limit in integer units. A dense two-dimensional 0/1 knapsack DP has:
- Worst-case time: **O(nTB)**.
- Worst-case space: **O(TB)** when only the previous/current state layers are retained.

The program uses a sparse dictionary of reachable (time, cost) states. If (S_i) is the number of reachable states after processing item (i), its running time is **O(\sum_i S_i)** and memory is **O(max S_i)**, with (S_i le (T+1)(B+1)). Therefore the dense worst-case bounds remain O(nTB) time and O(TB) space.

The brute-force validator enumerates all subsets: **O(n·2^n)** time and **O(n)** auxiliary space (excluding the returned selected list). The GUI limits this comparison to at most 22 attractions.

## 9. Testing and Results
Run:
```bash
python -m unittest discover -s tests -v
```
Insert the actual test output and screenshots here:
- [Screenshot 1: attraction entry table]
- [Screenshot 2: optimized itinerary]
- [Screenshot 3: DP vs brute-force validation]
- [Test output: paste actual run]

## 10. Comparison
For the same small input, compare the DP total rating with brute-force enumeration of every subset. Record the input, both ratings, selected attractions, time used and cost used. Matching objective values validates the DP result for that test case.

## 11. Conclusion and Future Scope
TripWise demonstrates a two-resource optimization problem using Dynamic Programming. Future improvements may include travel-time matrices, route optimization, attraction opening hours, multi-day itineraries, user-defined rating weights and live attraction data.

## 12. References
- Course material / lecture notes: [Add exact course reference]
- Python documentation: https://docs.python.org/3/
- Tkinter documentation: https://docs.python.org/3/library/tkinter.html
- Dynamic Programming / 0/1 Knapsack reference used by the team: [Add textbook/author/edition and relevant chapter]
