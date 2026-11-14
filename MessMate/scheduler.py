"""Exact maximum-cardinality mess scheduling using bipartite matching.

Time is discretized into fixed service slots. A student can be assigned to a
slot only when the entire service interval fits inside that student's window.
The matching is maximum-cardinality, so it serves as many students as possible.
"""
from dataclasses import dataclass
from typing import List, Dict, Tuple

@dataclass(frozen=True)
class Student:
    name: str
    available_from: int  # minutes after opening, inclusive
    available_until: int # minutes after opening, exclusive
    arrival_order: int = 0

def build_slots(open_minutes: int, close_minutes: int, service_minutes: int, step_minutes: int = 5):
    if service_minutes <= 0 or step_minutes <= 0 or close_minutes <= open_minutes:
        raise ValueError("Invalid opening hours, service duration, or slot step.")
    return list(range(open_minutes, close_minutes - service_minutes + 1, step_minutes))

def eligible_slots(student: Student, slots: List[int], service_minutes: int):
    return [s for s in slots if s >= student.available_from and s + service_minutes <= student.available_until]

def maximum_matching_schedule(students: List[Student], open_minutes: int, close_minutes: int,
                              service_minutes: int, step_minutes: int = 5):
    """Return (assignments, unserved, slot_count). Assignments map student name to start minute."""
    slots = build_slots(open_minutes, close_minutes, service_minutes, step_minutes)
    adjacency = [eligible_slots(st, slots, service_minutes) for st in students]
    slot_owner: Dict[int, int] = {}

    def augment(student_index, seen):
        for slot in adjacency[student_index]:
            if slot in seen:
                continue
            seen.add(slot)
            if slot not in slot_owner or augment(slot_owner[slot], seen):
                slot_owner[slot] = student_index
                return True
        return False

    # Deterministic order; augmenting paths can reassign earlier students when needed.
    for i in sorted(range(len(students)), key=lambda x: (len(adjacency[x]), students[x].arrival_order, x)):
        augment(i, set())

    assigned = {student_index: slot for slot, student_index in slot_owner.items()}
    assignments = {students[i].name: assigned[i] for i in assigned}
    unserved = [st.name for i, st in enumerate(students) if i not in assigned]
    return assignments, unserved, len(slots)

def fcfs_schedule(students: List[Student], open_minutes: int, close_minutes: int,
                  service_minutes: int, step_minutes: int = 5):
    """Baseline: process students by arrival_order and take their earliest free feasible slot."""
    slots = build_slots(open_minutes, close_minutes, service_minutes, step_minutes)
    free = set(slots)
    assignments = {}
    for st in sorted(students, key=lambda x: x.arrival_order):
        choices = [s for s in slots if s in free and s >= st.available_from and s + service_minutes <= st.available_until]
        if choices:
            chosen = choices[0]
            assignments[st.name] = chosen
            free.remove(chosen)
    unserved = [st.name for st in students if st.name not in assignments]
    return assignments, unserved

def format_time(minutes):
    return f"{minutes // 60:02d}:{minutes % 60:02d}"
