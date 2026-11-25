import math
students, hours, reqs_per_student, peak_factor = 150_000, 2, 8, 3          # ASSUMED: 8 requests per session, peak = 3x average
measured_rps_1core = 1205       # measured: B3/output_bare.txt, 4 client threads, Flask, 1 Python process
cpu_limit, util = 0.5, 0.7      # container limit 500m (deployment.yaml), plan to 70% utilisation
total = students*reqs_per_student; avg = total/(hours*3600); peak = avg*peak_factor
print(f"Total requests {total:,}; average {avg:.0f} req/s; planning peak {peak:.0f} req/s")
for label, factor in (("hello-page cost as measured", 1), ("5x heavier (templating + cached DB read)", 5), ("20x heavier (real DB query per request)", 20)):
    per = measured_rps_1core*cpu_limit*util/factor
    n = math.ceil(peak/per)
    print(f"{label:45s}: {per:6.0f} req/s per 0.5-CPU container -> {n:3d} containers ({n*0.25:.1f} CPU requested, {n*256} MiB requested)")
# Cost, 8 vCPU / 32 GB VM at ASSUMED $0.40/hour; containers packed at 2 per vCPU
for n in (2, 8, 30):
    vms = math.ceil(n/16); print(f"{n:3d} containers = {vms} VM(s) of 8 vCPU; result day (4 h incl. ramp) ${vms*0.40*4:.2f}; always-on year ${vms*0.40*8760:,.0f}")
