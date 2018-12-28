import sys
import re

# Input: fin01.0801
if len(sys.argv) < 2:
    print "Usage: %s <input>" % sys.argv[0]
    exit(1)

# 1 blk offset = 512B
# rd/wr amount include 3-section
# 1. 0 to 24
# 2. 25 to 210
# 3. 211 to 270

ifile = sys.argv[1]
rd_amt = [0, 0, 0]
wr_amt = [0, 0, 0]

with open(ifile) as f:
    for line in f:
        tmp = line.split()
        if (tmp[5] == '[kswapd0]'):
            continue
        if (tmp[5] == '[java]'):
            ts = float(tmp[6])
            if ('R' in tmp[4]):
                if (ts >= 0 and ts < 25):
                    rd_amt[0] += int(tmp[2])
                elif (ts >= 25 and ts < 211):
                    rd_amt[1] += int(tmp[2])
                elif (ts >= 211 and ts < 270):
                    rd_amt[2] += int(tmp[2])
            elif ('W' in tmp[4]):
                if (ts >= 0 and ts < 25):
                    wr_amt[0] += int(tmp[2])
                elif (ts >= 25 and ts < 211):
                    wr_amt[1] += int(tmp[2])
                elif (ts >= 211 and ts < 270):
                    wr_amt[2] += int(tmp[2])

print ('>>> Read')
print (rd_amt)
i = 1
for a in rd_amt:
    tmp = (a * 512) / 1024
    print('Section' + str(i) + ' ' + str(tmp) + 'KB')
    i += 1

print ('>>> Write')
print (wr_amt)

i = 1
for a in wr_amt:
    tmp = (a * 512) / 1024
    print('Section' + str(i) + ' ' + str(tmp) + 'KB')
    i += 1

