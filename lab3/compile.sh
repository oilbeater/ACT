#!/usr/bin/env bash
g++ cfg_ssa.cc -o cfg_ssa
g++ cfg_dce_ssa.cc -o cfg_dce_ssa
g++ cfg_scp_ssa.cc -o cfg_scp_ssa
g++ cfg_ssa_dce.cc -o cfg_ssa_dce
g++ cfg_ssa_scp.cc -o cfg_ssa_scp
g++ ssa_3add.cc -o ssa_3add
# A script that builds your compiler.
