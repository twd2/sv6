#!/usr/bin/env python3

import sys

data = b''
with open('{}/kernel.elf'.format(sys.argv[1]), 'rb') as f:
  data = f.read()
with open('{}/kernel.elf.bak'.format(sys.argv[1]), 'wb') as f:
  f.write(data)

# remove usages of %fs
                    # mov %fs:0x28, %rax; mov %rax, ??(%rsp)
data = data.replace(b'\x64\x48\x8b\x04\x25\x28\x00\x00\x00\x48\x89',
                    # nop; mov %rax, ??(%rsp)
                    b'\x66\x0F\x1F\x84\x00\x00\x00\x00\x00\x48\x89')

                    # xor %fs:0x28, %rcx
data = data.replace(b'\x64\x48\x33\x0c\x25\x28\x00\x00\x00',
                    # xor %rcx, %rcx; nop;
                    b'\x48\x31\xc9\x66\x0F\x1F\x44\x00\x00')

                    # xor %fs:0x28, %rsi
data = data.replace(b'\x64\x48\x33\x34\x25\x28\x00\x00\x00',
                    # xor %rsi, %rsi; nop;
                    b'\x48\x31\xf6\x66\x0F\x1F\x44\x00\x00')

                    # xor %fs:0x28, %rbx
data = data.replace(b'\x64\x48\x33\x1c\x25\x28\x00\x00\x00',
                    # xor %rbx, %rbx; nop;
                    b'\x48\x31\xdb\x66\x0F\x1F\x44\x00\x00')

with open('{}/kernel.elf'.format(sys.argv[1]), 'wb') as f:
  f.write(data)
