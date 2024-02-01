N = 60
D = 6
F = .8
CR = 90
MAX_ITER = 50
TOL = 10**(-14)


import random
import math
def isti(x1, x2):
    for i in range(len(x1)):
        if not x1[i] == x2[i]: return False
    return True

X_kord = [-15, -10, -5, 0, 5, 10, 15,
            15, 15, 15, 15, 15,
          15, 10, 5, 0, -5, -10, -15,
            -15, -15, -15, -15, -15,
          ]
Y_kord = [
        -15, -15, -15, -15, -15, -15, -15,
            -10, -5, 0, 5, 10, 15,
            15, 15, 15, 15, 15,
          15, 10, 5, 0, -5, -10,
          ]
S = [ -0.09940112332480822, -0.09570265063923192, -0.07782620994584906,
 -0.044595775065571636, -0.008470411838648773, -0.0013292572938769093,
 -0.01402876134848341, 0.0011785680597112547, -0.0016096599564817682,
 -0.03141072397571561, -0.05773121434057853, -0.07098734083487862,
 -0.07421256224434619, -0.09674779542915338, -0.13216942328836218,
 -0.18406033359301877, -0.24214426775005213, -0.25978279767024376,
 -0.2186443973931424, -0.3289283483195699, -0.4205252223787085,
 -0.32130499477499636, -0.205134242990832, -0.13760381018149595]
def f(x):
    suma = 0
    A1 = x[4]
    A2 = x[5]
    Xp1 = x[0]
    Yp1 = x[1]
    Xp2 = x[2]
    Yp2 = x[3]
    for i in range(len(S)):
        try:

            suma += (
                      (A1 / math.dist([X_kord[i], Y_kord[i]],[Xp1,Yp1]) ) +
                      (A2 / math.dist([X_kord[i], Y_kord[i]],[Xp2,Yp2]) ) - S[i]

                      )**2
        except:
            return 99999999
    return suma

def iteracija(generacija):

    novaGeneracija = []
    #Iteriramo kroz svaki
    for agent in generacija:
        while True:
            xa = generacija[random.randint(0, N-1)]
            if not isti(agent, xa):
                break
        while True:
            xb = generacija[random.randint(0, N-1)]
            if not isti(agent, xb) and not isti(xa, xb):
                break
        while True:
            xc = generacija[random.randint(0, N-1)]
            if not isti(agent, xc) and not isti(xa, xc) and not isti(xb, xc):
                break

        z = []

        R = random.randint(0,D-1)
        for i in range(D):
            kandidatBroj = xa[i]+(xb[i]-xc[i])*F
            sansaI = random.randint(0, 99)
            if (R == i or sansaI < CR) and (kandidatBroj >= -15 and kandidatBroj <= 15):
                z.append(kandidatBroj)
            else:
                z.append(agent[i])
        Fz = f(z)
        Fagent = f(agent)

        if Fz <= Fagent:
            novaGeneracija.append(z)
        else:
            novaGeneracija.append(agent)

        if(Fz < TOL):
            print(Fz)
            print(z)
            return None
        if(Fagent < TOL):
            print(Fagent)
            print(agent)
            return None
    return novaGeneracija

generacija = []
for i in range(N):
    agent = []
    for j in range(D):
        agent.append(random.randint(0,30)-15)
    generacija.append(agent)



while True:
    novaGen = iteracija(generacija)
    if novaGen is None:
        break
    generacija = novaGen

    minCostF = 9999999999
    for agent in generacija:
        costF = f(agent)
        if(minCostF> costF):
            minCostF = costF