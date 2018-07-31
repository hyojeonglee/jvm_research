import sys
import re

if len(sys.argv) < 2:
    print "Usage: %s <input result>" % sys.argv[0]
    exit(1)

ifile = sys.argv[1]
out_RD = open('./out_RD', 'a')
out_WR = open('./out_WR', 'a')
out_SW = open('./out_SW', 'a')

out_RD.write('#X Y')
out_WR.write('#X Y')
out_SW.write('#X Y')

st_addr_pre = '#'
st_addr = 0

with open(ifile) as f:
    for line in f:
        if st_addr_pre in line:
            tmp = line.split()
            st_addr = tmp[1]
            continue
        tmp = line.split()
        if (len(tmp) == 5):
            out_SW.write(st_addr + ' ' + tmp[4])
        elif (len(tmp) == 3):
            if 'R' in tmp[0]:
                out_RD.write(st_addr + ' ' + tmp[2])
            else 'W' in tmp[0]:
                out_WR.write(st_addr + ' ' + tmp[2])
            
out_RD.close()
out_WR.close()
out_SW.close()


