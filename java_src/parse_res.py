#!/usr/bin/env python

import sys
import re

if len(sys.argv) < 2:
    print "Usage: %s <res file>" % sys.argv[0]
    exit(1)

filepath = sys.argv[1]

cnt = 1

print('#X Y')
with open(filepath, 'r') as f:
    for line in f:
        if 'Swappiness' in line:
            line_list = line.split()
            print(str(cnt) + ' ' + str(line_list[2]))
            cnt += 1 
