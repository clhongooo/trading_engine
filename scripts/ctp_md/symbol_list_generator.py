#!/usr/bin/env python
from datetime import date
product_type_list = ["ag","al","au","bu","cu","fu","hc","ni","pb","rb","ru","sn","wr","zn","a","b","bb","c","cs","fb","i","j","jd","jm","l","m","p","pp","v","y","CF","FG","JR","LR","MA","OI","PM","RI","RM","RS","SF","SM","SR","TA","WH","ZC","IC","IF","IH","T","TF"]

TDY = date.today()
month_list = map(lambda x: (x+TDY.month-1)%12+1, range(12))
year_add_list = map(lambda m: 0 if m >= TDY.month else 1, month_list)
year_list = map(lambda x: (TDY.year+x)%100, year_add_list)
year_month_list = map(lambda x: ''.join(map(str, x)), zip(year_list,map(lambda m: str(m).zfill(2), month_list)))

pym_list = [j for i in map(lambda p: map(lambda ym: p+ym, year_month_list), product_type_list) for j in i]
print ','.join(pym_list)
