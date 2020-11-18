#!/usr/bin/env python3
from matplotlib import pyplot as plt
from numpy import array as arr

class Result2:
    instances = []
    def __init__(self):
        self.best            = []
        self.avg             = []
        self.std             = []
        self.worst           = []
        self.time            = []
        self.time_quality    = []
        self.steps           = []
        self.checked         = []

    def __repr__(self):
        return str(self.steps)

    def update(self, vals):
        assert len(vals) == 8
        self.best.append(float(vals[0]))
        self.avg.append(float(vals[1]))
        self.std.append(float(vals[2]))
        self.worst.append(float(vals[3]))
        self.time.append(float(vals[4]))
        self.time_quality.append(float(vals[5]))
        self.steps.append(float(vals[6]))
        self.checked.append(float(vals[7]))



def exp2():
    with open("results/exp2.csv") as f:
        lines = f.readlines()[1:]

    results = {alg: Result2() for alg in ['H', 'G', 'S', 'R', 'RW']}
    print(id(results['H']))
    print(id(results['G']))
    for l in lines:
        words = l.split(";")
        if words[0] == "INSTANCE":
            Result2.instances.append(words[1].strip())
        else:
            results[words[0]].update(words[1:])

    plt.rcParams["figure.figsize"] = (10,5)
    exp2best(results)
    exp2avg(results)
    exp2worst(results)
    exp2time(results)
    exp2time_quality(results)
    exp2steps(results)
    exp2checked(results)

def exp2best(results):
    plt.clf()
    x = Result2.instances
    for alg, result in results.items():
        plt.plot(x, result.best, label = alg)
    plt.xlabel("Instancje")
    plt.ylabel("Jakość")
    plt.legend()
    plt.savefig("plots/exp2best.pdf", format="pdf") 

def exp2avg(results):
    plt.clf()
    x = Result2.instances
    for alg, result in results.items():
        plt.plot(x, result.avg, label = alg)
        plt.fill_between(x, arr(result.avg) - arr(result.std), arr(result.avg) + arr(result.std), alpha = 0.5)
    plt.xlabel("Instancje")
    plt.ylabel("Jakość")
    plt.legend()
    plt.savefig("plots/exp2avg.pdf", format="pdf") 

def exp2worst(results):
    plt.clf()
    x = Result2.instances
    for alg, result in results.items():
        plt.plot(x, result.worst, label = alg)
    plt.xlabel("Instancje")
    plt.ylabel("Jakość")
    plt.legend()
    plt.savefig("plots/exp2worst.pdf", format="pdf") 

def exp2time(results):
    plt.clf()
    plt.ylabel('log')
    x = Result2.instances
    for alg, result in results.items():
        plt.plot(x, result.time, label = alg)
    plt.xlabel("Instancje")
    plt.ylabel("Czas działania")
    plt.yscale('log')
    plt.legend()
    plt.savefig("plots/exp2time.pdf", format="pdf") 

def exp2time_quality(results):
    plt.clf()
    plt.ylabel('log')
    x = Result2.instances
    for alg, result in results.items():
        plt.plot(x, result.time, label = alg)
    plt.xlabel("Instancje")
    plt.ylabel("Jakość w czasie")
    plt.yscale('log')
    plt.legend()
    plt.savefig("plots/exp2time_quality.pdf", format="pdf") 

def exp2steps(results):
    plt.clf()
    x = Result2.instances
    for alg, result in results.items():
        if alg in 'GS':
            plt.plot(x, result.steps, label = alg)
    plt.xlabel("Instancje")
    plt.ylabel("Liczba kroków")
    plt.legend()
    plt.savefig("plots/exp2steps.pdf", format="pdf") 

def exp2checked(results):
    plt.clf()
    x = Result2.instances
    for alg, result in results.items():
        if alg != 'H':
            plt.plot(x, result.checked, label = alg)
    plt.xlabel("Instancje")
    plt.ylabel("Liczba sprawdzonych rozwiązań")
    plt.yscale('log')
    plt.legend()
    plt.savefig("plots/exp2checked.pdf", format="pdf") 



class Result3:
    instances = []
    def __init__(self):
        self.init  = []
        self.final = []

    def update(self, vals):
        self.init.append(float(vals[0]))
        self.final.append(float(vals[1]))

def exp3():
    with open("results/exp3.csv") as f:
        lines = f.readlines()[1:]

    results = {}
    n = 1
    plt.rcParams["figure.figsize"] = (10,13)
    for l in lines:
        words = l.split(";")
        if words[0] == "INSTANCE":
            results = {alg: Result3() for alg in 'GS'}
            instance = words[1]
        elif words[0].strip() == "INSTANCE END":
            plt.subplot("22"+str(n))
            plt.scatter(results['G'].init, results['G'].final, label='G', s=4, c='blue')
            plt.scatter(results['S'].init, results['S'].final, label='S', s=4, c='orange')
            plt.xlabel("Jakosć początkowa")
            plt.ylabel("Jakosć końcowa")
            plt.title(instance)
            if n == 4: plt.legend()
            n+=1 
        else:
            results[words[0]].update(words[1:])
    plt.savefig("plots/exp3.pdf", format = 'pdf')
    

    

def main():
    #exp2()
    exp3()

if __name__ == '__main__':
    main()
