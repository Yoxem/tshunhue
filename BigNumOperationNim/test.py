#!/usr/bin/env python

def arrayize(n):
    if n == 0:
        return [0]
    else:
        result = []
        while n != 0:
            result.append(n % (2 ** 32))
            n = n // (2 ** 32)
        return result

arrayize(2 ** 32)

def numerize(arr : list):
    n = 0
    for i in range(len(arr)):
        n += arr[i] * ((2 ** 32) ** i)
    return n

test_suite = [[[4109920726, 1346167807, 9424],
              [137586114, 545115],
              [3972334612, 1345622692, 9424]],
              [[4086213910, 2929037583, 2244370610, 2948093978, 25680081],
              [993921090, 1587512372, 2060251786, 11882492, 6880319],
              [3092292820, 1341525211, 184118824, 2936211486, 18799762]],
              [[4086213915, 3195818255, 1049426845, 4172853648, 2935028599],
              [4086213909, 3195818255, 1049426845, 4172853648, 2935028599],
              [6]],
              [[0, 0, 0, 0, 1],
              [1],
              [4294967295, 4294967295, 4294967295, 4294967295]],
              [[4294967295, 4294967295, 4294967295, 4294967295],
              [4294967294, 4294967295, 4294967295, 4294967295],
              [1]],
              [[8],
              [7],
              [1]]]

y = [[arrayize(numerize(i[0])), arrayize(numerize(i[1])), arrayize(numerize(i[0])+numerize(i[1]))] for i in test_suite]

y.append([arrayize(1),arrayize(1),arrayize(2)])
y.append([arrayize(2 ** 32 - 1),arrayize(1),arrayize(2 ** 32)])
y.append([arrayize(2 ** 64 - 2),arrayize(3),arrayize(2 ** 64 + 1)])

print(y)