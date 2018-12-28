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

out_RD.write('#X Y')
out_WR.write('#X Y')
out_SW_R.write('#X Y')
out_SW_W.write('#X Y')

with open(ifile) as f:
    for line in f:
        event_str = ''
        tmp = line.split()
        for i in range(int(tmp[0]), int(tmp[0])+int(tmp[2])+1):
            event_str += '\n' + tmp[6] + ' ' + str(i)
        if (tmp[5] == '[java]'):
            if ('R' in tmp[4]):
                out_RD.write(event_str)
            elif ('W' in tmp[4]):
                out_WR.write(event_str)
        elif (tmp[5] == '[kswapd0]'):
            if ('R' in tmp[4]):
                out_SW_R.write(event_str)
            elif ('W' in tmp[4]):
                out_SW_W.write(event_str)
            
out_RD.close()
out_WR.close()
out_SW_R.close()
out_SW_W.close()


