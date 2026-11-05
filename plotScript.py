import matplotlib.pyplot as plt
import numpy as np

# -------------------------------
# 1. Define benchmark data
# -------------------------------
metrics = [
    {
        "title": "Solve Time (us/puzzle)",
        "unit": "us/puzzle",
        "values": [29, 2.71, 2.29, 1.64],
        "reduction": True
    },
    {
        "title": "Cycles (Billion)",
        "unit": "Cycles (Billion)",
        "values": [6.158, 0.568, 0.479, 0.342],
        "reduction": True
    },
    {
        "title": "Instructions (Billion)",
        "unit": "Instructions (Billion)",
        "values": [5.410, 1.217, 0.692, 1.075],
        "reduction": True
    },
    {
        "title": "L1D Misses (Thousand)",
        "unit": "Thousand",
        "values": [562.052, 89.774, 65.249, 73.712],
        "reduction": True
    },
    {
        "title": "LLC Loads (Thousand)",
        "unit": "Thousand",
        "values": [50.521, 4.648, 2.892, 2.758],
        "reduction": True
    },
    {
        "title": "LLC Misses (Thousand)",
        "unit": "Thousand",
        "values": [6.117, 1.070, 0.563, 0.717],
        "reduction": True
    },
    {
        "title": "Branch Instructions (Billion)",
        "unit": "Billion",
        "values": [1.71, 0.72, 0.033, 0.059],
        "reduction": True
    },
    {
        "title": "Branch Misses (Million)",
        "unit": "Million",
        "values": [194.626, 9.543, 9.842, 0.105],
        "reduction": True
    }
]

versions = ["V1", "V2", "V2b", "V2c"]
colors = ["#3478C4", "#4BA65D", "#9360D4", "#FF0000"]

# -------------------------------
# 2. Configure the figure
# -------------------------------
fig, axes = plt.subplots(4, 2, figsize=(12, 13))
fig.patch.set_facecolor("#F7F8F8")

fig.suptitle(
    "Performance Metrics Comparison: V1 vs V2 vs V2b vs V2c",
    fontsize=19,
    fontweight="bold",
    y=0.98
)

# -------------------------------
# 3. Draw each chart
# -------------------------------
for ax, metric in zip(axes.flat, metrics):

    values = metric["values"]
    x = np.arange(len(versions))

    bars = ax.bar(
        x, values,
        color=colors,
        width=0.75,
        edgecolor="white",
        linewidth=0.6
    )

    ax.set_title(
        metric["title"],
        fontsize=13,
        fontweight="bold",
        pad=10
    )

    ax.set_xticks(x)
    ax.set_xticklabels(versions, fontsize=10)
    ax.set_ylabel(metric["unit"], fontsize=10)

    ax.grid(
        axis="y",
        linestyle="-",
        alpha=0.25
    )
    ax.set_axisbelow(True)

    # Add space above the bars for labels
    ymax = max(values)
    ax.set_ylim(0, ymax * 1.30)

    # Value labels
    for bar, value in zip(bars, values):
        ax.annotate(
            f"{value:g}",
            xy=(
                bar.get_x() + bar.get_width() / 2,
                bar.get_height()
            ),
            xytext=(0, 3),
            textcoords="offset points",
            ha="center",
            va="bottom",
            fontsize=10,
            fontweight="bold"
        )

    # Percentage reduction from V1 to V3
    reduction = (1 - values[3] / values[0]) * 100

    ax.text(
        0.60, 0.94,
        f"Reduction: {reduction:.1f}%\n(V1 → V2c)",
        transform=ax.transAxes,
        ha="center",
        va="top",
        fontsize=9,
        bbox=dict(
            boxstyle="round,pad=0.5",
            facecolor="#E6EDF5",
            edgecolor="none",
            alpha=0.95
        )
    )

    # Clean up the axes
    ax.spines["top"].set_alpha(0.35)
    ax.spines["right"].set_alpha(0.35)
    ax.spines["left"].set_alpha(0.5)
    ax.spines["bottom"].set_alpha(0.5)

# -------------------------------
# 4. Add a summary at the bottom
# -------------------------------
# fig.text(
#     0.5, 0.025,
#     "Average Solve Time (us/puzzle):   "
#     "V1: 29   |   V2: 3.5   |   V3: 2.6",
#     ha="center",
#     fontsize=11,
#     fontweight="bold",
#     color="#17294D"
# )

plt.tight_layout(rect=[0.03, 0.055, 0.98, 0.96])

# Save a high-resolution image
plt.savefig(
    "images/performance_comparison_v2c.png",
    dpi=300,
    bbox_inches="tight",
    facecolor=fig.get_facecolor()
)

plt.show()
