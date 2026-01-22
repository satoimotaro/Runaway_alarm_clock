import numpy as np

alpha = 200.0 # ← 変位倍率
rho = 0.8 # ← フィルタ係数（0.7〜0.85推奨
speed_min = 60
speed_max = 255
d_safe = 40
d_min = 10

beta = 0.3
wall_safe = 15
wall = 45

dirs = {
    "LU": np.array([1, -1]),
    "RU": np.array([-1, -1]),
    "RD": np.array([-1, 1]),
    "LD": np.array([1, 1]),
}

dist_f_prev = {
    "LU": None,
    "RU": None,
    "RD": None,
    "LD": None,
}

def calc_displacement(current_x, current_y, LU, RU, RD, LD):
    global dist_f_prev

    dist_raw = {
    "LU": LU,
    "RU": RU,
    "RD": RD,
    "LD": LD,
    }
    
    dist = {}

    for k in dist_raw:
        if dist_f_prev[k] is None:
            dist[k] = dist_raw[k]
        else:
            dist[k] = rho * dist_f_prev[k] + (1 - rho) * dist_raw[k]

        dist_f_prev[k] = dist[k] 

    v_hand = np.zeros(2)

    for k in dirs:
        if dist[k] <= 0 or dist[k] >= d_safe:
            continue
        w = 1.0 / dist[k]
        v_hand += w * dirs[k]

    v_escape = -v_hand


    v_wall = np.zeros(2)

    # 左壁
    if 50-current_x < wall_safe:
        v_wall += (1 / max(55-current_x, 1e-6)) * np.array([-1, 0])
        print("Left wall proximity detected")

    # 右壁
    if current_x+50 < wall_safe:
        v_wall += (1 / max(current_x+55, 1e-6)) * np.array([1, 0])
        print("Right wall proximity detected")

    # 上壁
    if current_y+50 < wall_safe:
        v_wall += (1 / max(current_y+55, 1e-6)) * np.array([0, 1])
        print("Top wall proximity detected")

    # 下壁
    if 50-current_y < wall_safe:
        v_wall += (1 / max(55 - current_y, 1e-6)) * np.array([0, -1])
        print("Bottom wall proximity detected")


    v_total = v_escape + beta*v_wall

    norm = np.linalg.norm(v_total)


    if norm < 1e-6:
    # 逃げる必要なし → 停止 or そのまま
        # v_escape[:] = 0.0
        return current_x, current_y, 0
    # else:
        # v_total /= norm
        


    # # 0〜1 に正規化（近いほど1）
    # s = np.clip((d_safe - d_min) / (d_safe - d_min), 0.0, 1.0)

    # speed = speed_min + s * (speed_max - speed_min)

    # 位置更新（速度反映）
    target_x = min(max(current_x + alpha *  v_total[0],-wall),wall)
    target_y = min(max(current_y + alpha *  v_total[1],-wall),wall)
    speed = 200

    return int(target_x), int(target_y), int(speed)