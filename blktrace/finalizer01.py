import sys
import re

if len(sys.argv) < 3:
    print "Usage: %s <input file> <output file>" % sys.argv[0]
    exit(1)

ifile = sys.argv[1]
ofile = sys.argv[2]

out = open(ofile, 'w')
with open(ifile) as f:
    for line in f:
        tmp = line.split()
        if (tmp[5] == '[java]') or (tmp[5] == '[kswapd0]'):
            out.write(line)
            

