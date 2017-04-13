#!/bin/bash

wget -O ../conf/cme_mdp/pdn_config.xml    ftp://ftp.cmegroup.com/SBEFix/Production/Configuration/config.xml
wget -O ../conf/cme_mdp/nrcert_config.xml ftp://ftp.cmegroup.com/SBEFix/NRAutoCertPlus/Configuration/config.xml
wget -O ../conf/cme_mdp/cert_config.xml   ftp://ftp.cmegroup.com/SBEFix/CertAutoCertPlus/Configuration/config.xml
