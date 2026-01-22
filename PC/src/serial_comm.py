import serial
import time

class SerialComm:
    def __init__(self, port, baud):
        self.ser = serial.Serial(port, baud, timeout=0.1)
        time.sleep(2)

# 送るパラメータは仮
    def send_move(self, x, y, speed):
        cmd = f"{x},{y},{speed}\n"
        self.ser.write(cmd.encode())

# 4つの超音波センサの値を受け取る＋現在座標
    def read_data(self):
        line = self.ser.readline().decode('utf-8').strip()
        if not line:
            return None

        return tuple(map(int, line.split(',')))


# Arduino の Serial.print / println をリアルタイムで表示
    def monitor_serial(self):
        while self.ser.in_waiting>0:
            line = self.ser.readline().decode(errors="ignore").strip()
            print(f"[RECV] {line}")