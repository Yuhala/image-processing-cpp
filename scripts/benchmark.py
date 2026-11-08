#!/usr/bin/env python3
"""
benchmark.py - runs the sequential and OpenMP builds many times, collects the
TIMING lines they print (set by IMG_TIMING=1), and produces CSVs + speedup graphs.

usage (from the repo root):
  python3 scripts/benchmark.py --baseline ./app_baseline --parallel ./app_parallel \
      --image aya4096.pgm --threads 1 2 4 6 8 10 --reps 5

optional:
  --bestseq ./app_bestseq   a "best sequential" build (same source as parallel/,
                            compiled WITHOUT -fopenmp). If given, speedup is measured
                            against it, as the lecture notes define speedup (2.10).
  --ops gaussFilter sobelFilter ...   which operations to time
  --outdir results          where the CSVs and PNGs go

outputs:
  results/benchmark.csv   every single run
  results/summary.csv     medians, speedups, efficiency, per-phase times
  results/speedup.png     speedup vs threads (needs: pip3 install matplotlib)
  results/phases.png      read / compute / write time per thread count
"""
import argparse, csv, os, re, statistics, subprocess, sys, tempfile

p = argparse.ArgumentParser()
p.add_argument("--baseline", required=True, help="original sequential build")
p.add_argument("--parallel", required=True, help="OpenMP build")
p.add_argument("--bestseq", help="optional best-sequential build (parallel source, no -fopenmp)")
p.add_argument("--image", required=True)
p.add_argument("--threads", type=int, nargs="+", default=[1, 2, 4, 6, 8, 10])
p.add_argument("--reps", type=int, default=5)
p.add_argument("--ops", nargs="+",
               default=["gaussFilter", "sobelFilter", "histogramEquilization", "otsuBinarize", "mirror"])
p.add_argument("--outdir", default="results")
a = p.parse_args()

os.makedirs(a.outdir, exist_ok=True)
img = os.path.abspath(a.image)
TIMING = re.compile(r"TIMING threads=(\d+) read_ms=([\d.]+) compute_ms=([-\d.]+) "
                    r"write_ms=([\d.]+) total_ms=([\d.]+)")


def run(binary, op, threads):
    """Run one binary once; return (read, compute, write, total) in ms from its TIMING line."""
    env = dict(os.environ, OMP_NUM_THREADS=str(threads), IMG_TIMING="1")
    with tempfile.TemporaryDirectory() as d:  # output.pgm goes to a throwaway folder
        r = subprocess.run([os.path.abspath(binary), op, img], cwd=d, env=env,
                           stdout=subprocess.DEVNULL, stderr=subprocess.PIPE, text=True)
    if r.returncode != 0:
        sys.exit(f"{binary} {op} failed with exit code {r.returncode}")
    m = TIMING.search(r.stderr)
    if not m:
        sys.exit(f"No TIMING line from {binary}. Check the timing edits and rebuild.")
    return tuple(float(x) for x in m.groups()[1:])


builds = [("original", a.baseline, [1])]
if a.bestseq:
    builds.append(("bestseq", a.bestseq, [1]))
builds.append(("openmp", a.parallel, a.threads))

rows = []
for op in a.ops:
    print(f"== {op} ==", flush=True)
    for name, binary, thread_list in builds:
        for t in thread_list:
            run(binary, op, t)  # warm-up run, discarded (file cache, first-run costs)
            for rep in range(a.reps):
                rd, cp, wr, tot = run(binary, op, t)
                rows.append(dict(op=op, build=name, threads=t, rep=rep,
                                 read_ms=rd, compute_ms=cp, write_ms=wr, total_ms=tot))
            med = statistics.median(r["total_ms"] for r in rows
                                    if r["op"] == op and r["build"] == name and r["threads"] == t)
            print(f"  {name:8s} threads={t:2d}  median total = {med:10.1f} ms", flush=True)

with open(os.path.join(a.outdir, "benchmark.csv"), "w", newline="") as f:
    w = csv.DictWriter(f, fieldnames=list(rows[0].keys()))
    w.writeheader()
    w.writerows(rows)


def med(op, build, t, key):
    return statistics.median(r[key] for r in rows
                             if r["op"] == op and r["build"] == build and r["threads"] == t)


ref_build = "bestseq" if a.bestseq else "original"
summary = []
for op in a.ops:
    orig = med(op, "original", 1, "total_ms")
    ref = med(op, ref_build, 1, "total_ms")
    one = med(op, "openmp", a.threads[0], "total_ms")
    one_c = med(op, "openmp", a.threads[0], "compute_ms")
    for t in a.threads:
        tot = med(op, "openmp", t, "total_ms")
        comp = med(op, "openmp", t, "compute_ms")
        summary.append(dict(
            op=op, threads=t, total_ms=round(tot, 1),
            speedup=round(ref / tot, 2),                        # vs best sequential (or original)
            speedup_vs_original=round(orig / tot, 2),           # includes the I/O fix
            speedup_vs_openmp_1thread=round(one / tot, 2),      # pure parallel scaling
            efficiency=round((ref / tot) / t, 2),
            compute_speedup=round(one_c / comp, 2) if comp > 0 else "",
            read_ms=round(med(op, "openmp", t, "read_ms"), 1),
            compute_ms=round(comp, 1),
            write_ms=round(med(op, "openmp", t, "write_ms"), 1)))

with open(os.path.join(a.outdir, "summary.csv"), "w", newline="") as f:
    w = csv.DictWriter(f, fieldnames=list(summary[0].keys()))
    w.writeheader()
    w.writerows(summary)
print(f"\nspeedup measured against: {ref_build}")
print(open(os.path.join(a.outdir, "summary.csv")).read())

try:
    import matplotlib
    matplotlib.use("Agg")
    import matplotlib.pyplot as plt

    fig, ax = plt.subplots(figsize=(7, 5))
    for op in a.ops:
        pts = [s for s in summary if s["op"] == op]
        ax.plot([s["threads"] for s in pts], [s["speedup"] for s in pts], marker="o", label=op)
    ax.plot(a.threads, a.threads, "k--", linewidth=1, label="ideal linear")
    ax.axvline(6, color="grey", linestyle=":", linewidth=1)
    ax.text(6.1, 0.5, "6 performance cores", fontsize=8, color="grey")
    ax.set_xlabel("OpenMP threads")
    ax.set_ylabel(f"speedup vs {ref_build} sequential (x)")
    ax.set_title(f"Speedup on {os.path.basename(img)} (median of {a.reps} runs)", fontsize=10)
    ax.set_xticks(a.threads)
    ax.grid(alpha=0.3)
    ax.legend(fontsize=8)
    fig.tight_layout()
    fig.savefig(os.path.join(a.outdir, "speedup.png"), dpi=150)
    plt.close(fig)

    op0 = a.ops[0]
    pts = [s for s in summary if s["op"] == op0]
    labels = [str(s["threads"]) for s in pts]
    rd = [s["read_ms"] for s in pts]
    cp = [s["compute_ms"] for s in pts]
    wr = [s["write_ms"] for s in pts]
    fig, ax = plt.subplots(figsize=(7, 4.5))
    ax.bar(labels, rd, label="read")
    ax.bar(labels, cp, bottom=rd, label="compute")
    ax.bar(labels, wr, bottom=[x + y for x, y in zip(rd, cp)], label="write")
    ax.set_xlabel("OpenMP threads")
    ax.set_ylabel("time (ms)")
    ax.set_title(f"Time per phase: {op0}", fontsize=10)
    ax.legend(fontsize=8)
    fig.tight_layout()
    fig.savefig(os.path.join(a.outdir, "phases.png"), dpi=150)
    plt.close(fig)
    print(f"graphs written to {a.outdir}/speedup.png and {a.outdir}/phases.png")
except ImportError:
    print("matplotlib not installed: run `pip3 install matplotlib` and re-run for graphs "
          "(the CSVs are already saved).")