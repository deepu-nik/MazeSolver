import unittest
from scheduler import Student, maximum_matching_schedule, fcfs_schedule

class SchedulerTests(unittest.TestCase):
    def test_maximum_matching_can_reassign_earlier_choice(self):
        students=[Student("A",0,20,1),Student("B",0,10,2)]
        assignments,missed,_=maximum_matching_schedule(students,0,20,10,10)
        self.assertEqual(len(assignments),2)
        self.assertEqual(set(assignments),{"A","B"})
        self.assertEqual(missed,[])

    def test_unavailable_student_is_not_scheduled(self):
        students=[Student("A",0,5,1)]
        assignments,missed,_=maximum_matching_schedule(students,0,20,10,10)
        self.assertEqual(assignments,{})
        self.assertEqual(missed,["A"])

    def test_fcfs_baseline(self):
        students=[Student("A",0,20,1),Student("B",0,10,2)]
        assignments,_=fcfs_schedule(students,0,20,10,10)
        self.assertEqual(len(assignments),1)

if __name__=="__main__": unittest.main()
