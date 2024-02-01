import matplotlib.pyplot as plt

def load_data(filename):
    with open(filename, 'r') as file:
        data = [line.strip().split(',') for line in file]
    return [(float(H), float(R)) for a, b, S, H, R in data]

def plot_data(all_data, pareto_data):
    plt.scatter(*zip(*all_data), label='All Solutions')
    plt.scatter(*zip(*pareto_data), color='red', label='Pareto Front')
    plt.xlabel('H')
    plt.ylabel('R')
    plt.title('Pareto Front and All Solutions')
    plt.legend()
    plt.show()

all_solutions = load_data('all_solutions.txt')
pareto_solutions = load_data('pareto_front.txt')

plot_data(all_solutions, pareto_solutions)