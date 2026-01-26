import numpy as np

# =========================
# パラメータ
# =========================
alpha = 100.0
rho = 0.8
speed_min = 60
speed_max = 255

d_safe = 40.0
d_min  = 10.0

# 横逃げ
gamma_turn = 0.5

# 慣性
vel_rho = 0.7

# =========================
# 出力減衰（ソフト番・自己保護）
# =========================
output_norm_th = 0.8
decay_down = 2.0
decay_up   = 0.8
decay_min  = 0.25

# =========================
# センサ方向
# =========================
dirs = {
    "LU": np.array([ 1.0, -1.0]),
    "RU": np.array([-1.0, -1.0]),
    "RD": np.array([-1.0,  1.0]),
    "LD": np.array([ 1.0,  1.0]),
}
for k in dirs:
    dirs[k] /= np.linalg.norm(dirs[k])

# =========================
# 状態
# =========================
dist_f_prev = {k: None for k in dirs}
v_prev = np.zeros(2)
output_decay = 1.0

# =========================
# メイン
# =========================
def calc_displacement(current_x, current_y, LU, RU, RD, LD):
    global dist_f_prev, v_prev, output_decay

    # -------------------------
    # 距離LPF
    # -------------------------
    dist_raw = {"LU": LU, "RU": RU, "RD": RD, "LD": LD}
    dist = {}

    for k in dist_raw:
        if dist_f_prev[k] is None:
            dist[k] = dist_raw[k]
        else:
            dist[k] = rho * dist_f_prev[k] + (1 - rho) * dist_raw[k]
        dist_f_prev[k] = dist[k]

    # -------------------------
    # 手からの反発
    # -------------------------
    v_hand = np.zeros(2)
    d_closest = 1e6

    for k in dirs:
        d = dist[k]
        if d <= 0 or d >= d_safe:
            continue
        w = (d_safe - d) / d_safe
        v_hand += w * dirs[k]
        d_closest = min(d_closest, d)

    v_escape = -v_hand

    # 横逃げ
    if np.linalg.norm(v_escape) > 1e-6:
        v_perp = np.array([-v_escape[1], v_escape[0]])
        v_escape = v_escape + gamma_turn * v_perp

    # -------------------------
    # 出力強度に基づく減衰（壁非依存）
    # -------------------------
    out_norm = np.linalg.norm(v_escape)

    if out_norm > output_norm_th:
        output_decay *= decay_down
    else:
        output_decay += decay_up

    output_decay = np.clip(output_decay, decay_min, 1.0)

    v_total = v_escape * output_decay

    norm = np.linalg.norm(v_total)
    if norm < 1e-6:
        return current_x, current_y, 0

    dir_vec = v_total / norm

    # -------------------------
    # 慣性
    # -------------------------
    v_prev = vel_rho * v_prev + (1 - vel_rho) * dir_vec
    v_dir = v_prev / (np.linalg.norm(v_prev) + 1e-6)

    # -------------------------
    # 速度
    # -------------------------
    if d_closest < 1e5:
        s = np.clip((d_safe - d_closest) / (d_safe - d_min), 0.0, 1.0)
    else:
        s = 0.0

    speed = speed_min + s * (speed_max - speed_min)

    # -------------------------
    # 位置更新
    # -------------------------
    target_x = current_x + alpha * v_dir[0] * speed * 0.01
    target_y = current_y + alpha * v_dir[1] * speed * 0.01

    return int(target_x), int(target_y), int(speed)
