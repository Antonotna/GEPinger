# GEPinger
Pinger for windows

GUI pinger for windows written on QT framework.


General difference from standart windows ping:

1. _Support for changing of DSCP value in IP header_
2. _Support for fastping_
3. _Support for copying ouput to clipboard_
4. _GUI_

Short options explanations.

+ `inter - interval(in ms) between recieving ICMP replay packet and sending ICMP requets the next one.`

  `(e.g. 0 - fastping, 1000 - standart ping)`
  
+ `timeout - time(in sec) wait for ICMP replay`

+ `Count - num of sending packets. 0 for infinite ping.`

Rest options must be obvious.
