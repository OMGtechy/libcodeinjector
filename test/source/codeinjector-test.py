#!/usr/bin/env python3

import subprocess
import sys
import os

def main():
    lib_produced_filename = 'ctor'
    program_produced_filename = 'main'

    try:
        os.remove(lib_produced_filename)
    except FileNotFoundError:
        pass

    try:
        os.remove(program_produced_filename)
    except FileNotFoundError:
        pass

    enable_asan  = len(sys.argv) > 1 and sys.argv[1] == '1'

    print('enable_asan:  %r' % enable_asan)

    environment = os.environ.copy();

    if enable_asan: environment['LD_PRELOAD'] = 'libasan.so'

    completed_process = subprocess.run('./codeinjector-caller', env=environment)
    if completed_process.returncode != 0:
        print('return code non-zero', file = sys.stderr)
        return completed_process.returncode

    if not os.path.exists(lib_produced_filename):
        print('lib produced file missing', file = sys.stderr)
        return -1

    if not os.path.exists(program_produced_filename):
        print('program produced file missing', file = sys.stderr)
        return -1

    return 0

if __name__ == '__main__':
    sys.exit(main());

