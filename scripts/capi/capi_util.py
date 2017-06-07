#!/usr/bin/env python
import time

###################################################
Color_Off="\033[0m"       # Text Reset

# Regular Colors
Black="\033[0;30m"        # Black
Red="\033[0;31m"          # Red
Green="\033[0;32m"        # Green
Yellow="\033[0;33m"       # Yellow
Blue="\033[0;34m"         # Blue
Purple="\033[0;35m"       # Purple
Cyan="\033[0;36m"         # Cyan
White="\033[0;37m"        # White

# Bold
BBlack="\033[1;30m"       # Black
BRed="\033[1;31m"         # Red
BGreen="\033[1;32m"       # Green
BYellow="\033[1;33m"      # Yellow
BBlue="\033[1;34m"        # Blue
BPurple="\033[1;35m"      # Purple
BCyan="\033[1;36m"        # Cyan
BWhite="\033[1;37m"       # White
###################################################

def read_file(file_loc):
    with open(file_loc,'r') as f:
        return filter(lambda x: x[0] != '#', [line.strip() for line in f])

def justify_str(s,totlen,left_right="right",padchar=' '):
    def extra(s,totlen):
        return ''.join(map(lambda x: padchar, range(totlen - len(s))))
    s = str(s)
    if left_right == "left":
        return s + extra(s,totlen)
    elif left_right == "right":
        return extra(s,totlen) + s
    else:
        return s

def right_justify(s,totlen):
    return justify_str(s,totlen,"right")
def left_justify(s,totlen):
    return justify_str(s,totlen,"left")

def get_exchg(sym):
    sym_up = sym.upper()
    if sym_up[:3] in ["HSI","HHI","MHI","MCH"]:
        return "HKFE"
    elif sym_up[:2] in ["ES","NQ","YM","GC","SI","HG"]:
        return "CME"
    elif sym_up[:2] in ["AU","AG","CU"]:
        return "SHFE"
    else:
        return "HKFE"

def expand_to_signalfeed(oid_sf_tup_dict,instruction,oid_suffix=0,delimiter=','):
    def construct_tup(sf):
        return (sf, ','.join(map(str,sf)))

    fields_list = instruction.strip('\0').strip().split(delimiter)

    if len(fields_list) == 2:
        oaction,oid = fields_list
        new_sf = oid_sf_tup_dict[oid][0]
        new_sf[9] = "delete"
        return construct_tup(new_sf)
    else:
        augmented_fields_list = (fields_list + [""]*10)[:5]
        oaction,sym,px,qty,openclose = augmented_fields_list

        exchg         = get_exchg(sym)
        ymdhms        = time.strftime("%Y%m%d_%H%M%S")
        hms           = time.strftime("%H%M%S")
        fts           = ymdhms + "_000000"
        oid           = '_'.join(map(str, ["oid",hms,oid_suffix]))
        open_or_close = "open" if openclose == "" else openclose
        buy_or_sell   = "1" if float(qty) >= 0 else "2"
        oaction       = "insert" if oaction.lower() == "i" else "delete"
        otype         = "limit_order"
        oval          = "today"

        sf = [fts, "signalfeed", exchg, sym, oid, px, abs(float(qty)), open_or_close, buy_or_sell, oaction, otype, oval]
        return construct_tup(sf)

