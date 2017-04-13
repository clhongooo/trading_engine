#!/usr/bin/env python
import sys
import argparse
import xml.etree.ElementTree as ET

def printItem(e):
    if len(e) > 0:
        for i in e:
            printItem(i)
    else:
        if e is not None and e.text is not None:
            print e.text

parser = argparse.ArgumentParser(description = "Parse CME MDP config xml")
parser.add_argument("cme_mdp_config_file", nargs='?', help="cme_mdp_config.xml")
args = parser.parse_args()

e = ET.parse(args.cme_mdp_config_file).getroot()
printItem(e)
