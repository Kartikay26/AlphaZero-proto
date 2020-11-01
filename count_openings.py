from collections import defaultdict
MOVES = 1
BATCH = 2000
lines = open("logfile.txt").readlines()
openings = set([l[:2] for l in lines])
counts = defaultdict(int)
countp = defaultdict(int)
# 012
# 345
# 678
firstMove = {
    "center": ["40", "41", "42", "43", "44", "45", "46", "47", "48", "49"],
    "corner": ["00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "60", "61", "62", "63", "64", "65", "66", "67", "68", "69"],
    "edge":   ["10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39"],
}
secondMove = {
    "center_corner": ["40", "42", "46", "48"],
    "center_edge":   ["41", "45", "47", "43"],
    "corner_center":       ["04", "24", "64", "84"],
    "corner_corner_opp":   ["08", "80", "26", "62"],
    "corner_corner_same":  ["06", "02", "20", "28", "86", "82", "60", "68"],
    "corner_kinghts_move": ["61", "81", "23", "83", "07", "27", "05", "65"],
    "corner_adj":          ["01", "03", "63", "67", "87", "85", "25", "21"],
    "edge_center":       ["14", "54", "74", "34"],
    "edge_opp":          ["17", "71", "35", "53"],
    "edge_adj":          ["10", "12", "30", "36", "76", "78", "58", "52"],
    "edge_diag":         ["13", "31", "37", "73", "57", "75", "51", "15"],
    "edge_kinghts_move": ["16", "18", "32", "38", "70", "72", "50", "56"],
}
result = defaultdict(int)
for o in firstMove:
    print("%s" % o, end="\t")
print(end="\t")
for o in secondMove:
    print("%s" % o, end="\t")
# print("OTHER")
print("\t","x wins","\t","o wins","\t","draw")
i = 0
for l in lines:
    counts[l[:2]] += 1
    result[l[-2]] += 1
    i += 1
    total_sum = 0
    if i % BATCH == 0:
        for key in firstMove:
            s = sum(counts[op] for op in firstMove[key])
            print(s, end="\t")
        print(end="\t")
        for key in secondMove:
            s = sum(counts[op] for op in secondMove[key])
            total_sum += s
            print(s, end="\t")
        print("\t",result['x'],"\t",result['o'],"\t",result['.'])
        assert(total_sum == BATCH)
        # print(BATCH - sum(counts.values()))
        countp = counts
        counts = defaultdict(int)
        result = defaultdict(int)