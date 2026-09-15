from typing import List
class Solution:
    # nếu muốn cả 2 đều giảm thì thêm dấu trừ phias trước
    # Vd - self.countBit1(x)
    # vd -x
    def sortByBits(self, arr: List[int]) -> List[int]:
        sorted(arr, key= lambda x :(
            self.countBit1(x), x
        ))

    def countBit1(a):
        count  = 0
        while a > 0:
            if a % 2 == 1: count += 1
            a = a / 2
        return count