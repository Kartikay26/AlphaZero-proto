import time
import threading
import matplotlib.pyplot as plt

lgd = []
data = []

def main():
    thread1 = threading.Thread(target=input_thread, name="Input thread")
    thread2 = threading.Thread(
        target=plot_thread, name="Plot thread", daemon=True)
    thread1.start()
    thread2.start()


def input_thread():
    global lgd
    lgd = input().split()
    for _ in range(len(lgd)):
        data.append([])
    while True:
        line = input()
        line = [float(x) for x in line.split()]
        for i in range(len(line)):
            data[i].append(line[i])


def plot_thread():
    plt.ion()
    plt.style.use("seaborn")
    fig = plt.figure()
    plt.legend()
    print(lgd)
    while True:
        for i in range(len(lgd)):
            plt.plot(data[i], linewidth=1, label=lgd[i])
        plt.legend()
        plt.draw()
        time.sleep(0.1)
        fig.canvas.flush_events()
        plt.clf()


if __name__ == "__main__":
    main()
