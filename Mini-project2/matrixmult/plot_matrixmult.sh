#!/bin/bash

# load modules
if command -v module 1>/dev/null 2>&1; then
   module load gcc/6.1.0 mkl/11.3 gnuplot
fi

gnuplot timing.gp
