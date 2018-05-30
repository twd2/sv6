sv6 is a POSIX-like research operating system designed for multicore
scalability based on [xv6](http://pdos.csail.mit.edu/6.828/xv6).

sv6 is not a production kernel.  Think of it as a playground full of
half-baked experiments, dead code, some really cool hacks, and a few
great results.

*This is RISC-V 64 port.*


Building and running sv6 in QEMU
--------------------------------

TL;DR: `make && make qemu`

You'll need GCC for RISC-V 64 and GNU make.

There are several variables at the top of the top-level `Makefile` you
may want to override for your build environment.  It is recommended
you set them in `config.mk`.

The kernel is configured via `param.h`.  If you're just running sv6 in
QEMU, you don't have to modify `param.h`, but you may want to read
through it.

The most important `Makefile` variable is `HW`.  This controls the
hardware target you're building for and affects many settings both in
the `Makefile` and `param.h`.  The default `HW` is `qemu`.  Each of
our multicore machines also has a `HW` target, and other interesting
`HW` targets are mentioned below.
Builds go to `o.$HW`.


Running sv6 on real hardware (only support HiFive Unleashed)
------------------------------------------------------------

Make sure you can build and boot sv6 in QEMU first.

TL;DR: `make sdimg`

The SD card image is `o.qemu/sd.img`.
Please use `dd` or other tools to write the image to a real SD card.
Then, insert and boot!


Supported hardware
------------------

Not much.

sv6 RISC-V 64 port is known to run on 2 machines: QEMU and HiFive Unleashed.

No networking.


Running sv6 user-space in Linux (not tested)
--------------------------------------------

Much of the sv6 user-space can also be compiled for and run in Linux
using `make HW=linux`.  This will place Linux-compatible binaries in
`o.linux/bin`.

You can also boot a Linux kernel into a pure sv6 user-space!  `make
HW=linux` also builds `o.linux/initramfs`, which is a Linux initramfs
file system containing an sv6 init, sh, ls, and everything else.  You
can boot this on a real machine, or run a super-lightweight Linux VM
in QEMU using

    make HW=linux KERN=path/to/Linux/bzImage/or/vmlinuz qemu


How to
======

CPU profiling
-------------

sv6 supports NMI-based system-wide hardware performance counter
profiling on both Intel and AMD CPUs.  On recent Intel CPUs, it also
supports PEBS precise event sampling and memory load latency
profiling.

To profile a command, use the `perf` tool.  E.g.,

    perf mailbench -a all / 1

By default, `perf` monitors unhalted CPU cycles, but other events can
be selected from those known to `libutil/pmcdb.cc`.

Once `perf` has run, the sampler data can be read from `/dev/sampler`.
To transfer the file to your computer where it can be decoded, use the
web server:

    curl http://<hostname>/dev/sampler > sampler

Finally, to decode the sample file, use `perf-report`:

    ./o.$HW/tools/perf-report sampler o.$HW/kernel.elf

To get stack traces from a user binary, pass its unstripped ELF image
(e.g., `o.$HW/bin/ls.unstripped`) as the last argument instead of the
kernel image.


Kernel statistics
-----------------

The kernel continually maintains a lot of internal statistics
counters.  To see the changes in these counters over a command, run,
e.g.

    monkstats mailbench -a all / 1
