import sys
import re

if len(sys.argv) < 4:
    print "Usage: %s <pstree log> <btrace file> <output file>" % sys.argv[0]
    exit(1)

logfile = sys.argv[1]
btfile = sys.argv[2]
outfile = sys.argv[3]
pids = []

# Get pids from pstree log
f = open(logfile)
target = 'java'
for line in f:
    if target in str(line):
        tmp = re.findall('\d+',line)
        for t in tmp:
            pids.append(t)
f.close()

out = open(outfile, 'w')
with open(btfile) as f:
    for line in f:
        tmp = line.split()
        if (len(tmp) < 5):
            continue
        for p in pids:
            if (tmp[4] == p):
                out.write(line)                
        if (len(tmp) > 10) and (tmp[10] == '[kswapd0]'):
            out.write(line)
