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

def read_expectation(test_dir, test):
    file = os.path.join(test_dir, test + ".ans")
    lines = []

    for line in open(file, "r").read().splitlines():
        if line != "" and line[0] != ";":
            val = line[:len(line) if line.find(";") == -1 else line.find(";")]
            val = val[:len(line) if val.find(" ") == -1 else val.find(" ")]
            lines.append(val)

    return tuple(lines) if len(lines) == 2 else (lines[0], None)

def retrieve_ans(stdout):
    lines = stdout.splitlines()
    ans_line = lines[-1] if lines[-1].find('ans') >= 0 else lines[-2]
    flg_line = lines[-1] if lines[-1].find('flg') >= 0 else None
    return (ans_line[ans_line.find("=") + 1:],
            flg_line[flg_line.find("=") + 1:] if flg_line is not None else None)

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
                    exp_ans, exp_flg = read_expectation(test_dir, test)
                    exp_s_ans = str(int(exp_ans) - 65536)
                    act_ans, act_flg = retrieve_ans(stdout)
                    same_ans = exp_ans == act_ans or exp_s_ans == act_ans
                    same_flg = exp_flg == act_flg or exp_flg is None

                    if same_ans and same_flg:
                        out("[{0}OK{1}]\n".format(color.OKGREEN, color.ENDC))
                    elif not same_ans:
                        out("[{0}FAIL{1}]  ".format(color.FAIL, color.ENDC))
                        out("expected ans '{0}' or '{1}', got '{2}'\n"
                            .format(exp_ans, exp_s_ans, act_ans))
                    elif not same_flg:
                        out("[{0}FAIL{1}]  ".format(color.FAIL, color.ENDC))
                        out("expected flg '{0}', got '{1}'\n"
                            .format(exp_flg, act_flg))
                else:
                    err = retrieve_err(stdout)
                    out("[{0}HALT{1}]".format(color.WARNING, color.ENDC))
                    out("  '{0}'\n".format(err))


if __name__ == "__main__":
    main()
