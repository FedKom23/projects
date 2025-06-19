#объявляем все необходимые библиотеки
from vispy import app, scene
from vispy.geometry import Rect
import numpy as np
from funcs import (
    init_boids, directions, propagate, periodic_walls, flocking, distance
)
"""
объявляем начальные параметры
w-ширина
h-высота 
N-количество птиц
dt-временной интервал
perception-радиус взаимодействия
vrange-диапозон скоростей
"""
w, h = 1280, 720
N = 5000
dt = 0.1
asp = w / h
perception = 1 / 20
vrange = (0.02, 0.08)
#создаем отдельный массив из шума стандартного нормального распределения
mean = [0, 0]
cov = [[1, 0], [0, 1]]
noises = np.random.multivariate_normal(mean, cov, N)
"""
массив коэффициентов взаимодействия между классами
и внутри классов
первые два массива это 1-1 и 2-2
последний это между классами
коэффициент шума добавлен отдельно в flocking
"""

#                    c      a    s      w
coeffs = np.array([[0.3, 0.2, 0.2, 0.1],
                   [0.3, 0.1, 0.1, 0.1],
                   [0.2, 0.2, 8, 0.1]])

boids = np.zeros((N, 7), dtype=np.float64)  # для каждого боида 7 значений - координата, скорость , ускорения для x и y и номер класса
#инициализируем массив птичек
init_boids(boids, asp, vrange=vrange)
# print(boids)
#инициализируем экран виспи, устанавливаем параметры, и инициализируем объект птиц в виде стрелок
canvas = scene.SceneCanvas(show=True, size=(w, h))
view = canvas.central_widget.add_view()
view.camera = scene.PanZoomCamera(rect=Rect(0, 0, asp, 1))
#делаем два цвета для разных классов розовый и красный
arrows_0 = scene.Arrow(arrows=directions(boids[boids[:, -1] == 0], dt),
                       arrow_color=(1, 0, 0, 1),
                       width=5,
                       arrow_size=5,
                       connect='segments',
                       parent=view.scene)
arrows_1 = scene.Arrow(arrows=directions(boids[boids[:, -1] == 1], dt),
                       arrow_color=(1, 0, 1, 1),
                       width=5,
                       arrow_size=5,
                       connect='segments',
                       parent=view.scene)

#инициализируем две переменные с текстом одна динамическая для фиксации фпс другая статичная с выводом начальных данных
text = scene.Text('', parent=canvas.scene, pos=(60, 10), color='white')
message_text = scene.Text(
    "\n\n\n\n" + " num boids " + str(N) + "\n   " + "  c     " + "a    " + "s    " + "w    " + "\n   " + str(coeffs[0]) \
    + "\n   " + str(coeffs[1])+"\n   "+ str(coeffs[2]), parent=canvas.scene, color='white', pos=(80, 30))

#функция обновления положения(и по сути движения отоброжаемого на экране)
def update(event):
    D = distance(
        boids)  # считаем попарное расстояние между всеми боидами, вынесено в отдельную функцию, так как cdist работает быстрее написанной функции с @njit
    flocking(boids, D, perception, coeffs, asp, vrange, noises)  # рассчитываем ускорение на следующий шаг
    propagate(boids, dt, vrange)  # рассчитываем движение
    periodic_walls(boids, asp)
    # arrows.set_data(arrows=directions(boids, dt))
    arrows_0.set_data(arrows=directions(boids[boids[:, -1] == 0], dt))
    arrows_1.set_data(arrows=directions(boids[boids[:, -1] == 1], dt))
    fps = canvas.fps
    text.text = 'FPS: {:.2f}'.format(fps)
    canvas.update()


if __name__ == '__main__':
    #запуск динамического экрана
    timer = app.Timer(interval=0, start=True, connect=update)
    #отоброжаем фпс
    canvas.measure_fps()
    app.run()


