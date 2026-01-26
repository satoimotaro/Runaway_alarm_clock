# test_coreXY.py
import numpy as np
import matplotlib.pyplot as plt

import core_XY as cxy

# =========================
# テスト条件
# =========================
current_pos = np.array([0.0, 0.0])

sensor_test = {
    "LU": 50.0,
    "RU": 99.0,
    "RD": 99.0,
    "LD": 99.0,
}

# =========================
# 計算
# =========================
tx, ty, speed = cxy.calc_displacement(
    current_pos[0],
    current_pos[1],
    sensor_test["LU"],
    sensor_test["RU"],
    sensor_test["RD"],
    sensor_test["LD"],
)

target_pos = np.array([tx, ty])

# =========================
# 可視化
# =========================
plt.figure(figsize=(6, 6))
plt.axis("equal")
plt.grid(True)
plt.xlim(-120, 120)
plt.ylim(-120, 120)

# 現在地
plt.scatter(*current_pos, color="black")
plt.text(current_pos[0], current_pos[1], " current")
plt.scatter(
    target_pos[0],
    target_pos[1],
    color="red",
    s=80,
    zorder=5
)
plt.text(
    target_pos[0] + 2,
    target_pos[1] + 2,
    "target",
    color="red",
    fontsize=10
)
# 移動ベクトル
plt.arrow(
    current_pos[0],
    current_pos[1],
    target_pos[0] - current_pos[0],
    target_pos[1] - current_pos[1],
    length_includes_head=True,
    head_width=3,
    color="blue"
)

# センサ方向
for k, d in sensor_test.items():
    p = cxy.dirs[k] * d
    plt.plot([0, p[0]], [0, p[1]], linestyle="--")
    plt.text(p[0], p[1], f"{k}:{int(d)}")

plt.title(f"CoreXY Escape Vector Test  speed={speed}")
plt.show()
