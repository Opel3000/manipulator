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
    # cv2.imshow('img', img)
    # cv2.waitKey()
    # cv2.destroyAllWindows()
    pass


def segimg(img):
    pixel = 4

    global red_min, red_max
    red_min = np.array((0, 0, 210), np.uint8)
    red_max = np.array((49, 29, 255), np.uint8)
    cv2.rectangle(img, (0, 0), (pixel, pixel), (30, 28, 244), -1)

    global blm_min, blm_max
    blm_min = np.array((67, 67, 67), np.uint8)
    blm_max = np.array((69, 69, 69), np.uint8)
    cv2.rectangle(img, (pixel, 0), (pixel*2, pixel), (68, 68, 68), -1)

    global gre_min, gre_max
    gre_min = np.array((0, 210, 15), np.uint8)
    gre_max = np.array((1, 255, 20), np.uint8)
    cv2.rectangle(img, (pixel*2, 0), (pixel*3, pixel), (1, 245, 17), -1)

    global yel_min, yel_max
    yel_min = np.array((0, 233, 253), np.uint8)
    yel_max = np.array((73, 239, 255), np.uint8)
    cv2.rectangle(img, (pixel*3, 0), (pixel*4, pixel), (72, 238, 254), -1)

    global blu_min, blu_max
    blu_min = np.array((254, 76, 0), np.uint8)
    blu_max = np.array((255, 113, 57), np.uint8)
    cv2.rectangle(img, (pixel*4, 0), (pixel*5, pixel), (255, 112, 56), -1)

    global hsv_blu_min, hsv_blu_max
    hsv_blu_min = np.array((90, 50, 70), np.uint8)
    hsv_blu_max = np.array((120, 255, 255), np.uint8)

    global hsv_blm_min, hsv_blm_max
    hsv_blm_min = np.array((0, 0, 60), np.uint8)
    hsv_blm_max = np.array((70, 70, 70), np.uint8)


def obrezka(F, hsv_min, hsv_max):
    F[1][1] = 255

    F = cv2.inRange(F, hsv_min, hsv_max)
    # mask = cv2.GaussianBlur(mask, (5, 5), 0)
    # cv2.imshow('Final', F)
    # cv2.waitKey()
    # cv2.destroyAllWindows()

    x, y, w, h = 0, 0, 0, 0
    contours = cv2.findContours(F, cv2.RETR_TREE, cv2.CHAIN_APPROX_NONE)
    contours = contours[0]
    if contours:
        contours = sorted(contours, key=cv2.contourArea, reverse=True)
        (x, y, w, h) = cv2.boundingRect(contours[0])

        if h > 10 and w > 10:
            flagError = True
            # print('Норм')
        else:
            flagError = False
            # print('Выход')

        F = F[y:y + h, x:x + w]
        S = np.sum(F[1:len(F), 1:len(F[0])]) // 255

        mid_x = (x+(w//2))
        mid_y = (y+(h//2))

    return F, mid_x, mid_y, flagError, h, x, y, w


def obrezkaMat(F, hsv_min, hsv_max):
    F[1][1] = 255

    F = cv2.inRange(F, hsv_min, hsv_max)
    # mask = cv2.GaussianBlur(mask, (5, 5), 0)
    # cv2.imshow('Final', F)
    # cv2.waitKey()
    # cv2.destroyAllWindows()

    x, y, w, h = 0, 0, 0, 0
    contours = cv2.findContours(F, cv2.RETR_TREE, cv2.CHAIN_APPROX_NONE)
    contours = contours[0]
    if contours:
        contours = sorted(contours, key=cv2.contourArea, reverse=True)
        (x, y, w, h) = cv2.boundingRect(contours[0])

        mid_x = (x+(w//2))
        mid_y = (y+(h//2))

        if (mid_x > pixel_x // 2 - 2 * w) and (mid_x < pixel_x // 2 + 2 * w):
            flagError = True
            # print('Норм')
        else:
            flagError = False
            # print('Выход')

        F = F[y:y + h, x:x + w]
        S = np.sum(F[1:len(F), 1:len(F[0])]) // 255

    return F, mid_x, mid_y, flagError, h, x, y, w


def materials_matrix(img):
    segimg(img)

    massmat, count, massmat_fin = [], 0, []

    while True:  # по красным фигурам
        # break
        img1, mid_x, mid_y, ids, h, x, y, w = obrezkaMat(img, red_min, red_max)
        if ids:
            cv2.rectangle(img, (x+w, y+h), (x, y), (0, 0, 0), -1)
            massmat.append([mid_x, mid_y, 1, h, w, count])
        else:
            break

    while True:  # по зелёным фигурам
        # break
        img1, mid_x, mid_y, ids, h, x, y, w = obrezkaMat(img, gre_min, gre_max)
        if ids:
            cv2.rectangle(img, (x+w, y+h), (x, y), (0, 0, 0), -1)
            massmat.append([mid_x, mid_y, 2, h, w, count])
        else:
            break

    while True:  # по жёлтым фигурам
        # break
        img1, mid_x, mid_y, ids, h, x, y, w = obrezkaMat(img, yel_min, yel_max)
        if ids:
            cv2.rectangle(img, (x+w, y+h), (x, y), (0, 0, 0), -1)
            massmat.append([mid_x, mid_y, 3, h, w, count])
        else:
            break

    while True:  # по синим фигурам
        # break
        img1, mid_x, mid_y, ids, h, x, y, w = obrezkaMat(img, blu_min, blu_max)
        if ids:
            cv2.rectangle(img, (x+w, y+h), (x, y), (0, 0, 0), -1)
            massmat.append([mid_x, mid_y, 4, h, w, count])
        else:
            break

    while True:  # по черным фигурам
        # break
        img1, mid_x, mid_y, ids, h, x, y, w = obrezkaMat(img, blm_min, blm_max)
        if ids:
            cv2.rectangle(img, (x+w, y+h), (x, y), (0, 0, 0), -1)
            massmat.append([mid_x, mid_y, 5, h, w, count])
        else:
            break

    if len(massmat) == 1:
        # ser.write(str(massmat[0][2]).encode('utf-8'))
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


def build_matrix(mass_one):
    mass_one_fin = []
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
        mass_one_fin = [0]
        min_x = min(mass_one[0][0], mass_one[1][0])
        max_x = max(mass_one[0][0], mass_one[1][0])

        if min_x < pixel_x // 2 - mass_one[0][4]:
            for mas in mass_one:
                if min_x == mas[0]:
                    mass_one_fin.insert(0, mas[2])
        else:
            for mas in mass_one:
                if min_x == mas[0]:
                    mass_one_fin.insert(1, mas[2])

        if max_x > pixel_x // 2 + mass_one[0][4]:
            for mas in mass_one:
                if max_x == mas[0]:
                    mass_one_fin.insert(2, mas[2])
        else:
            for mas in mass_one:
                if max_x == mas[0]:
                    mass_one_fin.insert(1, mas[2])

    elif len(mass_one) == 1:
        if mass_one[0][0] < pixel_x // 2 - mass_one[0][4]:
            mass_one_fin = [mass_one[0][2], 0, 0]
        elif mass_one[0][0] > pixel_x // 2 + mass_one[0][4]:
            mass_one_fin = [0, 0, mass_one[0][2]]
        else:
            mass_one_fin = [0, mass_one[0][2], 0]

    elif len(mass_one) == 0:
        mass_one_fin = [0, 0, 0]

    building_map.append(mass_one_fin)


def matrix_final():

    cap = cv2.VideoCapture(3)
    ret, frame = cap.read()

    while True:
        cv2.imshow('img1', frame)
        cv2.imwrite('b1.png', frame)
        cv2.waitKey(0)
        cv2.destroyAllWindows()
        break

    cap.release()

    img = cv2.imread('b1.png')

    global imghsv
    count, massmap = -1, []

    segimg(img)

    watchimg(img)

    while True:  # по красным фигурам
        # break
        img1, mid_x, mid_y, ids, h, x, y, w = obrezka(img, red_min, red_max)
        if ids:
            count += 1
            cv2.rectangle(img, (x+w, y+h), (x, y), (0, 0, 0), -1)
            massmap.append([mid_x, mid_y, 1, h, w, count])
        else:
            break

    while True:  # по зелёным фигурам
        # break
        img1, mid_x, mid_y, ids, h, x, y, w = obrezka(img, gre_min, gre_max)
        if ids:
            count += 1
            cv2.rectangle(img, (x+w, y+h), (x, y), (0, 0, 0), -1)
            massmap.append([mid_x, mid_y, 2, h, w, count])
        else:
            break

    while True:  # по жёлтым фигурам
        # break
        img1, mid_x, mid_y, ids, h, x, y, w = obrezka(img, yel_min, yel_max)
        if ids:
            count += 1
            cv2.rectangle(img, (x+w, y+h), (x, y), (0, 0, 0), -1)
            massmap.append([mid_x, mid_y, 3, h, w, count])
        else:
            break

    watchimg(img)
    imghsv = img.copy()

    while True:  # по синим фигурам
        # break
        img1, mid_x, mid_y, ids, h, x, y, w = obrezka(img, blu_min, blu_max)
        if ids:
            count += 1
            cv2.rectangle(img, (x+w, y+h), (x, y), (0, 0, 0), -1)
            massmap.append([mid_x, mid_y, 4, h, w, count])
        else:
            break

    while True:  # по черным фигурам
        # break
        img1, mid_x, mid_y, ids, h, x, y, w = obrezka(img, blm_min, blm_max)
        if ids:
            count += 1
            cv2.rectangle(img, (x+w, y+h), (x, y), (0, 0, 0), -1)
            massmap.append([mid_x, mid_y, 5, h, w, count])
        else:
            break

    min_y = 999999

    for mas in massmap:
        if mas[1] < min_y:
            min_y = mas[1]

    mass_one = []

    for mas in massmap:
        if not(mas[1] > min_y+20):
            mass_one.append(mas)

    build_matrix(mass_one)

    mass_two = []

    for mas in massmap:
        if (mas[1] > min_y + 0.5 * massmap[0][3]) and (mas[1] < min_y + 1.5 * massmap[0][3]):
            mass_two.append(mas)

    build_matrix(mass_two)

    mass_three = []

    for mas in massmap:
        if not(mas[1] < min_y + 1.5 * massmap[0][3]):
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


countVar = -1

while True:
    if ser.in_waiting > 0:
        if countVar == -1:
            ser.flush()
            matrix_final()
            building_map_fin = ""

            for mas in building_map:
                for i in mas:
                    building_map_fin += str(i)

            print(building_map_fin)

            ser.write(building_map.encode('utf-8'))  # building_map
            countVar+=1

        else:
            cap = cv2.VideoCapture(3)
            ret, frame = cap.read()
            while True:
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
            ser.flush()



print("--- %s seconds ---" % (time.time() - start_time))
