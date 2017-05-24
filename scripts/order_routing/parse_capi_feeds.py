#!/usr/bin/env python2
import sys

def parse_capi_feed(capi_feed):

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
          return str(fld) + " (sell)"
      elif int(fld) == 1:
          return str(fld) + " (buy)"
      else:
          return str(fld) + " (unknown)"

  def explain_ordert_status(fld):
      if int(fld) == 1:
          return str(fld) + " (error)"
      elif int(fld) == 0:
          return str(fld) + " (ok)"
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
  fields = capi_feed.strip().split(',') + [""]*10
  if fields[1] == "signalfeed":
    key_list = ["feed_type","timestamp","market","symbol","order_id","price","quantity","open_close","buy_sell","order_action","order_type","order_validity","others"]
    value_list = ["signalfeed",fields[0],fields[2],fields[3],fields[4],fields[5],fields[6],fields[7],fields[8],fields[9],fields[10],fields[11],fields[12]]
  elif fields[1] == "orderfeed":
    key_list = ["feed_type","timestamp","market","symbol","order_id","price","quantity","open_close","buy_sell","quantity_filled","deleted","order_status","error_desc","source","order_type","order_validity"]
    value_list = ["orderfeed",fields[0],fields[2],fields[3],fields[4],fields[5],fields[6],fields[7],explain_buy_sell(fields[8]),fields[9],explain_deleted(fields[10]),explain_ordert_status(fields[11]),fields[12],fields[13],fields[14],fields[15],fields[16]]

  return map(lambda x: right_justify(x[0],20) + " : " + left_justify(x[1],30), zip(key_list,value_list))

if __name__ == '__main__':
  in_lines = [line for line in sys.stdin]
  print '\n\n'.join(map(lambda x: '\n'.join(parse_capi_feed(x)), in_lines))
