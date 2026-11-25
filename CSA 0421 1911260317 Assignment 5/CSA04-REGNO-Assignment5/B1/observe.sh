#!/usr/bin/env bash
echo "=== ps (top 5 by memory) ==="; ps aux --sort=-%mem | head -6 | cut -c1-110
echo; echo "=== top (one batch iteration, first 12 lines) ==="; top -bn1 | head -12
echo; echo "=== free -m ==="; free -m
echo; echo "=== vmstat 1 4 ==="; vmstat 1 4
echo; echo "=== iostat equivalent: /proc/diskstats (vda) (iostat not installed) ==="; grep -w vda /proc/diskstats
echo; echo "=== df -h (real file systems) ==="; df -h -x tmpfs -x devtmpfs | head -5
echo; echo "=== network: ss not installed; /proc/net/sockstat and listening TCP sockets from /proc/net/tcp ==="; cat /proc/net/sockstat
awk 'NR>1 && $4=="0A" {split($2,a,":"); printf "listening on port %d\n", strtonum("0x" a[2])}' /proc/net/tcp 2>/dev/null || echo "(gawk needed for strtonum)"
