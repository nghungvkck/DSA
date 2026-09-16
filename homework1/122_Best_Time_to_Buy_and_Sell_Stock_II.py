from typing import List
class Solution:
    def maxProfit(self, prices: List[int]) -> int:
        count  = 0;
        for i in range(0, prices.__len__()-1):
            if prices[i] <= prices[i+1]:
                count += prices[i+1] - prices[i]
        return count;