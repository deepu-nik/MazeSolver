import unittest
from optimizer import Attraction,optimize_attractions,brute_force_optimize

class OptimizerTests(unittest.TestCase):
    def setUp(self):
        self.items=[Attraction("A",120,500,9),Attraction("B",90,200,7),Attraction("C",60,100,6),Attraction("D",150,350,8)]
    def test_dp_matches_brute_force(self):
        dp=optimize_attractions(self.items,240,700)
        bf=brute_force_optimize(self.items,240,700)
        self.assertEqual(dp[1:],bf[1:])
    def test_both_constraints_apply(self):
        chosen,score,t,c=optimize_attractions(self.items,60,1000)
        self.assertLessEqual(t,60); self.assertLessEqual(c,1000)
        self.assertEqual(score,6)
    def test_zero_budget(self):
        items=[Attraction("Free",30,0,4),Attraction("Paid",30,1,9)]
        chosen,score,t,c=optimize_attractions(items,60,0)
        self.assertEqual([x.name for x in chosen],["Free"])
        self.assertEqual(c,0)

if __name__=="__main__": unittest.main()
