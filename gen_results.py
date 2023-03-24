import subprocess
import resource
import matplotlib.pyplot as plt

size = 6000
strsize = str(size)

procs = []
time = []
for i in range(2, 17):
  usage_start = resource.getrusage(resource.RUSAGE_CHILDREN)
  subprocess.run('mpiexec -n ' + str(i) + ' ./main.bin', shell=True)
  usage_end = resource.getrusage(resource.RUSAGE_CHILDREN)
  cpu_time = usage_end.ru_utime - usage_start.ru_utime
  procs.append(i)
  time.append(cpu_time)

fig, ax = plt.subplots()
ax.plot(procs, time)

ax.set(xlabel='processes', ylabel='time (sec)',
       title='matrix size: ' + strsize + ' * ' + strsize)
ax.grid()

fig.savefig(strsize + '.png')
# plt.show()