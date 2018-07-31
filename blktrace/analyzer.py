import sys
import re

if len(sys.argv) < 3:
    print "Usage: %s <parsed result> <output file>" % sys.argv[0]
    exit(1)

pfile = sys.argv[1]
ofile = sys.argv[2]

read = 'R'
write = 'W'
none = 'N'
swap = '[kswapd0]'
java = '[java]'
to_dev = 'D'

out = open(ofile, 'w')

with open(pfile) as f:
    for line in f:
        tmp = line.split()
        if (len(tmp) < 5) or (tmp[6] == none):
            continue
        if (len(tmp) > 10):
            if (tmp[10] == '[bash]'):
                continue
            if (read in tmp[6]) or (write in tmp[6]) or (tmp[10] == swap) or (tmp[10] == java):
                if (tmp[10] == '<-'):
                    continue
                # if (tmp[5] == to_dev):
                result = tmp[7] + ' + ' + tmp[9] + ' : ' + tmp[6] + ' ' + tmp[10] + ' ' + tmp[3] + '\n'
                out.write(result)

