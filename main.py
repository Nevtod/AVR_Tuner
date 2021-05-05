from math import cos, sin, pi

import matplotlib.pyplot as plt
import matplotlib as mpl
import numpy as np
from numpy.fft import rfft, rfftfreq

vals = []
print("Hello world!")
helpBStr = bytes('v==', encoding='utf-8')
helpInt = int.from_bytes(helpBStr, 'little')

cnt = 0
barr = bytearray()
bArr2 = bytearray()

with open('/home/vlad/Workspace/PycharmProjects/scientificProject/data.in',
          'rb') as input:  # "~/Workspace/MC/AVR_Tuner/Tuner/data.in", "rb") as input:
    while cnt < 1:
        cnt += 1
        barr = input.read()
        if len(barr) == 0:
            break  # end of file

        print(barr)
        print('sheet0')
        for i in range(10, len(barr) - 20, 1):

            if int.from_bytes(barr[i: i + 3], 'little') == helpInt:
                i += 3
                j = i
                while int.from_bytes(barr[j: j + 3], 'little') != helpInt:
                    bArr2.append(barr[j])
                    j += 1

                vals.append(int.from_bytes(barr[i: j], 'little'))
                i = j - 1


for i in range(1, len(vals) - 1):
    if abs(vals[i] - vals[i - 1]) > 50:
        vals[i] = (vals[i + 1] + vals[i - 1]) / 2
print("bArr2: ", bArr2)
print("vals: ", vals)
for i in range(1, len(vals)):
    if abs(vals[i]) > 800:
        vals[i] = 800

mpl.rcParams['font.size'] = 20 # Управление стилем, в данном случаем - размером шрифта

# y = b + k

fig, ax = plt.subplots()

# Подписываем оси и график
ax.set_title(r"Сигнал")
ax.set_ylabel(r"Значение")
ax.set_xlabel(r" секунд прошло")

fig.set_figwidth(20)
fig.set_figheight(20)
N = len(vals) # sec / 1000 = sec
start = 0
xs = np.linspace(0.001 * start, 0.001 * min(N + start, len(vals)), min(N, len(vals) - start))
npVals = np.array(vals[start: min(len(vals), start + N)])
print("npVals:",npVals)

npVals = np.floor(npVals)
plt.plot(xs, npVals)


def tricksON_and_show(plt, ax):
    ax.minorticks_on()
    #  Определяем внешний вид линий основной сетки:
    ax.grid(which='major',
            color='k',
            linewidth = 0.5,
           alpha = 0.5)

    #  Определяем внешний вид линий вспомогательной
    #  сетки:
    ax.grid(which='minor',
            color = 'k',
            linestyle = ':')
    #plt.savefig("/home/vlad/PycharmProjects/Labs/grapic.png")
    plt.show()
tricksON_and_show(plt, ax)


# FFT

FD = 1000000 / 1024
start = 6300
end = 6800
signal = npVals[start: end]
mid = np.sum(signal) / np.size(signal)
signal -= mid
plt.plot(xs[start: end], signal)
plt.show()

# rfftfreq сделает всю работу по преобразованию номеров элементов массива в герцы
# нас интересует только спектр амплитуд, поэтому используем abs из numpy (действует на массивы поэлементно)
# делим на число элементов, чтобы амплитуды были в милливольтах, а не в суммах Фурье. Проверить просто — постоянные составляющие должны совпадать в сгенерированном сигнале и в спектре
spectrum = rfft(signal)
plt.plot(rfftfreq((end - start), 1./FD), np.abs(spectrum)/(end - start))
plt.xlabel(u'Частота, Гц')
plt.ylabel(u'Напряжение, мВ')
plt.title(u'Спектр')
tricksON_and_show(plt, ax)




def get_spectrum_stick(freq):
    T = (end - start)
    An = 0
    Bn = 0
    for t in range(len(signal)):
        An += signal[t] * np.cos(2 * pi * freq * (t / FD))
        Bn += signal[t] * np.sin(2 * pi * freq * (t / FD))

    # An /= T
    # Bn /= T
    #return np.sqrt(np.abs(An)**2 + np.abs(Bn)**2)
    return np.abs(Bn)

frequences = np.linspace(0, 400, 401 * 20)
spectrum = get_spectrum_stick(frequences)

plt.plot(frequences, spectrum)
plt.xlabel(u'Частота, Гц')
plt.ylabel(u'Напряжение, мВ')
plt.title(u'Спектр(второй способ)')
tricksON_and_show(plt, ax)