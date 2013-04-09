#!/bin/sh
# ensure that you execute this script in a ROOT enabled shell
# something like this could do the job
# cd /opt/root && . ./bin/thisroot.sh && cd - > /dev/null


# 1) Build AcquRoot in acqu_core
cd acqu_core
acqu_sys=`pwd` CCCOMP=g++ make AcquRoot
