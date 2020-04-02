import threading
import matplotlib.pyplot as plt

from time import sleep

data = []


def main():
    thread1 = threading.Thread(target=input_thread, name="Input thread")
    thread2 = threading.Thread(
        target=plot_thread, name="Plot thread", daemon=True)
    thread1.start()
    thread2.start()


def input_thread():
    while True:
        line = input()
        data.append([float(x) for x in line.split()])


def plot_thread():
    plt.ion()
    fig = plt.figure()
    while True:
        plt.plot(data, color="grey", linewidth=1)
        plt.draw()
        fig.canvas.flush_events()
        sleep(0.1)


if __name__ == "__main__":
    main()
