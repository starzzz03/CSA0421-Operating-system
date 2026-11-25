#!/usr/bin/env bash
# Deploy 1: bare metal (the app run directly on Linux). Measures start-up, memory, size and rps.
cd "$(dirname "$0")/../B2"
t0=$(date +%s.%N); python3 app.py >/dev/null 2>&1 & pid=$!
until curl -s localhost:8000/health >/dev/null 2>&1 || python3 -c "import urllib.request,sys;urllib.request.urlopen('http://127.0.0.1:8000/health')" 2>/dev/null; do sleep 0.02; done
t1=$(date +%s.%N)
echo "Start-up time to first successful /health: $(echo "$t1 - $t0" | bc) s"
echo "Resident memory (RSS) of the app: $(grep VmRSS /proc/$pid/status)"
echo "Disk used by app files: $(du -sk app.py | cut -f1) KB (+ shared Python/Flask packages: $(du -sm $(python3 -c 'import flask,os;print(os.path.dirname(flask.__file__))') | cut -f1) MB for flask alone)"
for th in 4 16; do echo "--- / with $th client threads, 5 s:"; python3 ../B3/loadtest.py 127.0.0.1 8000 / $th 5; done
echo "--- /health, 16 threads:"; python3 ../B3/loadtest.py 127.0.0.1 8000 /health 16 5
echo "Peak RSS after load: $(grep VmHWM /proc/$pid/status)"
kill $pid
