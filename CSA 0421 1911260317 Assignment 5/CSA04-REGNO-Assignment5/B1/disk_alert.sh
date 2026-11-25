#!/usr/bin/env bash
# Disk-space alert: warn when any file system is above THRESHOLD % (default 80). Log to syslog if possible.
THRESHOLD=${1:-80}
df -P -x tmpfs -x devtmpfs -x squashfs | awk -v t="$THRESHOLD" 'NR>1 {gsub("%","",$5); if ($5+0 >= t) printf "ALERT: %s is %s%% full (mounted on %s)\n", $1, $5, $6; else ok++} END {if (ok==NR-1) print "OK: all file systems below " t "%"}'
