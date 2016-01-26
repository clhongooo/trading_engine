#!/bin/bash

rm -f core* ./Debug/core*

hk_stk()
{
    for i in $(seq 2007 2014)
    do
        ./Debug/FSMC_Training /home/qy/Dropbox/nirvana/conf/hsi_constit/"$i"01.csv /home/qy/Dropbox/dataENF/yahData/data/ .HK.csv /home/qy/Dropbox/dataENF/assembledData/VHSI_D1.csv /home/qy/Dropbox/nirvana/conf/hsi_fsmc_1d_pmf/ $(expr $i - 2)"0101" $(expr $i - 1)"1231" /home/qy/Dropbox/nirvana/conf/HKFE_Calendar_full.csv 0.00005 0.3 22 0.000725
        ./Debug/FSMC_Training /home/qy/Dropbox/nirvana/conf/hsi_constit/"$i"02.csv /home/qy/Dropbox/dataENF/yahData/data/ .HK.csv /home/qy/Dropbox/dataENF/assembledData/VHSI_D1.csv /home/qy/Dropbox/nirvana/conf/hsi_fsmc_1d_pmf/ $(expr $i - 2)"0201"             $i"0131" /home/qy/Dropbox/nirvana/conf/HKFE_Calendar_full.csv 0.00005 0.3 22 0.000725
        ./Debug/FSMC_Training /home/qy/Dropbox/nirvana/conf/hsi_constit/"$i"03.csv /home/qy/Dropbox/dataENF/yahData/data/ .HK.csv /home/qy/Dropbox/dataENF/assembledData/VHSI_D1.csv /home/qy/Dropbox/nirvana/conf/hsi_fsmc_1d_pmf/ $(expr $i - 2)"0301"             $i"0231" /home/qy/Dropbox/nirvana/conf/HKFE_Calendar_full.csv 0.00005 0.3 22 0.000725
        ./Debug/FSMC_Training /home/qy/Dropbox/nirvana/conf/hsi_constit/"$i"04.csv /home/qy/Dropbox/dataENF/yahData/data/ .HK.csv /home/qy/Dropbox/dataENF/assembledData/VHSI_D1.csv /home/qy/Dropbox/nirvana/conf/hsi_fsmc_1d_pmf/ $(expr $i - 2)"0401"             $i"0331" /home/qy/Dropbox/nirvana/conf/HKFE_Calendar_full.csv 0.00005 0.3 22 0.000725
        ./Debug/FSMC_Training /home/qy/Dropbox/nirvana/conf/hsi_constit/"$i"05.csv /home/qy/Dropbox/dataENF/yahData/data/ .HK.csv /home/qy/Dropbox/dataENF/assembledData/VHSI_D1.csv /home/qy/Dropbox/nirvana/conf/hsi_fsmc_1d_pmf/ $(expr $i - 2)"0501"             $i"0431" /home/qy/Dropbox/nirvana/conf/HKFE_Calendar_full.csv 0.00005 0.3 22 0.000725
        ./Debug/FSMC_Training /home/qy/Dropbox/nirvana/conf/hsi_constit/"$i"06.csv /home/qy/Dropbox/dataENF/yahData/data/ .HK.csv /home/qy/Dropbox/dataENF/assembledData/VHSI_D1.csv /home/qy/Dropbox/nirvana/conf/hsi_fsmc_1d_pmf/ $(expr $i - 2)"0601"             $i"0531" /home/qy/Dropbox/nirvana/conf/HKFE_Calendar_full.csv 0.00005 0.3 22 0.000725
        ./Debug/FSMC_Training /home/qy/Dropbox/nirvana/conf/hsi_constit/"$i"07.csv /home/qy/Dropbox/dataENF/yahData/data/ .HK.csv /home/qy/Dropbox/dataENF/assembledData/VHSI_D1.csv /home/qy/Dropbox/nirvana/conf/hsi_fsmc_1d_pmf/ $(expr $i - 2)"0701"             $i"0631" /home/qy/Dropbox/nirvana/conf/HKFE_Calendar_full.csv 0.00005 0.3 22 0.000725
        ./Debug/FSMC_Training /home/qy/Dropbox/nirvana/conf/hsi_constit/"$i"08.csv /home/qy/Dropbox/dataENF/yahData/data/ .HK.csv /home/qy/Dropbox/dataENF/assembledData/VHSI_D1.csv /home/qy/Dropbox/nirvana/conf/hsi_fsmc_1d_pmf/ $(expr $i - 2)"0801"             $i"0731" /home/qy/Dropbox/nirvana/conf/HKFE_Calendar_full.csv 0.00005 0.3 22 0.000725
        ./Debug/FSMC_Training /home/qy/Dropbox/nirvana/conf/hsi_constit/"$i"09.csv /home/qy/Dropbox/dataENF/yahData/data/ .HK.csv /home/qy/Dropbox/dataENF/assembledData/VHSI_D1.csv /home/qy/Dropbox/nirvana/conf/hsi_fsmc_1d_pmf/ $(expr $i - 2)"0901"             $i"0831" /home/qy/Dropbox/nirvana/conf/HKFE_Calendar_full.csv 0.00005 0.3 22 0.000725
        ./Debug/FSMC_Training /home/qy/Dropbox/nirvana/conf/hsi_constit/"$i"10.csv /home/qy/Dropbox/dataENF/yahData/data/ .HK.csv /home/qy/Dropbox/dataENF/assembledData/VHSI_D1.csv /home/qy/Dropbox/nirvana/conf/hsi_fsmc_1d_pmf/ $(expr $i - 2)"1001"             $i"0931" /home/qy/Dropbox/nirvana/conf/HKFE_Calendar_full.csv 0.00005 0.3 22 0.000725
        ./Debug/FSMC_Training /home/qy/Dropbox/nirvana/conf/hsi_constit/"$i"11.csv /home/qy/Dropbox/dataENF/yahData/data/ .HK.csv /home/qy/Dropbox/dataENF/assembledData/VHSI_D1.csv /home/qy/Dropbox/nirvana/conf/hsi_fsmc_1d_pmf/ $(expr $i - 2)"1101"             $i"1031" /home/qy/Dropbox/nirvana/conf/HKFE_Calendar_full.csv 0.00005 0.3 22 0.000725
        ./Debug/FSMC_Training /home/qy/Dropbox/nirvana/conf/hsi_constit/"$i"12.csv /home/qy/Dropbox/dataENF/yahData/data/ .HK.csv /home/qy/Dropbox/dataENF/assembledData/VHSI_D1.csv /home/qy/Dropbox/nirvana/conf/hsi_fsmc_1d_pmf/ $(expr $i - 2)"1201"             $i"1131" /home/qy/Dropbox/nirvana/conf/HKFE_Calendar_full.csv 0.00005 0.3 22 0.000725
    done
}

us_etf()
{
    for i in $(seq 2005 2015)
    do
        ./Debug/FSMC_Training /home/qy/Dropbox/nirvana/conf/etf_list.csv /home/qy/Dropbox/dataENF/Blmg/ .csv NOT_USED /home/qy/Dropbox/nirvana/conf/us_etf_fsmc_1d_pmf/ $(expr $i - 2)"0101" $(expr $i - 1)"1231" /home/qy/Dropbox/nirvana/conf/NYSE_Calendar_full.csv 0.00005 0.3 22 0.000725
        ./Debug/FSMC_Training /home/qy/Dropbox/nirvana/conf/etf_list.csv /home/qy/Dropbox/dataENF/Blmg/ .csv NOT_USED /home/qy/Dropbox/nirvana/conf/us_etf_fsmc_1d_pmf/ $(expr $i - 2)"0201"             $i"0131" /home/qy/Dropbox/nirvana/conf/NYSE_Calendar_full.csv 0.00005 0.3 22 0.000725
        ./Debug/FSMC_Training /home/qy/Dropbox/nirvana/conf/etf_list.csv /home/qy/Dropbox/dataENF/Blmg/ .csv NOT_USED /home/qy/Dropbox/nirvana/conf/us_etf_fsmc_1d_pmf/ $(expr $i - 2)"0301"             $i"0231" /home/qy/Dropbox/nirvana/conf/NYSE_Calendar_full.csv 0.00005 0.3 22 0.000725
        ./Debug/FSMC_Training /home/qy/Dropbox/nirvana/conf/etf_list.csv /home/qy/Dropbox/dataENF/Blmg/ .csv NOT_USED /home/qy/Dropbox/nirvana/conf/us_etf_fsmc_1d_pmf/ $(expr $i - 2)"0401"             $i"0331" /home/qy/Dropbox/nirvana/conf/NYSE_Calendar_full.csv 0.00005 0.3 22 0.000725
        ./Debug/FSMC_Training /home/qy/Dropbox/nirvana/conf/etf_list.csv /home/qy/Dropbox/dataENF/Blmg/ .csv NOT_USED /home/qy/Dropbox/nirvana/conf/us_etf_fsmc_1d_pmf/ $(expr $i - 2)"0501"             $i"0431" /home/qy/Dropbox/nirvana/conf/NYSE_Calendar_full.csv 0.00005 0.3 22 0.000725
        ./Debug/FSMC_Training /home/qy/Dropbox/nirvana/conf/etf_list.csv /home/qy/Dropbox/dataENF/Blmg/ .csv NOT_USED /home/qy/Dropbox/nirvana/conf/us_etf_fsmc_1d_pmf/ $(expr $i - 2)"0601"             $i"0531" /home/qy/Dropbox/nirvana/conf/NYSE_Calendar_full.csv 0.00005 0.3 22 0.000725
        ./Debug/FSMC_Training /home/qy/Dropbox/nirvana/conf/etf_list.csv /home/qy/Dropbox/dataENF/Blmg/ .csv NOT_USED /home/qy/Dropbox/nirvana/conf/us_etf_fsmc_1d_pmf/ $(expr $i - 2)"0701"             $i"0631" /home/qy/Dropbox/nirvana/conf/NYSE_Calendar_full.csv 0.00005 0.3 22 0.000725
        ./Debug/FSMC_Training /home/qy/Dropbox/nirvana/conf/etf_list.csv /home/qy/Dropbox/dataENF/Blmg/ .csv NOT_USED /home/qy/Dropbox/nirvana/conf/us_etf_fsmc_1d_pmf/ $(expr $i - 2)"0801"             $i"0731" /home/qy/Dropbox/nirvana/conf/NYSE_Calendar_full.csv 0.00005 0.3 22 0.000725
        ./Debug/FSMC_Training /home/qy/Dropbox/nirvana/conf/etf_list.csv /home/qy/Dropbox/dataENF/Blmg/ .csv NOT_USED /home/qy/Dropbox/nirvana/conf/us_etf_fsmc_1d_pmf/ $(expr $i - 2)"0901"             $i"0831" /home/qy/Dropbox/nirvana/conf/NYSE_Calendar_full.csv 0.00005 0.3 22 0.000725
        ./Debug/FSMC_Training /home/qy/Dropbox/nirvana/conf/etf_list.csv /home/qy/Dropbox/dataENF/Blmg/ .csv NOT_USED /home/qy/Dropbox/nirvana/conf/us_etf_fsmc_1d_pmf/ $(expr $i - 2)"1001"             $i"0931" /home/qy/Dropbox/nirvana/conf/NYSE_Calendar_full.csv 0.00005 0.3 22 0.000725
        ./Debug/FSMC_Training /home/qy/Dropbox/nirvana/conf/etf_list.csv /home/qy/Dropbox/dataENF/Blmg/ .csv NOT_USED /home/qy/Dropbox/nirvana/conf/us_etf_fsmc_1d_pmf/ $(expr $i - 2)"1101"             $i"1031" /home/qy/Dropbox/nirvana/conf/NYSE_Calendar_full.csv 0.00005 0.3 22 0.000725
        ./Debug/FSMC_Training /home/qy/Dropbox/nirvana/conf/etf_list.csv /home/qy/Dropbox/dataENF/Blmg/ .csv NOT_USED /home/qy/Dropbox/nirvana/conf/us_etf_fsmc_1d_pmf/ $(expr $i - 2)"1201"             $i"1131" /home/qy/Dropbox/nirvana/conf/NYSE_Calendar_full.csv 0.00005 0.3 22 0.000725
    done
}

us_etf
