import taichi as ti
import taichi_glsl as ts
from base_shader import BaseShader
import math
"""
Задача Б
Шейдер "спираль - звездочка"
состоит из:
 sdf-звкздочки,
 затенения  контура путям последовательных применений smoorthstep(с двух сторон от нулевого расстояния)
  для дробной части d,
  выделения контура путем smooth_border,
  смешивания цветов в зависимости от расстояния,
  динамически изменяющейся радиус(делает вложенность звездочек),
  вращение пространства(спиралевидность)
  и отражение относительно x
"""


@ti.func
def rot(a):
    """
    Функция для расчета матрицы поворота.
    При помощи декоратора ti.func может выполняться на видеокарте.
    :param a: угол поворота в радианах
    :return: матрица поворота на угол a в двумерном пространстве
    """
    c = ti.cos(a)
    s = ti.sin(a)
    return ts.mat([c, -s], [s, c])


@ti.func
def clamp(x, low, high):
    """Фиксируем векторные значения между нижней и верхней границей."""
    return ti.max(ti.min(x, high), low)


@ti.func
def pal(t, a, b, c, d):
    """функция перемешивания цветов , также в зависимости от расстояние и времени"""
    return a + b * ti.cos((2 * math.pi) * (c * t + d))


@ti.func
def fract(x):
    """
    считаем дробную часть числа
    """
    return x - ti.floor(x)


@ti.func
def smoothstep(edge0, edge1, x):
    """
    напишем собственную функцию Эрмита-smoothstep, которая может работать в первых двух полях с переменными в виде координат
    """
    n = (x - edge0) / (edge1 - edge0)
    t = clamp(n, 0.0, 1.0)
    return t * t * (3.0 - 2.0 * t)


@ti.func
def smooth_border(p, a, w):
    return 1 - smoothstep(0, a, abs(p) - w)


@ti.func
def sd_star(p, r, n, m):
    """
    Вычисляет расстояние до звездообразной формы.
    :param p: Координаты точки.
    :param r: Радиус звездообразной формы.
    :param n: Количество лучей.
    :param m: Параметр между 2 и n.
    :return: Расстояние до звездообразной формы.
    """
    an = 3.141593 / n
    en = 3.141593 / m
    acs = ts.vec2(ti.cos(an), ti.sin(an))
    ecs = ts.vec2(ti.cos(en), ti.sin(en))

    bn = ((ti.atan2(p[0], p[1])) % (2.0 * an)) - an
    p_norm = ti.sqrt(p[0] ** 2 + p[1] ** 2)
    p = p_norm * ts.vec2(ti.cos(bn), ti.abs(ti.sin(bn)))
    p -= r * acs
    p += ecs * ti.max(ts.vec2(0.0, 0.0), -ts.dot(ts.vec2(p[0], p[1]), ecs)) * (r * acs[1] / ecs[1])
    return ti.sqrt(p[0] ** 2 + p[1] ** 2) * ts.sign(p[0])


class Shader(BaseShader):

    def __init__(self,
                 title: str,
                 res: tuple[int, int] | None = None,
                 gamma: float = 2.2,
                 count: int = 5
                 ):
        super().__init__(title, res=res, gamma=gamma)
        self.count = count

    @ti.func
    def main_image(self, fragCoord, t):
        """
        реализовать разноцветный канал в виде динамической формы звезды
        :param fragCoord- пара начальных координат:
        :param t - время:
        :return:
        """
        # задаем начальные координаты с масшатабми
        uv = fragCoord / self.resf
        pos = (ts.vec2(0.5) - uv)
        pos.x *= self.resf[0] / self.resf[1]
        pos = rot(t*1.)@pos #делаем спираливидную структуру путем поворота с той же частотой , с которой изменяется радиус
        pos.x = ti.abs(pos.x)#отражение относительно оси x
        r = 5-(ti.sin(t)/2)#динамически изменяющейся радиус
        d = sd_star(pos, r, self.count, self.count - 1)  # получаем расстояние звезды
        # сглаживание координат, используя интерполяцию Эрмита между дробными частями числа(чтобы задать темные внешние области канала).
        # и премешивания разных цветов
        # col = ts.vec3(1.)
        col = ts.vec3(1.)
        if 0<fract(d)<0.1:
            col = ts.vec3(0.4, 0.8, 0.9)
            col *= smooth_border(fract(d), 0.1, 0.05)#граница
        if 0.1 < fract(d):
            #затенение
            col = ts.vec3(smoothstep(fract(d), fract(d) - 0.200, 0.2))
            col-= ts.vec3(
                smoothstep(fract(d), fract(d) + 0.200, 0.2))
            #смешивание цветов
            col*= pal(d,
                        ts.vec3(0.7, 0.4, 0.4),
                        ts.vec3(0.6, 0.58, 0.005),
                        ts.vec3(1.0, 1.0, 1.0),
                        ts.vec3(0.31, 0.41, 0.15)
                                                    )
        return col


if __name__ == "__main__":
    ti.init(arch=ti.gpu)

    shader = Shader(
        "star",
        res=(int(600 * (16 / 9)), int(600)),
        count=5,
        gamma=1.
    )

    shader.main_loop()
