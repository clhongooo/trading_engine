#!/bin/bash
parse_cme_mdp_config_xml.py ../conf/cme_mdp/nrcert_config.xml | grep -A 3 UDP | list_to_matrix 5 | csel , 2 4
