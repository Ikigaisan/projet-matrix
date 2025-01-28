import matplotlib.pyplot as plt
import numpy as np
import struct
import sys

vector_X = sys.argv[1]
vector_Y = sys.argv[2]
vector_a = sys.argv[3]

with open(vector_X, "rb") as file :
    m, = struct.unpack(">Q", file.read(8))
    X = np.zeros(m)
    for i in range(m) : X[i], = struct.unpack("d", file.read(8))
with open(vector_Y, "rb") as file :
    m, = struct.unpack(">Q", file.read(8))
    Y = np.zeros(m)
    for i in range(m) : Y[i], = struct.unpack("d", file.read(8))
with open(vector_a, "rb") as file :
    n, = struct.unpack(">Q", file.read(8))
    a = np.zeros(n)
    for i in range(n) : a[i], = struct.unpack("d", file.read(8))

x = np.linspace(np.min(X), np.max(X), 1000)
y = np.polyval(np.flip(a),x)
y2 = np.polyval(np.polyfit(X,Y,2),x)

plt.figure()
plt.scatter(X, Y)
plt.plot(x,y,label="Approximation aux moindres carrés - étudiant")
plt.plot(x,y2,label="Approximation aux moindres carrés - Python")
plt.legend()
plt.savefig("approximation.pdf")
plt.show()