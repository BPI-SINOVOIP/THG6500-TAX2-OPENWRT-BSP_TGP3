#!/bin/sh

#!/bin/sh

endtime=$(cat /proc/uptime | cut -d ' ' -f1)
logger -t ROAMing_wpa -p notice "roam_endtime=$endtime(s)"
starttime=$(uci -q -P /var/state get wireless.roam_start_time)
logger -t ROAMing_wpa -p notice "roam_starttime=$starttime(s)"
result=$(awk -v n1="$endtime" -v n2="$starttime" 'BEGIN {print n1 - n2}')
logger -t ROAM_wpa -p notice "roam_time=$result(s)"
