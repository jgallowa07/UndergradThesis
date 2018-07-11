import sys as os
#from pathlib import Path


import matplotlib.pyplot as plt
'''
# evenly sampled time at 200ms intervals
t = np.arange(0., 5., 0.2)

# red dashes, blue squares and green triangles
plt.plot(t, t, 'r--', t, t**2, 'bs', t, t**3, 'g^')
plt.show()
'''

'''
rootdir = Path('/Users/jared/Documents/SLiM_TSBenchmarks/Bench/ComparisonTimes/RealTimes')
# Return a list of regular files only, not directories
file_list = [f for f in rootdir.glob('**/*') if f.is_file()]

print(file_list)
'''

import sys
import re
files = []

P = [1000,2500,5000,7500,10000,20000,100000]

timing_data = {}
memory_data = {}
speedups = {}

'''
Keys

neut_timing_N1000
neut_timing_N10000

pedNM_timing_N1000
pedNM_timing_N10000

ped_timing_N1000
ped_timing_N10000
ped_add_timing_N1000
ped_add_timing_N10000

rel_timing_N1000
rel_timing_N10000


'''
for i in sys.argv[1:]:
    OK = True
    for line in open(i, 'r'):
        if re.search('non-zero', line):
            OK = False
            break
    if OK is True:
        files.append(i)

for i in files:
    
    sep_value = i.split(".")
        
    #print(sep_value)
    N = sep_value[2][1:]
    Rho = sep_value[3][4:]
    trace_key = sep_value[1][1:] + "_" + sep_value[2]
    fp = open(i,'r')
    data = fp.readline().split('/')
    runtime = float(data[2])/60/60
    memory = float(data[3])
    #print(runtime,memory)

    if (trace_key in timing_data):
        timing_data[trace_key].append(runtime)
        memory_data[trace_key].append(memory)
    else:
        timing_data[trace_key] = [runtime]
        memory_data[trace_key] = [memory]

#print(timing_data["ped_timing_N1000"])

for i in timing_data:
	print(i,"has value",timing_data[i])

for i in memory_data:
	print(i,"has value",timing_data[i])



'''
for i in timing_data:
#    print(i, "has value", timing_data[i])
    parse = i.split("_")
    if (parse[1] == "add"):
        #print(parse)

        #print(i, "has value", timing_data[i])
        for j in range(len(timing_data[i])):
            timing_data[parse[0]+"_"+parse[2]+"_"+parse[3]][j] += timing_data[i][j]


#print(timing_data["ped_timing_N1000"])
        
for i in timing_data:
    timing_data[i] = sorted(timing_data[i])
    print(i)

for i in memory_data:
    memory_data[i] = sorted(memory_data[i])


	

PopSizes = ['N = 1e+03','N = 1e+04']

f1, axarr1 = plt.subplots(1,2,sharey=True,sharex=True,linewidth=.01)
axarr1[0].plot(P,timing_data['neut_timing_N1000'],'go-',label=PopSizes[0])
axarr1[0].plot(P,timing_data['neut_timing_N10000'],'b^-',label=PopSizes[1])
axarr1[0].plot(P,timing_data['rel_timing_N1000'],'mo--',label=PopSizes[0] + ' SLiM 2.6') 
axarr1[0].plot(P,timing_data['rel_timing_N10000'],'c^--',label=PopSizes[1] + ' SLiM 2.6') 
axarr1[0].legend(loc='upper left',frameon=False)
axarr1[0].set_xscale('log')
#axarr1[0].set_yscale('log')
axarr1[0].set_ylabel('Run time (hours)',fontsize='medium')
axarr1[0].set_xlabel('Scaled recombination rate (' + r'$\rho = 4Nr$)',fontsize='medium')
axarr1[0].set_title('With Neutral Mutations',fontsize='medium')
axarr1[0].grid()

axarr1[1].plot(P,timing_data['ped_timing_N1000'],'go-',label=PopSizes[0])
axarr1[1].plot(P,timing_data['ped_timing_N10000'],'b^-',label=PopSizes[1])
axarr1[1].plot(P,timing_data['pedNM_timing_N1000'],'mo--',label=PopSizes[0] + ' [recordMutations = F]') 
axarr1[1].plot(P,timing_data['pedNM_timing_N10000'],'c^--',label=PopSizes[1] + ' [recordMutations = F]') 
axarr1[1].legend(loc='upper left',frameon=False)
axarr1[1].set_xlabel('Scaled recombination rate (' + r'$\rho = 4Nr$)',fontsize='medium')
axarr1[1].set_title('With Tree Sequence Recording',fontsize='medium')
axarr1[1].grid()


f, axarr = plt.subplots(1,2,sharey=True,sharex=True,linewidth=.01)
axarr[0].plot(P,memory_data['neut_timing_N1000'],'go-',label=PopSizes[0])
axarr[0].plot(P,memory_data['neut_timing_N10000'],'b^-',label=PopSizes[1])
axarr[0].plot(P,memory_data['rel_timing_N1000'],'mo--',label=PopSizes[0] + ' SLiM 2.6') 
axarr[0].plot(P,memory_data['rel_timing_N10000'],'c^--',label=PopSizes[1] + ' SLiM 2.6') 
axarr[0].legend(loc='upper left',frameon=False)
axarr[0].set_xscale('log')
#axarr[0].set_yscale('log')
axarr[0].set_ylabel('Memory (maximum resident set size in KB)',fontsize='medium')
axarr[0].set_xlabel('Scaled recombination rate (' + r'$\rho = 4Nr$)',fontsize='medium')
axarr[0].set_title('With Neutral Mutations',fontsize='medium')
axarr[0].grid()

axarr[1].plot(P,memory_data['ped_timing_N1000'],'go-',label=PopSizes[0])
axarr[1].plot(P,memory_data['ped_timing_N10000'],'b^-',label=PopSizes[1])
axarr[1].plot(P,memory_data['pedNM_timing_N1000'],'mo--',label=PopSizes[0] + ' [recordMutations = F]') 
axarr[1].plot(P,memory_data['pedNM_timing_N10000'],'c^--',label=PopSizes[1] + ' [recordMutations = F]') 
axarr[1].legend(loc='upper left',frameon=False)
axarr[1].set_xlabel('Scaled recombination rate (' + r'$\rho = 4Nr$)',fontsize='medium')
axarr[1].set_title('With Tree Sequence Recording',fontsize='medium')
axarr[1].grid()

plt.show()

'''



'''
f, axarrr = plt.subplots(1,2,sharey=True,sharex=True)
axarr[0].plot(P,timing_data['rel_timing_N1000'],'go-',label=PopSizes[0])
axarr[0].plot(P,timing_data['rel_timing_N10000'],'b^-',label=PopSizes[1])
axarr[0].legend(loc='upper left',frameon=False)
axarr[0].set_xscale('log')


axarr[1].plot(P,timing_data['rel_timing_N1000'],'go-',label=PopSizes[0])
axarr[1].plot(P,timing_data['rel_timing_N10000'],'b^-',label=PopSizes[1])
'''     

   




