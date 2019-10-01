#!/bin/bash

# load modules
if command -v module 1>/dev/null 2>&1; then
   module load gcc/6.1.0 gnuplot
fi

gnuplot generic.gp
