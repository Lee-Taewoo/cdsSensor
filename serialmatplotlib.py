import serial
import matplotlib.pyplot as plt
import matplotlib.animation as ani
from collections import deque

ser = serial.Serial("COM3", 115200)

fig, ax = plt.subplots()
data = deque(maxlen = 300)
data2 = deque(maxlen = 300)
data3 = deque(maxlen = 300)
bar = ax.bar(0, 0)

def update_bar(frame):
    if ser.in_waiting > 0:
        line = ser.readline().decode().rstrip()
        print(line)
        try:
            value = float(line)
            bar[0].set_height(value)
            ax.set_ylim(3500, 4000)
        except ValueError:
            pass

def update_graph(frame):
    if ser.in_waiting> 0:
        line = ser.readline().decode().rstrip()
        print(line)
        try:
            value1, value2, value3 = map(float, line.split(','))
            data.append(value1)
            data2.append(value2)
            data3.append(value3)
            ax.clear()
            ax.plot(data)
            ax.plot(data2)
            ax.plot(data3)
        except ValueError:
            pass

an = ani.FuncAnimation(fig, update_graph, interval = 1, save_count = 50)
plt.show()

ser.close()
