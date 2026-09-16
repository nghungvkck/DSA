from typing import List
class Solution:
    def longestCommonPrefix(self, strs: List[str]) -> str:
        result = ""
        length = float("inf");

        for x in strs:
            if len(x) <= length: 
                length = len(x)
        for i in range(0, length, 1):
            for j in range(0, len(strs) -1 , 1):
                if strs[j][i] != strs[j+1][i]:
                    return result
            result += strs[0][i]
        return result


def main():
    pass

if __name__ == "main":
    main()