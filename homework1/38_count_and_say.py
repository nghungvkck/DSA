class Solution:
    def __init__(self):
        self.result = "1"

    def countAndSay(self, n: int) -> str:
        if n== 1 : return "1"
        count  = 1


        while count < n:
            result  = self.RLE(result)
            count+=1
        return result

    def RLE(self,  strs):
        test = ""
        for i in range(0, len(strs)):
            s = strs[i]
            count = 0
            for j in range(i , len(strs)):
                if s!= strs[j]:
                    break
                count+=1
            test = test + str(count) + s
            i += count - 1
        return test