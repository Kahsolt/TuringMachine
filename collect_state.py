#!/usr/bin/env python3
# 2019年11月8日
# 快速收集tm文件中，转移函数所使用的状态名称，然后你就可以直接粘贴到#Q
#

import sys

if __name__ == "__main__":
  if len(sys.argv) != 2:
    print('Usage: %s <*.tm>' % sys.argv[0])
    exit(0)

  states = set()
  with open(sys.argv[1], encoding='utf8') as fp:
    for line in fp.read().split('\n'):
      line = line.strip()
      if line and not line.startswith(';') and not line.startswith('#'):
        trans = line.split(' ')[:6]
        states.add(trans[0])
        states.add(trans[4])
  
  s = '{ ' + ', '.join(sorted(list(states))) + ' }'
  print(s)
