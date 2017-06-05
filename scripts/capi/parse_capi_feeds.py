#!/usr/bin/env python2
import sys

def parse_capi_feed(capi_feed,indentation=0):
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

  def explain_buy_sell(fld):
      if int(fld) == 2:
          return str(fld) + Red + " (sell)" + Color_Off
      elif int(fld) == 1:
          return str(fld) + Green + " (buy)" + Color_Off
      else:
          return str(fld) + " (unknown)"

  def explain_ordert_status(fld):
      if int(fld) == 1:
          return str(fld) + BRed + " (error)" + Color_Off
      elif int(fld) == 0:
          return str(fld) + BGreen + " (ok)" + Color_Off
      else:
          return str(fld)

  def explain_deleted(fld):
      if int(fld) == 1:
          return str(fld) + " (deleted)"
      elif int(fld) == 0:
          return str(fld) + " (active)"
      else:
          return str(fld)

  def explain_source(fld):
      if int(fld) == 1:
          return str(fld) + " (msg reply)"
      elif int(fld) == 0:
          return str(fld) + " (real-time)"
      else:
          return str(fld)

  key_list   = []
  value_list = []
  fields = capi_feed.strip('\0').strip().split(',') + [""]*10
  if fields[1] == "signalfeed":
      key_list = ["feed_type","timestamp","market","symbol","order_id","price","quantity","open_close","buy_sell","order_action","order_type","order_validity","others"]
      value_list = ["signalfeed",fields[0],fields[2],fields[3],fields[4],fields[5],fields[6],fields[7],fields[8],fields[9],fields[10],fields[11],fields[12]]
  elif fields[1] == "orderfeed":
      key_list = ["feed_type","timestamp","market","symbol","order_id","price","quantity","open_close","buy_sell","quantity_filled","deleted","order_status","error_desc","source","order_type","order_validity"]
      value_list = ["orderfeed",fields[0],fields[2],fields[3],fields[4],fields[5],fields[6],fields[7],explain_buy_sell(fields[8]),fields[9],explain_deleted(fields[10]),explain_ordert_status(fields[11]),fields[12],explain_source(fields[13]),fields[14],fields[15],fields[16]]
  elif fields[1] == "tradefeed":
      key_list = ["feed_type","timestamp","market","symbol","order_id","price","quantity","buy_sell","trade_id","source"]
      value_list = [BPurple+"tradefeed"+Color_Off,fields[0],fields[2],fields[3],fields[4],fields[5],fields[6],explain_buy_sell(fields[7]),fields[8],explain_source(fields[9])]

  return map(lambda x: right_justify(x[0],indentation+20) + " : " + left_justify(x[1],30), zip(key_list,value_list))

if __name__ == '__main__':
    in_lines = [line for line in sys.stdin]
    print '\n\n'.join(map(lambda x: '\n'.join(parse_capi_feed(x)), in_lines))
