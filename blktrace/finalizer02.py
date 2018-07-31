import sys
import re
from collections import defaultdict

# using all.analysis.0730
if len(sys.argv) < 3:
    print "Usage: %s <input file> <output file>" % sys.argv[0]
    exit(1)

ifile = sys.argv[1]
ofile = sys.argv[2]


st_addr = []
cnt = 0
with open(ifile) as f:
    for line in f:
        tmp = line.split()
        if not(int(tmp[0]) in st_addr):
            if (cnt % 100 == 0):
                print (str(cnt) + ' Append start addr!')
            st_addr.append(int(tmp[0]))
            cnt += 1

print ('>>> Finish append!')
print ('>>> Start sort!')

st_addr.sort()

print ('>>> Finish sort!')

ifile.close()

print ('>>> Start create mem dict!')
mem_dict = defaultdict(list)
for addr in st_addr:
    mem_dict[addr].append(' ')
print ('>>> Finish create mem dict!')
# print (mem_dict)

print ('>>> Start fill mem_dict with event strings!')
with open(ifile) as f:
    for line in f:
        tmp = line.split()
        key = int(tmp[0])
        event_str = ' '
        if (tmp[5] == '[java]'):
            event_str = tmp[4] + ' + ' + tmp[6]
        else:
            event_str = tmp[4] + ' + ' + tmp[5] + ' + ' + tmp[6]
            
        mem_dict[key].append(event_str)
print ('>>> Finish fill mem_dict with event strings!')

out = open(ofile, 'w')

print ('>>> Start write output file!')
for k in mem_dict.keys():
    e_list = mem_dict.get(k)
    out.write('# ' + k + ' : ' + len(tmp_list) + '\n')
    for e in e_list:
        out.write(e + '\n')

print ('>>> ALL FINISH!')


