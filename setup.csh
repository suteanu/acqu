#!/bin/tcsh

setenv acqu_dir ${HOME}/acqu

setenv acqu_sys ${acqu_dir}/acqu_core
setenv acqu ${acqu_dir}/acqu_user
setenv CALIB ${acqu_dir}/CaLib
setenv OSCAR ${acqu_dir}/OSCAR
setenv Worker ${acqu_dir}/Worker/AR

setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:${CALIB}/lib:${OSCAR}/lib
setenv PATH ${PATH}:${acqu}/bin:${Worker}/bin
