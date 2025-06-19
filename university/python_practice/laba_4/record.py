import cv2
"""
записывал я с помощью приложения для macOs TapRecorder
данный код нужен для ускорения или "склеивания" кадров для видео,
чтобы было более плавно и на итоговом видео было больше фпс
"""
cap = cv2.VideoCapture('/Users/fedorkomarov/Movies/TapRecord Video/REC-20240304205530.mp4')
output_file = '/Users/fedorkomarov/Movies/TapRecord Video/REC-5000.mp4'
output_fps = 60
fourcc = cv2.VideoWriter_fourcc(*'mp4v')
out = cv2.VideoWriter(output_file, fourcc, output_fps, (int (cap.get(3)), int(cap.get(4))))
while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        break
    out.write(frame)
cap.release()
out.release()
cv2.destroyAllWindows()