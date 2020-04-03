#!python3

import time
import threading
import matplotlib.pyplot as plt
from argparse import ArgumentParser

data = []

parser = ArgumentParser(
    prog="hist.py",
    description="Generate histogram of incoming numbers"
)
parser.add_argument("--timedelay", "-t", default=0.1, type=float)
parser.add_argument("--bins", "-b", default=10, type=int)

args = parser.parse_args()


def main():
    thread1 = threading.Thread(target=input_thread, name="Input thread")
    thread2 = threading.Thread(
        target=plot_thread, name="Plot thread", daemon=True)
    thread1.start()
    thread2.start()


def input_thread():
    while True:
        line = input()
        data.append(float(line))


def plot_thread():
    plt.ion()
    plt.style.use("seaborn")
    fig = plt.figure()
    while True:
        plt.hist(data, bins=args.bins)
        plt.draw()
        time.sleep(args.timedelay)
        fig.canvas.flush_events()
        plt.clf()


if __name__ == "__main__":
    main()
