from typing import List
class Solution:
    def __init__(self):
        self.phone = ["", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"]
        self.result = []
        self.path = ""

    def func(self, digits:str, index):
        # dk dung
        if index == len(digits):
            self.result.append(self.path);
            return

        for ch in self.phone[int(digits[index])]:
            self.path += ch
            self.func(digits, index +1)
            self.path = self.path[:-1]


    def letterCombinations(self, digits: str) -> List[str]:
        self.func(digits, 0)
        return self.result

