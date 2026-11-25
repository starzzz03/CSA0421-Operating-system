#!/usr/bin/env python3
"""Tiny load generator (ab/wrk/hey are not installed): N threads, keep-alive, fixed duration."""
import sys, time, threading, http.client
host, port, path, threads, dur = sys.argv[1], int(sys.argv[2]), sys.argv[3], int(sys.argv[4]), float(sys.argv[5])
count = [0]*threads; errs = [0]*threads; lat = [[] for _ in range(threads)]
def w(i):
    c = http.client.HTTPConnection(host, port); end = time.time()+dur
    while time.time() < end:
        t = time.perf_counter()
        try:
            c.request("GET", path); r = c.getresponse(); r.read()
            if r.status == 200: count[i] += 1; lat[i].append(time.perf_counter()-t)
            else: errs[i] += 1
        except Exception:
            errs[i] += 1; c = http.client.HTTPConnection(host, port)
ts = [threading.Thread(target=w, args=(i,)) for i in range(threads)]
t0 = time.time(); [t.start() for t in ts]; [t.join() for t in ts]; el = time.time()-t0
allv = sorted(x for l in lat for x in l)
print(f"requests={sum(count)} errors={sum(errs)} rps={sum(count)/el:.0f} mean_ms={1000*sum(allv)/max(1,len(allv)):.2f} p99_ms={1000*allv[int(.99*len(allv))-1]:.2f}")
