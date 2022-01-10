#pedigre_preprocess.py:

import argparse
import pandas as pd
import sys

sys.setrecursionlimit(100000)

E = []
parent = {}

def findset(x):
    if parent[x] == x:
        return parent[x]
    else:
        parent[x] = findset(parent[x])
        return parent[x]

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('filename', help='input forest file')
    parser.add_argument('--parent', dest='parent', default='mother', help='define which parent line to analyze')
    args = parser.parse_args()

    if (args.parent != 'father') and (args.parent != 'mother'):
        print("ERROR: Unrecognized option for parent! Allowed options: father, mother.")
        sys.exit()

    data = pd.read_csv(args.filename, dtype=str).fillna('')

    global parent
    parent = { row.ID : row.ID for row in data.itertuples() }
    if 'live' in data.columns:
        alive = list( data[ data.live.astype(int) == 1 ].ID )
    else:
        alive = list( data.ID )

    for row in data.itertuples():
        x = row.ID
        if args.parent == 'father':
            iterator = [row.father]
        elif args.parent == 'mother':
            iterator = [row.mother]
        for y in iterator:
            if len(y) > 0:
                if y not in parent: continue
                if findset(x) != findset(y):
                    E.append((x,y))
                    parent[findset(y)] = findset(x)

    for x, y in E:
        print("{} {}".format(x, y))
    print("\n".join(alive))

if __name__ == "__main__":
    main()
