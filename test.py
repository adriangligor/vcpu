#! /usr/bin/env python
# -*- coding: utf_8 -*-

import os
import subprocess
import sys


class color:
    TEST = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'


def find_stages(stage_dir):
    stages = []
    stage_parent = os.path.dirname(os.path.abspath(stage_dir))
    last_stage_nr = int(os.path.basename(stage_dir)[5:])

    for stage_nr in xrange(1, last_stage_nr + 1):
        stage = stage_parent + "/stage" + str(stage_nr)
        if not os.path.exists(stage):
            break
        else:
            stages.append(os.path.relpath(stage))

    return stages

def find_tests(test_dir):
    tests = {}

    for root, dirs, files in os.walk(test_dir):
        for file in files:
            test_name, ext = os.path.splitext(file)
            if ext == ".lst" or ext == ".ans":
                if not test_name in tests:
                    tests[test_name] = 1
                else:
                    tests[test_name] = 2

    return tests

def read_ans(test_dir, test):
    file = os.path.join(test_dir, test + ".ans")
    lines = open(file, "r").read()
    return lines.replace("\n", "")

def retrieve_ans(stdout):
    ans_pos = stdout.rfind("\n", 0, -1)
    last_line = stdout[ans_pos:].replace("\n", "")
    eq_pos = last_line.find("=")
    return last_line[eq_pos + 1:]

def retrieve_err(stdout):
    ans_pos = stdout.rfind("\n", 0, -1)
    last_line = stdout[ans_pos:]
    return last_line.replace("\n", "")

def run_test(shell, file):
    p = subprocess.Popen([shell, file], stdout = subprocess.PIPE)
    stdout, stderr = p.communicate()
    exit_code = p.returncode
    return (stdout, exit_code)

def main():
    if len(sys.argv) == 1:
        print "usage: {0} stageX".format(os.path.basename(sys.argv[0]))
        sys.exit(1)

    stageX = sys.argv[1]
    if not os.path.exists(stageX):
        print "{0} not found, please provide a stage directory".format(stageX)
        sys.exit(1)

    if not os.path.basename(stageX).startswith("stage"):
        print "{0} is not a valid stage directory name".format(stageX)
        sys.exit(1)

    out = sys.stdout.write
    stages = find_stages(stageX)

    for stage in stages:
        out("testing {0}\n".format(stage))

        test_dir = stage + "/test"
        shell = stage + "/out/shell"

        if not os.path.exists(shell):
            print "{0} not found, please compile it first".format(shell)
            sys.exit(1)

        if not os.path.exists(test_dir) or not os.path.isdir(test_dir):
            print "{0} is not a valid directory".format(test_dir)
            sys.exit(1)

        tests = find_tests(test_dir)

        for test in sorted(tests.iterkeys()):
            status = tests[test]
            
            if status == 2:
                out("test {0}{1:<30}{2}".format(color.TEST, test, color.ENDC))
                
                file = os.path.join(test_dir, test) + ".lst"
                stdout, exit_code = run_test(shell, file)
                
                if exit_code == 0:
                    expected = read_ans(test_dir, test)
                    actual = retrieve_ans(stdout)
                    if expected == actual:
                        out("[{0}OK{1}]\n".format(color.OKGREEN, color.ENDC))
                    else:
                        out("[{0}FAIL{1}]  ".format(color.FAIL, color.ENDC))
                        out("expected {0}, got {1}\n".format(expected, actual))
                else:
                    err = retrieve_err(stdout)
                    out("[{0}HALT{1}]".format(color.WARNING, color.ENDC))
                    out("  '{0}'\n".format(err))


if __name__ == "__main__":
    main()
