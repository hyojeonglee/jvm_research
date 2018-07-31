import sys
import re

if len(sys.argv) < 2:
    print "Usage: %s <input result>" % sys.argv[0]
    exit(1)

ifile = sys.argv[1]
out_RD = open('./out_RD', 'a')
out_WR = open('./out_WR', 'a')
out_SW_R = open('./out_SW_R', 'a')
out_SW_W = open('./out_SW_W', 'a')


out_RD.write('#X Y\n')
out_WR.write('#X Y\n')
out_SW_R.write('#X Y\n')
out_SW_W.write('#X Y\n')

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
            if ('R' in tmp[0]):
                out_SW_R.write(tmp[4] + ' ' + st_addr + '\n')
            elif ('W' in tmp[0]):
                out_SW_W.write(tmp[4] + ' ' + st_addr + '\n')
        elif (len(tmp) == 3):
            if ('R' in tmp[0]):
                out_RD.write(tmp[2] + ' ' + st_addr + '\n')
            elif ('W' in tmp[0]):
                out_WR.write(tmp[2] + ' ' + st_addr + '\n')
            
out_RD.close()
out_WR.close()
out_SW_R.close()
out_SW_W.close()


