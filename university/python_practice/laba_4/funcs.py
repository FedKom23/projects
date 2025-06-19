import numpy as np
from scipy.spatial.distance import cdist
from numba import njit, prange


# замена для np.mean(axis=0), чтобы можно было скомпилировать функции
@njit
def mean_axis0(arr):
    n = arr.shape[1]
    res = np.empty(n, dtype=arr.dtype)
    for i in range(n):
        res[i] = arr[:, i].mean()
    return res


# инициируем оиды - задаем им первоначальные координаты, скорость и номер класса
def init_boids(boids: np.ndarray, asp: float,
               vrange: tuple = (0., 1.)):
    n = boids.shape[0]
    rng = np.random.default_rng()
    boids[:, 0] = rng.uniform(0., asp, size=n)
    boids[:, 1] = rng.uniform(0., 1., size=n)
    alpha = rng.uniform(0, 2 * np.pi, size=n)
    v = rng.uniform(*vrange, size=n)
    c, s = np.cos(alpha), np.sin(alpha)
    boids[:, 2] = v * c
    boids[:, 3] = v * s
    #получаем рандомное количество птиц в разных классах
    class_id = rng.uniform(0., 1., size=n)
    # print(class_id)
    #присваиваем класс в зависемости от значение больше или меньше 0.5
    boids[:, -1] = np.where(class_id > 0.5, 1, 0)


# для отрисовси стрелок. Стрелка рисуется во второй паре координат. А направление - от первых координат ко вторым
@njit
def directions(boids: np.ndarray,
               dt: float) -> np.ndarray:
    return np.hstack((
        boids[:, :2] - dt * boids[:, 2:4],
        boids[:, :2]
    ))


# искусственное ограничение скорости
@njit
def clip_mag(arr: np.ndarray, lims=(0., 1.)):
    # v = np.linalg.norm(arr, axis=1)
    v = np.sum(arr * arr, axis=1) ** 0.5
    mask = v > 0
    v_clip = np.clip(v, *lims)
    arr[mask] *= (v_clip[mask] / v[mask]).reshape(-1, 1)


"""
задаем движение
скорость - прибавляем dt умножить на ускорение
координаты-прибавляем dt умножить на скорость
"""
@njit
def propagate(boids: np.ndarray, dt: float, vrange: tuple):
    boids[:, 2:4] += dt * boids[:, 4:6]
    clip_mag(boids[:, 2:4], lims=vrange)
    boids[:, 0:2] += dt * boids[:, 2:4]

#если выходим за границы поля появляемся с другой стороны , беря остаток
@njit
def periodic_walls(boids: np.ndarray,
                   asp: float):
    boids[:, 0:2] %= np.array([asp, 1.])


# избегание стен - чем ближе к стене, тем больше значение ускорения от стены
@njit
def walls(boids: np.ndarray, asp: float):
    c = 1
    x = boids[:, 0]
    y = boids[:, 1]

    a_left = 1 / (np.abs(x) + c) ** 2
    a_right = -1 / (np.abs(x - asp) + c) ** 2

    a_bottom = 1 / (np.abs(y) + c) ** 2
    a_top = -1 / (np.abs(y - 1.) + c) ** 2

    return np.column_stack((a_left + a_right, a_bottom + a_top))


"""
сплоченность; нужно держаться вместе с группой
"""


@njit
def cohesion(coeffs: np.ndarray,
             boids: np.ndarray,
             idx: int,
             neigh_mask: np.ndarray,
             perception: float) -> np.ndarray:
    #добавляем взаимодействие между подходящими соседями
    #умножаем соответствущие коэффициенты относительно заданных классов
    temp = boids[neigh_mask, :].copy()
    tek = 0
    if boids[idx, -1] == 0:
        tek = coeffs[0, 0]
        temp[temp[:, -1] == 0, :2] *= coeffs[0, 0]
        temp[temp[:, -1] == 1, :2] *= coeffs[2, 0]
    else:
        tek = coeffs[1, 0]
        temp[temp[:, -1] == 0, :2] *= coeffs[2, 0]
        temp[temp[:, -1] == 1, :2] *= coeffs[1, 0]
    center = mean_axis0(temp[:, :2])
    a = (center - tek * boids[idx, :2]) / perception
    return a


"""
коэффициент разделения межу соседями
"""


@njit
def separation(coeffs: np.ndarray,
               boids: np.ndarray,
               idx: int,
               neigh_mask: np.ndarray) -> np.ndarray:
    #добавляем взаимодействие между подходящими соседями
    #умножаем соответствущие коэффициенты относительно заданных классов
    temp = boids[neigh_mask, :].copy()
    tek = 0
    if boids[idx, -1] == 0:
        tek = coeffs[0, 2]
        temp[temp[:, -1] == 0, :2] *= coeffs[0, 2]
        temp[temp[:, -1] == 1, :2] *= coeffs[2, 2]
    else:
        tek = coeffs[1, 2]
        temp[temp[:, -1] == 0, :2] *= coeffs[2, 2]
        temp[temp[:, -1] == 1, :2] *= coeffs[1, 2]
    d = mean_axis0(temp[:, :2] - tek * boids[idx, :2])
    return -d / ((d[0] ** 2 + d[1] ** 2) + 1)


"""
задает одну траекторию между соседями
"""


@njit
def alignment(coeffs: np.ndarray,
              boids: np.ndarray,
              idx: int,
              neigh_mask: np.ndarray,
              vrange: tuple) -> np.ndarray:
    #добавляем взаимодействие между подходящими соседями
    #умножаем соответствущие коэффициенты относительно заданных классов
    temp = boids[neigh_mask, :].copy()
    tek = 0
    if boids[idx, -1] == 0:
        tek = coeffs[0, 1]
        temp[temp[:, -1] == 0, 2:4] *= coeffs[0, 1]
        temp[temp[:, -1] == 1, 2:4] *= coeffs[2, 1]
    else:
        tek = coeffs[1, 1]
        temp[temp[:, -1] == 0, 2:4] *= coeffs[2, 1]
        temp[temp[:, -1] == 1, 2:4] *= coeffs[1, 1]
    v_mean = mean_axis0(coeffs[0, 1] * boids[neigh_mask, 2:4])
    a = (v_mean - tek * boids[idx, 2:4]) / (2 * vrange[1])
    return a


"""
матрица расстояний
"""


def distance(boids: np.ndarray) -> np.ndarray:  # считаем расстояние каждого боида с каждым
    return cdist(boids[:, :2], boids[:, :2])


"""
функция обновления ускорений или по сути обновления движения 
за счет cohesion, agliment, separate, walls, noises
"""
@njit(parallel=True)
def flocking(boids: np.ndarray,
             D: np.ndarray,
             perception: float,
             coeffs: np.ndarray,
             asp: float,
             vrange: tuple,
             noises: np.ndarray):  # считаем все ускорения на каждой итерации
    N = boids.shape[0]
    for i in range(N):
        D[i, i] = perception + 1
    mask = D < perception  # маска - смотрим соседей, где расстаояние между боидами меньше perception
    wal = walls(boids, asp)
    for i in prange(N):
        if not np.any(mask[i]):
            coh = np.zeros(2)
            alg = np.zeros(2)
            sep = np.zeros(2)
        else:
            coh = cohesion(coeffs, boids, i, mask[i], perception)
            alg = alignment(coeffs, boids, i, mask[i], vrange)
            sep = separation(coeffs, boids, i, mask[i])
        a = coh + alg + \
            sep + np.where(boids[i, -1] == 0, coeffs[0, 3], coeffs[1, 3]) * wal[i] + np.where(boids[i, -1] == 0,
                                                                                              coeffs[0, 4],
                                                                                              coeffs[1, 4]) * noises[i]
        boids[i, 4:6] = a
