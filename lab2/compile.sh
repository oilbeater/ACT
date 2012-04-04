#!/usr/bin/env bash
g++ translator.cc -o convert
g++ cfg_cfg.cc -o cfg_cfg
g++ cfg_scr.cc -o cfg_scr
g++ cfg_scp_rep.cc -o cfg_scp_rep
g++ cfg_scp_3addr.cc -o cfg_scp_3addr
g++ cfg_dse_rep.cc -o cfg_dse_rep
g++ cfg_dse_3addr.cc -o cfg_dse_3addr
# A script that builds your compiler.
