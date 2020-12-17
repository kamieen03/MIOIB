#!/usr/bin/env python3
from matplotlib import pyplot as plt
from numpy import array as arr
from scipy.stats import pearsonr
import numpy as np
from copy import deepcopy

class Result2:
    instances = []
    def __init__(self):
        self.best            = []
        self.avg_score       = []
        self.std_score       = []
        self.worst           = []
        self.avg_time        = []
        self.std_time        = []
        self.time_quality    = []
        self.avg_steps       = []
        self.std_steps       = []
        self.avg_checked     = []
        self.std_checked     = []

    def update(self, vals):
        assert len(vals) == 11
        self.best.append(float(vals[0]))
        self.avg_score.append(float(vals[1]))
        self.std_score.append(float(vals[2]))
        self.worst.append(float(vals[3]))
        self.avg_time.append(float(vals[4]))
        self.std_time.append(float(vals[5]))
        self.time_quality.append(float(vals[6]))
        self.avg_steps.append(float(vals[7]))
        self.std_steps.append(float(vals[8]))
        self.avg_checked.append(float(vals[9]))
        self.std_checked.append(float(vals[10]))



def exp2():
    with open("results/exp2.csv") as f:
        lines = f.readlines()[1:]

    results = {alg: Result2() for alg in ['H', 'G', 'S', 'R', 'RW', 'SA', 'T']}
    for l in lines:
        words = l.split(";")
        if words[0] == "INSTANCE":
            Result2.instances.append(words[1].strip())
        else:
            results[words[0]].update(words[1:])

    plt.rcParams["figure.figsize"] = (10,5)
    colors = iter(['tab:red', 'tab:green', 'tab:blue', 'tab:orange', 'tab:purple', 'tab:cyan', 'tab:gray'])
    exp2best(results, deepcopy(colors))
    exp2avg(results, deepcopy(colors))
    exp2worst(results, deepcopy(colors))
    exp2time(results, deepcopy(colors))
    exp2time_quality(results, deepcopy(colors))
    exp2steps(results, deepcopy(colors))
    exp2checked(results, deepcopy(colors))

def exp2best(results, colors):
    plt.clf()
    x = Result2.instances
    for alg, result in results.items():
        c = next(colors)
        plt.plot(x, result.best, label = alg, c=c)
    plt.xlabel("Instancje")
    plt.ylabel("Jakość")
    plt.legend()
    plt.savefig("plots/exp2best.pdf", format="pdf", bbox_inches='tight')

def exp2avg(results, colors):
    plt.clf()
    x = Result2.instances
    for alg, result in results.items():
        c = next(colors)
        plt.plot(x, result.avg_score, label = alg, c=c)
        plt.fill_between(x, arr(result.avg_score) -
                arr(result.std_score), arr(result.avg_score) +
                arr(result.std_score), alpha = 0.5, color=c)
    plt.xlabel("Instancje")
    plt.ylabel("Jakość")
    plt.legend()
    plt.savefig("plots/exp2avg.pdf", format="pdf", bbox_inches='tight')

def exp2worst(results, colors):
    plt.clf()
    x = Result2.instances
    for alg, result in results.items():
        c = next(colors)
        plt.plot(x, result.worst, label = alg, c=c)
    plt.xlabel("Instancje")
    plt.ylabel("Jakość")
    plt.legend()
    plt.savefig("plots/exp2worst.pdf", format="pdf", bbox_inches='tight')

def exp2time(results, colors):
    plt.clf()
    x = [17,35,43,48,55,70,124,170,323,403]
    for alg, result in results.items():
        c = next(colors)
        plt.plot(x, result.avg_time, label = alg, c=c)
        plt.scatter(x, result.avg_time, c=c)
        plt.fill_between(x, arr(result.avg_time) -
                arr(result.std_time), arr(result.avg_time) + arr(result.std_time), alpha = 0.5, color=c)
    #print(arr(results['G'].avg_time) / arr(results['S'].avg_time))
    print(arr(results['G'].avg_time) / arr(x)**2)
    plt.xlabel("Wielkość instancji")
    plt.ylabel("Czas działania [ms]")
    plt.yscale('log')
    plt.xlim(1,410)
    plt.grid(True)
    plt.legend()
    plt.savefig("plots/exp2time.pdf", format="pdf", bbox_inches='tight')

def exp2time_quality(results, colors):
    plt.clf()
    plt.ylabel('log')
    x = [17,35,43,48,55,70,124,170,323,403]
    for alg, result in results.items():
        c = next(colors)
        plt.plot(x, result.time_quality, label = alg, c=c)
        plt.scatter(x, result.time_quality, c=c)
    plt.xlabel("Wielkość instancji")
    plt.ylabel("Jakość w czasie")
    plt.yscale('log')
    plt.xlim(1,410)
    plt.grid(True)
    plt.legend()
    plt.savefig("plots/exp2time_quality.pdf", format="pdf", bbox_inches='tight')

def exp2steps(results, colors):
    plt.clf()
    x = [17,35,43,48,55,70,124,170,323,403]
    for alg, result in results.items():
        c = next(colors)
        if alg in 'GS':
            plt.plot(x, result.avg_steps, label = alg, c=c)
            plt.fill_between(x, arr(result.avg_steps) -
                    arr(result.std_steps), arr(result.avg_steps) + arr(result.std_steps), alpha = 0.5, color=c)
            plt.scatter(x, result.avg_steps, c=c)
    plt.xlabel("Wielkość instancji")
    plt.ylabel("Liczba kroków")
    plt.xlim(1,410)
    plt.legend()
    plt.grid(True)
    plt.savefig("plots/exp2steps.pdf", format="pdf", bbox_inches='tight')

def exp2checked(results, colors):
    plt.clf()
    x = Result2.instances
    x = [17,35,43,48,55,70,124,170,323,403]
    for alg, result in results.items():
        c = next(colors)
        if alg != 'H':
            plt.plot(x, result.avg_checked, label = alg, c=c)
            plt.fill_between(x, arr(result.avg_checked) -
                    arr(result.std_checked), arr(result.avg_checked) +
                    arr(result.std_checked), alpha = 0.5, color=c)
            plt.scatter(x, result.avg_checked, c=c)
    plt.xlabel("Wielkość instancji")
    plt.ylabel("Liczba sprawdzonych rozwiązań")
    plt.yscale('log')
    plt.xlim(1,410)
    plt.grid(True)
    plt.legend()
    plt.savefig("plots/exp2checked.pdf", format="pdf", bbox_inches='tight')



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
    plt.rcParams["figure.figsize"] = (10,10)
    print('PCCs in exp3:')
    for l in lines:
        words = l.split(";")
        if words[0] == "INSTANCE":
            results = {alg: Result3() for alg in 'GS'}
            instance = words[1].strip()
        elif words[0].strip() == "INSTANCE END":
            print(f'{instance} G {pearsonr(results["G"].init, results["G"].final)[0]}')
            print(f'{instance} S {pearsonr(results["S"].init, results["S"].final)[0]}')
            plt.subplot("22"+str(n))
            plt.scatter(results['G'].init, results['G'].final, label='G', s=4, c='blue')
            plt.scatter(results['S'].init, results['S'].final, label='S', s=4, c='orange')
            if n in [3,4]: plt.xlabel("Jakość początkowa")
            if n in [1,3]: plt.ylabel("Jakość końcowa")
            plt.title(instance)
            if n == 4: plt.legend()
            n+=1 
        else:
            results[words[0]].update(words[1:])
    plt.subplots_adjust(hspace=0.45, wspace=0.35)
    plt.savefig("plots/exp3.pdf", format = 'pdf', bbox_inches='tight')
    

class Result4:
    instances = []
    def __init__(self):
        self.avg  = []
        self.best = []

    def update(self, vals):
        self.avg.append(float(vals[0]))
        self.best.append(float(vals[1]))

def exp4():
    with open("results/exp4.csv") as f:
        lines = f.readlines()[1:]

    results = {}
    n = 1
    plt.figure(figsize=(8,10))
    for l in lines:
        words = l.split(";")
        if words[0] == "INSTANCE":
            results = {alg: Result4() for alg in 'GS'}
            instance = words[1].strip()
        elif words[0].strip() == "INSTANCE END":
            plt.subplot("41"+str(n))
            plt.plot(list(range(1,301)), results['G'].avg, '--', label='G avg', c='blue')
            plt.plot(list(range(1,301)), results['S'].avg, '--', label='S avg', c='orange')
            plt.plot(list(range(1,301)), results['G'].best, '-', label='G best', c='blue')
            plt.plot(list(range(1,301)), results['S'].best, '-', label='S best', c='orange')
            if n == 4: plt.xlabel("Przebieg")
            plt.ylabel("Jakość")
            plt.title(instance)
            if n == 4: plt.legend()
            n+=1 
        else:
            results[words[0]].update(words[1:])
    plt.subplots_adjust(hspace=.45)
    plt.savefig("plots/exp4.pdf", format = 'pdf', bbox_inches='tight')
    

def exp5():
    with open("results/exp5.csv") as f:
        lines = f.readlines()[1:]
    
    n = 1
    plt.figure(figsize=(10,5))
    for l in lines:
        words = l.split(";")
        if words[0] == "INSTANCE":
            x, y = [], []
            instance = words[1].strip()
        elif words[0].strip() == "INSTANCE END":
            plt.subplot(1,2,n)
            plt.scatter(x, y, s=4)
            if n == 2: plt.xlabel("Jakość")
            plt.ylabel("Dystans")
            plt.title(instance)
            n+=1 
        else:
            x.append(float(words[0])) 
            y.append(float(words[1])) 
    plt.subplots_adjust(hspace=.45)
    plt.savefig("plots/exp5.pdf", format = 'pdf', bbox_inches='tight')
    

def expSA():
    with open("results/expSA.csv") as f:
        lines = f.readlines()[1:]

    results = {}
    C = None
    for l in lines:
        words = l.split(";")
        if words[0] == "C":
            C = float(words[1])
            results[C] = Result2()
        elif words[0] == "INSTANCE":
            if words[1].strip() not in Result2.instances:
                Result2.instances.append(words[1].strip())
        else:
            results[C].update(words[1:])

    plt.rcParams["figure.figsize"] = (10,5)
    plt.clf()
    x = Result2.instances
    for C, result in results.items():
        plt.plot(x, result.avg_score, label = C)
        #plt.fill_between(x, arr(result.avg_score) -
        #        arr(result.std_score), arr(result.avg_score) +
        #        arr(result.std_score), alpha = 0.5)
    plt.xlabel("Instancje")
    plt.ylabel("Jakość")
    plt.legend()
    plt.savefig("plots/expSA.pdf", format="pdf", bbox_inches='tight')

def expT():
    with open("results/expT.csv") as f:
        lines = f.readlines()[1:]

    results = {}
    E = None
    for l in lines:
        words = l.split(";")
        if words[0] == "E":
            E = float(words[1])
            results[E] = Result2()
        elif words[0] == "INSTANCE":
            if words[1].strip() not in Result2.instances:
                Result2.instances.append(words[1].strip())
        else:
            results[E].update(words[1:])

    plt.rcParams["figure.figsize"] = (10,5)
    plt.clf()
    x = Result2.instances
    for E, result in results.items():
        plt.plot(x, result.avg_score, label = E)
        #plt.fill_between(x, arr(result.avg_score) -
        #        arr(result.std_score), arr(result.avg_score) +
        #        arr(result.std_score), alpha = 0.5)
    plt.xlabel("Instancje")
    plt.ylabel("Jakość")
    plt.legend()
    plt.savefig("plots/expT.pdf", format="pdf", bbox_inches='tight')


def main():
    print("Generating exp2 plots")
    exp2()
    print("Generating exp3 plots")
    exp3()
    print("Generating exp4 plots")
    exp4()
    print("Generating exp5 plots")
    exp5()
    print("Generating expSA plots")
    expSA()
    print("Generating expT plots")
    expT()

if __name__ == '__main__':
    main()
