import taichi as ti
import taichi_glsl as ts
from base_shader import BaseShader
import time

"""
Задача А
Шейдер "Барбер" состоит из:
    двух дубликатов пар координат:
        uv:
            закрашиваем полотно равными полосами(синими, красными, белыми) ширины 0.125
            поварачиваем и делаем динамическое движение по диагонали
        uv0:
            Дополнительные координаты, которые нужны для наложения черных вертикальных 
            полос по бокам полотна и затемнения относительно центра с помощью smoorthstep, чтобы придать скругленность 
            "цветной" динамической картинке
    
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


class Shader(BaseShader):
    #делаем внешний конструктор с теми же параметрами
    #title-разрешение
    #res-разрешение
    #gamma-степенной коэф для цвета
    def __init__(self,
                 title: str,
                 res: tuple[int, int] | None = None,
                 gamma: float = 2.2
                 ):
        super().__init__(title, res=res, gamma=gamma)

    @ti.func
    def main_image(self, fragCoord, t):
        """
        :param fragCoord - пара координат:
        :param t-время:
        :return col- цвет:
        """
        #берем две пары координат uv и uv0
        uv = fragCoord / ts.vec2(self.res[0], self.res[1])
        uv0 = fragCoord / ts.vec2(self.res[0], self.res[1])
        uv = ts.fract(rot(-2.) @ uv + (-0.2) * t)#поварачиваем одну пару координат и перемещаем координаты по диагонали
        aspi = self.resf[0] / self.resf[1]
        #в этих же координатах uv задаем полосы разных цветов(красные, белые, синие) шириной 0.125
        col = ts.vec3(0., 0., 0.)
        if 0.25 > uv.y > 0.125 or 0.75 > uv.y > 0.625:
            col = ts.vec3(0, 0, 1)
        if 0.125 > uv.y > 0. or 0.375 > uv.y > 0.25 or 0.625 > uv.y > 0.5 or 0.875 > uv.y > 0.75:
            col = ts.vec3(1, 1, 1)
        if 0.5 > uv.y > 0.375 or 1 > uv.y > 0.875:
            col = ts.vec3(1, 0, 0)
        #в неизменненых координатах делаем черные полосы побокам и делаем плавное затемнение с помощью smothstep(функция Эрмита)
        if 0.2 > uv0.x > 0. or 1 > uv0.x > 0.8:
            col = ts.vec3(0., 0., 0.)
        col *= ts.smoothstep(0.07, 0, ti.abs(uv0.x - 0.3 * aspi))
        return col


if __name__ == "__main__":
    ti.init(arch=ti.gpu)

    shader = Shader(
        "barber",
        res=(int(600 * (16 / 9)), int(600)),
        gamma=1.
    )

    shader.main_loop()
