#!/bin/bash
# ensure that you execute this script in a ROOT enabled shell
# something like this could do the job
# cd /opt/root && . ./bin/thisroot.sh && cd - > /dev/null

source setup.sh

# 1a) Build AcquRoot in acqu_core
cd $acqu_sys
make AcquRoot

# 1b) Build OSCAR
cd $OSCAR
make

# 1c) Build CaLib
cd $CALIB
make

# 2) Build minimal acqu_user code
cd $acqu
make AcquRoot

cd $acqu_dir
echo "You might want to add 'source $acqu_dir/setup.sh' to your ~/.bashrc"
echo "This automatically sets the appropriate environment variables to run acqu"



