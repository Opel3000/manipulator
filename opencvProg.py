import numpy as np
import serial
import cv2
import time
start_time = time.time()


# red - 1
# green - 2
# yellow - 3
# blue - 4
# black - 5


ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)

building_map = []

pixel_x = 640


def watchimg(img):
    cv2.imshow('img', img)
    cv2.waitKey()
    cv2.destroyAllWindows()
    pass


def segimg(img):
    pixel = 4

    global red_min, red_max
    red_min = np.array((0, 10, 140), np.uint8)
    red_max = np.array((35, 45, 255), np.uint8)
    cv2.rectangle(img, (0, 0), (pixel, pixel), (35, 45, 255), -1)

    global blm_min, blm_max
    blm_min = np.array((0, 0, 0), np.uint8)
    blm_max = np.array((15, 20, 20), np.uint8)
    cv2.rectangle(img, (pixel, 0), (pixel*2, pixel), (15, 20, 20), -1)

    global gre_min, gre_max
    gre_min = np.array((20, 65, 0), np.uint8)
    gre_max = np.array((80, 255, 20), np.uint8)
    cv2.rectangle(img, (pixel*2, 0), (pixel*3, pixel), (80, 255, 20), -1)

    global yel_min, yel_max
    yel_min = np.array((30, 155, 195), np.uint8)
    yel_max = np.array((90, 210, 220), np.uint8)
    cv2.rectangle(img, (pixel*3, 0), (pixel*4, pixel), (90, 210, 220), -1)

    global blu_min, blu_max
    blu_min = np.array((60, 0, 0), np.uint8)
    blu_max = np.array((255, 40, 20), np.uint8)
    cv2.rectangle(img, (pixel*4, 0), (pixel*5, pixel), (255, 40, 20), -1)

    global hsv_blu_min, hsv_blu_max
    hsv_blu_min = np.array((90, 50, 70), np.uint8)
    hsv_blu_max = np.array((120, 255, 255), np.uint8)

    global hsv_blm_min, hsv_blm_max
    hsv_blm_min = np.array((0, 0, 60), np.uint8)
    hsv_blm_max = np.array((70, 70, 70), np.uint8)


def obrezka(F, hsv_min, hsv_max):
    F[1][1] = 255

    F = cv2.inRange(F, hsv_min, hsv_max)

    x, y, w, h = 0, 0, 0, 0
    contours = cv2.findContours(F, cv2.RETR_TREE, cv2.CHAIN_APPROX_NONE)
    contours = contours[0]
    if contours:
        contours = sorted(contours, key=cv2.contourArea, reverse=True)
        (x, y, w, h) = cv2.boundingRect(contours[0])

        if h > 10 and w > 10 and h < 110 and w < 110:
            flagError = True
        else:
            flagError = False

        F = F[y:y + h, x:x + w]
        S = np.sum(F[1:len(F), 1:len(F[0])]) // 255

        midle_x = (x+(w//2))
        midle_y = (y+(h//2))

    return midle_x, midle_y, flagError, h, x, y, w


def obrezkaMat(F, hsv_min, hsv_max):

    F = cv2.inRange(F, hsv_min, hsv_max)
    x, y, w, h = 0, 0, 0, 0
    contours = cv2.findContours(F, cv2.RETR_TREE, cv2.CHAIN_APPROX_NONE)
    contours = contours[0]
    if contours:
        contours = sorted(contours, key=cv2.contourArea, reverse=True)
        (x, y, w, h) = cv2.boundingRect(contours[0])

        mid_x = (x+(w//2))
        mid_y = (y+(h//2))

        if h > 15 and (w > 15):
            flagError = True
            # print('Норм')
        else:
            flagError = False
            # print('Выход')

        F = F[y:y + h, x:x + w]
        S = np.sum(F[1:len(F), 1:len(F[0])]) // 255

    return mid_x, mid_y, flagError, h, x, y, w


def materials_matrix(img):

    massmat, count, massmat_fin = [], 0, []
    segimg(img)
    while True:  # по красным фигурам
        # break
        mid_x, mid_y, ids, h, x, y, w = obrezkaMat(img, red_min, red_max)
        if ids:
            cv2.rectangle(img, (x+w, y+h), (x, y), (255, 255, 255), -1)
            massmat.append([mid_x, mid_y, 1, h, w, count])
        else:
            break
    while True:  # по зелёным фигурам
        # break
        mid_x, mid_y, ids, h, x, y, w = obrezkaMat(img, gre_min, gre_max)
        if ids:
            cv2.rectangle(img, (x+w, y+h), (x, y), (255, 255, 255), -1)
            massmat.append([mid_x, mid_y, 2, h, w, count])
        else:
            break
    while True:  # по жёлтым фигурам
        # break
        mid_x, mid_y, ids, h, x, y, w = obrezkaMat(img, yel_min, yel_max)
        if ids:
            cv2.rectangle(img, (x+w, y+h), (x, y), (255, 255, 255), -1)
            massmat.append([mid_x, mid_y, 3, h, w, count])
        else:
            break
    while True:  # по синим фигурам
        # break
        mid_x, mid_y, ids, h, x, y, w = obrezkaMat(img, blu_min, blu_max)
        if ids:
            cv2.rectangle(img, (x+w, y+h), (x, y), (255, 255, 255), -1)
            massmat.append([mid_x, mid_y, 4, h, w, count])
        else:
            break
    while True:  # по черным фигурам
        # break
        mid_x, mid_y, ids, h, x, y, w = obrezkaMat(img, blm_min, blm_max)
        if ids:
            cv2.rectangle(img, (x+w, y+h), (x, y), (255, 255, 255), -1)
            massmat.append([mid_x, mid_y, 5, h, w, count])
        else:
            break

    print(massmat)

    if len(massmat) == 1:
        ser.write(str(massmat[0][2]).encode('utf-8'))
        print(str(massmat[0][2]).encode('utf-8'))
    elif len(massmat) == 2:

        min_w = 999

        for mas in massmat:
            if min_w > mas[4]:
                min_w = mas[4]
                id_min = mas[2]

        massmat_fin.append(id_min)

        for mas in massmat:
            if mas[2] != id_min:
                massmat_fin.append(mas[2])

        print((str(massmat_fin[0])+str(massmat_fin[1])).encode('utf-8'))
        ser.write((str(massmat_fin[0])+str(massmat_fin[1])).encode('utf-8'))

    else:
        massmat.sort()
        block = ""
        for i in massmat:
            if not (str(i[2]) in block):
                block+=str(i[2])
                massmat_fin.append(i[2])

        print((str(massmat_fin[1]) + str(massmat_fin[0])).encode('utf-8'))
        ser.write((str(massmat_fin[1])+str(massmat_fin[0])).encode('utf-8'))


def build_matrix(mass_one):
    mass_one_fin = []
    min_x, max_x = 999, -1
    if len(mass_one) == 3:
        for mas in mass_one:
            if mas[0] < min_x:
                min_x = mas[0]
                index_min_x = mass_one.index(mas)

        mass_one_fin.append(mass_one[index_min_x][2])

        for mas in mass_one:
            if mas[0] > max_x:
                max_x = mas[0]
                index_max_x = mass_one.index(mas)
                maxmas = mas

        for mas in mass_one:
            if mass_one.index(mas) != index_min_x and mass_one.index(mas) != index_max_x:
                mass_one_fin.append(mas[2])

        mass_one_fin.append(maxmas[2])

    elif len(mass_one) == 2:
        mass_one_fin = [0, 0, 0]
        min_x = min(mass_one[0][0], mass_one[1][0])
        max_x = max(mass_one[0][0], mass_one[1][0])

        if min_x < pixel_x // 2 - (mass_one[0][4]*0.4):
            for mas in mass_one:
                if min_x == mas[0]:
                    mass_one_fin[0] = mas[2]
        else:
            for mas in mass_one:
                if min_x == mas[0]:
                    mass_one_fin[1] = mas[2]

        if max_x > pixel_x // 2 + (mass_one[0][4]*0.4):
            for mas in mass_one:
                if max_x == mas[0]:
                    mass_one_fin[2] = mas[2]
        else:
            for mas in mass_one:
                if max_x == mas[0]:
                    mass_one_fin[1] = mas[2]

    elif len(mass_one) == 1:
        if mass_one[0][0] < pixel_x // 2 - (mass_one[0][4]*0.4):
            mass_one_fin = [mass_one[0][2], 0, 0]
        elif mass_one[0][0] > pixel_x // 2 + (mass_one[0][4]*0.4):
            mass_one_fin = [0, 0, mass_one[0][2]]
        else:
            mass_one_fin = [0, mass_one[0][2], 0]

    elif len(mass_one) == 0:
        mass_one_fin = [0, 0, 0]

    building_map.append(mass_one_fin)


def matrix_final():

    cap = cv2.VideoCapture(3)

    while True:
        ret, frame = cap.read()
        cv2.imshow('img1', frame)
        cv2.imwrite('b1.png', frame)
        cv2.waitKey(0)
        cv2.destroyAllWindows()
        break

    cap.release()

    img = cv2.imread('cfin.png')

    M = cv2.getRotationMatrix2D((640 // 2, 480 // 2), 80, 1.0)
    img = cv2.warpAffine(img, M, (640, 480))

    global imghsv
    count, massmap = -1, []

    segimg(img)

    while True:  # по красным фигурам
        # break
        mid_x, mid_y, ids, h, x, y, w = obrezka(img, red_min, red_max)
        if ids:
            count += 1
            cv2.rectangle(img, (x+w, y+h), (x, y), (255, 255, 255), -1)
            massmap.append([mid_x, mid_y, 1, h, w, count])
        else:
            break

    while True:  # по зелёным фигурам
        # break
        mid_x, mid_y, ids, h, x, y, w = obrezka(img, gre_min, gre_max)
        if ids:
            count += 1
            cv2.rectangle(img, (x+w, y+h), (x, y), (255, 255, 255), -1)
            massmap.append([mid_x, mid_y, 2, h, w, count])
        else:
            break

    while True:  # по жёлтым фигурам
        # break
        mid_x, mid_y, ids, h, x, y, w = obrezka(img, yel_min, yel_max)
        if ids:
            count += 1
            cv2.rectangle(img, (x+w, y+h), (x, y), (255, 255, 255), -1)
            massmap.append([mid_x, mid_y, 3, h, w, count])
        else:
            break

    imghsv = img.copy()

    while True:  # по синим фигурам
        # break
        mid_x, mid_y, ids, h, x, y, w = obrezka(img, blu_min, blu_max)
        if ids:
            count += 1
            cv2.rectangle(img, (x+w, y+h), (x, y), (255, 255, 255), -1)
            massmap.append([mid_x, mid_y, 4, h, w, count])
        else:
            break

    while True:  # по черным фигурам
        # break
        mid_x, mid_y, ids, h, x, y, w = obrezka(img, blm_min, blm_max)
        if ids:
            count += 1
            cv2.rectangle(img, (x+w, y+h), (x, y), (255, 255, 255), -1)
            massmap.append([mid_x, mid_y, 5, h, w, count])
        else:
            break

    min_y = 999999

    for mas in massmap:
        if mas[1] < min_y:
            min_y = mas[1]

    mass_one = []

    for mas in massmap:
        if not(mas[1] > min_y+30):
            mass_one.append(mas)

    build_matrix(mass_one)

    mass_two = []

    for mas in massmap:
        # print("min_y", min_y)
        # print("massmap[0][3]", massmap[0][3])
        # print("mas[1]", mas[1])
        # print("min_y + 0.5 * massmap[0][3]", min_y + 0.5 * massmap[0][3])
        # print("min_y + 1.7 * massmap[0][3]", min_y + 1.7 * massmap[0][3])
        # print("mas", mas)
        if (mas[1] > min_y + 0.5 * massmap[0][3]) and (mas[1] < min_y + massmap[0][3] * 1.7):
            mass_two.append(mas)

    build_matrix(mass_two)

    mass_three = []

    for mas in massmap:
        if not(mas[1] < min_y + 1.7 * massmap[0][3]):
            mass_three.append(mas)

    build_matrix(mass_three)


# def hsv_black_blue(imghsv):
#     hsv = cv2.cvtColor(imghsv, cv2.COLOR_BGR2HSV)
#
#     count, masshsv, pixel = 0, [], 4
#
#     cv2.rectangle(hsv, (0, 0), (pixel, pixel), (100, 100, 100), -1)
#     cv2.rectangle(hsv, (pixel, 0), (pixel*2, pixel), (68, 68, 68), -1)
#
#     watchimg(hsv)
#
#     while True:  # по синим фигурам
#         # break
#         img1, mid_x, mid_y, ids, h, x, y, w = obrezka(hsv, hsv_blu_min, hsv_blu_max)
#         if ids:
#             count += 1
#             cv2.rectangle(hsv, (x+w, y+h), (x, y), (0, 0, 0), -1)
#             masshsv.append([mid_x, mid_y, 4, h, w, count])
#         else:
#             break
#
#     while True:  # по черный фигурам
#         # break
#         img1, mid_x, mid_y, ids, h, x, y, w = obrezka(hsv, hsv_blm_min, hsv_blm_max)
#         if ids:
#             count += 1
#             cv2.rectangle(hsv, (x+w, y+h), (x, y), (0, 0, 0), -1)
#             masshsv.append([mid_x, mid_y, 5, h, w, count])
#         else:
#             break
#
#     print(masshsv)


countVar = 0

while True:
    if ser.in_waiting > 0:
        if countVar == 0:
            matrix_final()
            building_map_fin = ""

            for mas in building_map:
                for i in mas:
                    building_map_fin += str(i)

            print(building_map_fin)

            ser.write(building_map.encode('utf-8'))  # building_map
            countVar+=1

        elif countVar > 0 and countVar < 6:
            cap = cv2.VideoCapture(3)
            while True:
                ret, frame = cap.read()
                cv2.imshow('img1', frame)
                cv2.imwrite('c'+str(countVar)+'.png', frame)
                cv2.waitKey(0)
                cv2.destroyAllWindows()
                break

            img = cv2.imread('c'+str(countVar)+'.png')
            materials_matrix(img)
            # hsv_black_blue(imghsv)
            countVar += 1
            cap.release()
        else:
            exit(0)

print("--- %s seconds ---" % (time.time() - start_time))
