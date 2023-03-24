import random

size = 6000

with open('input.txt', 'w') as f:
  # size
  f.write(str(size) + ' ' + str(size) + '\n')
  # matrix
  for i in range(size):
    for j in range(size):
      f.write(str(random.randint(-100, 100)) + ' \n'[j == size - 1])
  # vector
  for j in range(size):
    f.write(str(random.randint(-100, 100)) + ' \n'[j == size - 1])
