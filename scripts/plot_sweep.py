import csv, sys, statistics as st
from collections import defaultdict
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

src = sys.argv[1] if len(sys.argv) > 1 else "results/sweep.csv"
rows = list(csv.DictReader(open(src)))
vals = defaultdict(list)
for r in rows:
    for k in ("read_ms", "compute_ms", "write_ms", "total_ms"):
        vals[(r["impl"], r["image"], r["op"], int(r["threads"]), k)].append(float(r[k]))
M = {k: st.median(v) for k, v in vals.items()}          # median of the repeats

OPS = ["gaussFilter", "otsuBinarize", "sobelFilter"]
IMGS = ["aya1024.pgm", "aya2048.pgm", "aya4096.pgm"]
SIZE = {"aya1024.pgm": "1024²", "aya2048.pgm": "2048²", "aya4096.pgm": "4096²"}
T = list(range(1, 11))
COL = {"gaussFilter": "#2a78d6", "otsuBinarize": "#eb6834", "sobelFilter": "#1baf7a"}
MARK = {"gaussFilter": "o", "otsuBinarize": "s", "sobelFilter": "^"}
SIZECOL = {"aya1024.pgm": "#6da7ec", "aya2048.pgm": "#2a78d6", "aya4096.pgm": "#104281"}

def speedup(img, op, t, key="total_ms"):
    """Speedup relative to the best sequential program (bestseq, 1 thread)."""
    return M[("bestseq", img, op, 1, key)] / M[("parallel", img, op, t, key)]

# Summary table
with open("results/sweep_summary.csv", "w", newline="") as f:
    w = csv.writer(f)
    w.writerow(["image", "op", "threads", "bestseq_total_ms", "parallel_total_ms", "speedup",
                "efficiency", "read_ms", "compute_ms", "write_ms", "compute_speedup"])
    for img in IMGS:
        for op in OPS:
            for t in T:
                s = speedup(img, op, t)
                w.writerow([img, op, t,
                            round(M[("bestseq", img, op, 1, "total_ms")], 1),
                            round(M[("parallel", img, op, t, "total_ms")], 1),
                            round(s, 2), round(s / t, 2),
                            *[round(M[("parallel", img, op, t, k)], 1) for k in ("read_ms", "compute_ms", "write_ms")],
                            round(speedup(img, op, t, "compute_ms"), 2)])

def frame(ax, ylabel):
    ax.plot(T, T, color="#8a8a85", linestyle="--", linewidth=1, label="Ideal (linear)")
    ax.axvline(6.5, color="#b5b4ad", linestyle=":", linewidth=1)
    ax.text(6.6, 9.6, "efficiency cores\nfrom thread 7", fontsize=8, color="#5f5e5a", va="top")
    ax.set_xticks(T)
    ax.set_xlim(0.7, 10.6)
    ax.set_ylim(0, 10.5)
    ax.set_xlabel("Threads")
    ax.set_ylabel(ylabel)
    ax.grid(True, color="#e6e5e0", linewidth=0.6)
    for side in ("top", "right"):
        ax.spines[side].set_visible(False)

def by_operation(key, title, fname):
    fig, ax = plt.subplots(figsize=(6.4, 4.4))
    frame(ax, "Speedup vs best sequential")
    for op in OPS:
        ys = [speedup("aya4096.pgm", op, t, key) for t in T]
        ax.plot(T, ys, color=COL[op], marker=MARK[op], markersize=5, linewidth=2, label=op)
        ax.text(10.15, ys[-1], f"{ys[-1]:.1f}×", fontsize=8, color="#3d3d3a", va="center")
    ax.set_title(title, fontsize=11, loc="left")
    ax.legend(frameon=False, fontsize=8, loc="upper left")
    fig.tight_layout()
    fig.savefig(fname, dpi=200)

by_operation("total_ms", "Whole-program speedup, 4096² image", "results/speedup_4096.png")
by_operation("compute_ms", "Compute-phase speedup, 4096² image", "results/compute_speedup_4096.png")

# Scaling with image size: one panel per operation
fig, axes = plt.subplots(1, 3, figsize=(11, 3.8), sharey=True)
for ax, op in zip(axes, OPS):
    frame(ax, "Speedup vs best sequential" if op == OPS[0] else "")
    for img in IMGS:
        ax.plot(T, [speedup(img, op, t) for t in T], color=SIZECOL[img], marker="o",
                markersize=4, linewidth=2, label=SIZE[img])
    ax.set_title(op, fontsize=10, loc="left")
axes[0].legend(frameon=False, fontsize=8, loc="upper left", title="Image size", title_fontsize=8)
fig.tight_layout()
fig.savefig("results/speedup_by_size.png", dpi=200)

for op in OPS:
    print(f"{op:13s} 4096²: 6 threads {speedup('aya4096.pgm', op, 6):.2f}×, "
          f"10 threads {speedup('aya4096.pgm', op, 10):.2f}×")