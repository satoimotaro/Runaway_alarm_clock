import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

import core_XY as cxy

# =========================
# シミュレーション設定
# =========================
dt = 0.05
robot_pos = np.array([0.0, 0.0])
trail = []

hand_pos = np.array([60.0, 0.0])  # マウスで更新

# センサFOV設定
SENSOR_FOV = np.deg2rad(30)
NEAR = 30.0
FAR  = 60.0

WALL = 50

# =========================
# センサ計算
# =========================
def calc_sensors(robot, hand):
    rel = hand - robot
    dist = np.linalg.norm(rel)
    if dist < 1e-6:
        return {k: 20 for k in cxy.dirs}

    dir_h = rel / dist
    sensors = {}

    for k, dvec in cxy.dirs.items():
        cosang = np.dot(dir_h, dvec)

        if cosang < np.cos(SENSOR_FOV):
            sensors[k] = 100
            continue

        if dist < NEAR:
            sensors[k] = 20
        elif dist < FAR:
            sensors[k] = 50
        else:
            sensors[k] = 100

    return sensors

# =========================
# matplotlib 初期化
# =========================
fig, ax = plt.subplots(figsize=(6, 6))
ax.set_xlim(-100, 100)
ax.set_ylim(-100, 100)
ax.set_aspect("equal")
ax.grid(True)

robot_dot, = ax.plot([], [], "ko")
hand_dot,  = ax.plot([], [], "ro")
vec_arrow = None
trail_line, = ax.plot([], [], "b-", alpha=0.4)

# =========================
# マウス操作
# =========================
def on_mouse(event):
    if event.xdata is not None:
        hand_pos[:] = event.xdata, event.ydata

fig.canvas.mpl_connect("motion_notify_event", on_mouse)

# =========================
# 更新ループ
# =========================
def update(frame):
    global robot_pos, vec_arrow

    sensors = calc_sensors(robot_pos, hand_pos)

    tx, ty, _ = cxy.calc_displacement(
        robot_pos[0], robot_pos[1],
        sensors["LU"], sensors["RU"],
        sensors["RD"], sensors["LD"]
    )
    print(sensors["LU"],sensors["RU"],sensors["RD"],sensors["LD"], tx,ty)

    target = np.array([tx, ty])
    move = 0.2 * (target - robot_pos)
    
    new_pos = robot_pos + move

    # 壁制限
    new_pos[0] = np.clip(new_pos[0], -WALL, WALL)
    new_pos[1] = np.clip(new_pos[1], -WALL, WALL)

    robot_pos[:] = new_pos

    trail.append(robot_pos.copy())
    if len(trail) > 200:
        trail.pop(0)
    
    robot_dot.set_data([robot_pos[0]], [robot_pos[1]])
    hand_dot.set_data([hand_pos[0]], [hand_pos[1]])


    xs = [p[0] for p in trail]
    ys = [p[1] for p in trail]
    trail_line.set_data(xs, ys)

    if vec_arrow:
        vec_arrow.remove()
    vec_arrow = ax.arrow(
        robot_pos[0], robot_pos[1],
        move[0]*5, move[1]*5,
        color="blue",
        head_width=3
    )

    return robot_dot, hand_dot, trail_line

ani = FuncAnimation(fig, update, interval=int(dt*1000))
plt.title("CoreXY 2D Escape Simulation (move mouse)")
plt.show()
