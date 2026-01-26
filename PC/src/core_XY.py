import numpy as np

# =========================
# パラメータ
# =========================
alpha = 100.0
rho = 0.8

speed_min = 60
speed_max = 255

d_safe = 60.0
d_min  = 10.0

# 横逃げ強度
gamma_turn = 0.01

# 慣性LPF
vel_rho = 0.2

# 減衰
alpha_parallel = 0.7
alpha_perp = 1.2

# =========================
# センサ方向（正規化）
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
v_prev_pal = np.zeros(2)
v_rd = np.zeros(2)

# =========================
# メイン
# =========================
def calc_displacement(current_x, current_y, LU, RU, RD, LD):
    global dist_f_prev, v_prev, v_rd, v_prev_pal

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
    no_threat = all(dist[k] >= d_safe for k in dist)
    if no_threat:
        v_prev *= 0.8
        v_rd = 0

        if np.linalg.norm(v_prev) < 1e-3:
            v_prev[:] = 0
            return current_x, current_y, 0


    # -------------------------
    # 手からの反発（勾配型）
    # -------------------------
    v_hand = np.zeros(2)
    d_closest = 1e6

    for k in dirs:
        d = dist[k]
        if d <= 0 or d >= d_safe:
            continue

        # 正規化距離勾配（非線形）
        x = (d_safe - d) / d_safe
        w = x * x   # 2乗で近距離を強調

        v_hand += w * dirs[k]
        d_closest = min(d_closest, d)

    # 主逃げ方向
    v_parallel = -v_hand
    v_perp = 0

    # -------------------------
    # 横逃げ（手ベクトル基準 + 上下バランス）
    # -------------------------
    n = np.linalg.norm(v_hand)
    if n > 1e-6:
        hand_dir = v_hand / n
        tangent = np.array([-hand_dir[1], hand_dir[0]])

        # 左右差
        side_lr = (dist["RU"] + dist["RD"]) - (dist["LU"] + dist["LD"])
        # 上下差（追加）
        side_ud = (dist["LD"] + dist["RD"]) - (dist["LU"] + dist["RU"])
        if(current_y > 0):side_lr*= -1
        if(current_x > 0):side_ud*= -1 
        
        # 合成判定
        side = side_lr - 0.5 * side_ud

        v_perp += gamma_turn * np.tanh(side / d_safe) * tangent

    # -------------------------
    # 減衰
    # -------------------------
    if(np.linalg.norm(v_parallel - v_prev_pal) < 1e-1):
        v_rd = 0
        print("direction changed")
    v_rd += v_parallel*(1 - alpha_parallel)
    v_parallel -= v_rd
    v_perp *= alpha_perp
    v_prev_pal = v_parallel
    v_escape = v_parallel + v_perp

    # -------------------------
    # 慣性（正規化前）
    # -------------------------
    v_prev = vel_rho * v_prev + (1 - vel_rho) * v_escape

    norm = np.linalg.norm(v_prev)
    if norm < 1e-6:
        return current_x, current_y, 0

    v_dir = v_prev / norm

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
