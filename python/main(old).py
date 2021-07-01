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

# ser = serial.Serial('/dev/ttyACM0', 9600)
ser = serial.Serial('/dev/ttyACM1', 9600)

building_map = []

red_min, red_max, blm_min, blm_max, gre_min, gre_max = 0, 0, 0, 0, 0, 0
yel_min, yel_max, blu_min, blu_max =  0, 0, 0, 0

flagBGR = False

pixel_x = 640

cameraPort = 0


def watchimg(img):
   #  cv2.imshow('img', img)
   #  cv2.waitKey()
   #  cv2.destroyAllWindows()
    pass


def segimg(img):
    pixel = 4
    if flagBGR:
        
        cv2.rectangle(img, (0, 0), (pixel, pixel), (25, 30, 255), -1)
        cv2.rectangle(img, (pixel, 0), (pixel*2, pixel), (0, 0, 0), -1)
        cv2.rectangle(img, (pixel*2, 0), (pixel*3, pixel), (70, 255, 27), -1)
        cv2.rectangle(img, (pixel*3, 0), (pixel*4, pixel), (60, 255, 204), -1)
        cv2.rectangle(img, (pixel*4, 0), (pixel*5, pixel), (255, 28, 31), -1)
        
    else:
        
        cv2.rectangle(img, (0, 0), (pixel, pixel), (20, 20, 255), -1)
        cv2.rectangle(img, (pixel, 0), (pixel*2, pixel), (0, 0, 0), -1)
        cv2.rectangle(img, (pixel*2, 0), (pixel*3, pixel), (23, 255, 27), -1)
        cv2.rectangle(img, (pixel*3, 0), (pixel*4, pixel), (60, 255, 204), -1)
        cv2.rectangle(img, (pixel*4, 0), (pixel*5, pixel), (255, 28, 31), -1)
    
    
def obrezka(F, hsv_min, hsv_max):
    if not(flagBGR):
        F = cv2.cvtColor(F, cv2.COLOR_BGR2HSV)

    F = cv2.inRange(F, hsv_min, hsv_max)

    watchimg(F)

    x, y, w, h = 0, 0, 0, 0
    contours = cv2.findContours(F, cv2.RETR_TREE, cv2.CHAIN_APPROX_NONE)
    contours = contours[0]
    if contours:
        contours = sorted(contours, key=cv2.contourArea, reverse=True)
        (x, y, w, h) = cv2.boundingRect(contours[0])
        F = F[y:y + h, x:x + w]
        S = np.sum(F[1:len(F), 1:len(F[0])]) // 255

        if h > 20 and w > 20 and S > 700:
            flagError = True
        else:
            flagError = False
            
        midle_x = (x+(w//2))
        midle_y = (y+(h//2))

    return midle_x, midle_y, flagError, h, x, y, w


def obrezkaMat(F, hsv_min, hsv_max):
    if not(flagBGR):
        F = cv2.cvtColor(F, cv2.COLOR_BGR2HSV)
        
    F = cv2.inRange(F, hsv_min, hsv_max)
    
    watchimg(F)
    
    x, y, w, h = 0, 0, 0, 0
    contours = cv2.findContours(F, cv2.RETR_TREE, cv2.CHAIN_APPROX_NONE)
    contours = contours[0]
    if contours:
        contours = sorted(contours, key=cv2.contourArea, reverse=True)
        (x, y, w, h) = cv2.boundingRect(contours[0])
        F = F[y:y + h, x:x + w]
        S = np.sum(F[1:len(F), 1:len(F[0])]) // 255

        if h > 75 and (w > 75):
            flagError = True
        else:
            flagError = False

        mid_x = (x+(w//2))
        mid_y = (y+(h//2))
        

    return mid_x, mid_y, flagError, h, x, y, w


def materials_matrix(img):

    massmat, count, massmat_fin = [], 0, []
    segimg(img)
    
    while True:  # по черным фигурам
        # break
        mid_x, mid_y, ids, h, x, y, w = obrezkaMat(img, blm_min, blm_max)
        if ids:
            midle_x = (x+(w//2))
            midle_y = (y+(h//2))
            cv2.rectangle(img, (x+w, y+h), (x, y-25), (255, 255, 255), -1)
            massmat.append([mid_x, mid_y, 5, h, w, count])
        else:
            break
    
    while True:  # по синим фигурам
        # break
        mid_x, mid_y, ids, h, x, y, w = obrezkaMat(img, blu_min, blu_max)
        if ids:
            midle_x = (x+(w//2))
            midle_y = (y+(h//2))
            cv2.rectangle(img, (x+w, y+h), (x, y-25), (255, 255, 255), -1)
            massmat.append([mid_x, mid_y, 4, h, w, count])
        else:
            break
        
    while True:  # по зелёным фигурам
        # break
        mid_x, mid_y, ids, h, x, y, w = obrezkaMat(img, gre_min, gre_max)
        if ids:
            midle_x = (x+(w//2))
            midle_y = (y+(h//2))
            cv2.rectangle(img, (x+w, y+h), (x, y-25), (255, 255, 255), -1)
            massmat.append([mid_x, mid_y, 2, h, w, count])
        else:
            break
    while True:  # по жёлтым фигурам
        # break
        mid_x, mid_y, ids, h, x, y, w = obrezkaMat(img, yel_min, yel_max)
        if ids:
            midle_x = (x+(w//2))
            midle_y = (y+(h//2))
            cv2.rectangle(img, (x+w, y+h), (x, y-25), (255, 255, 255), -1)
            massmat.append([mid_x, mid_y, 3, h, w, count])
        else:
            break
    
    while True:  # по красным фигурам
        # break
        mid_x, mid_y, ids, h, x, y, w = obrezkaMat(img, red_min, red_max)
        if ids:
            midle_x = (x+(w//2))
            midle_y = (y+(h//2))
            cv2.rectangle(img, (x+w, y+h), (x, y-25), (255, 255, 255), -1)
            massmat.append([mid_x, mid_y, 1, h, w, count])
        else:
            break

    print(massmat)

    if len(massmat) == 1:
        ser.write((str(massmat[0][2])+str(massmat[0][2])).encode('utf-8'))
        print((str(massmat[0][2])+str(massmat[0][2])).encode('utf-8'))
    elif len(massmat) == 2:

        massmat.sort()

        for i in massmat:
            massmat_fin.append(i[2])

        print((str(massmat_fin[1])+str(massmat_fin[0])).encode('utf-8'))
        ser.write((str(massmat_fin[1])+str(massmat_fin[0])).encode('utf-8'))

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

    cap = cv2.VideoCapture(cameraPort)
    xx = 0
    while True:
        ret, frame = cap.read()
        cv2.imshow('img1', frame)
        cv2.waitKey(100)
        xx += 1
        if xx > 20:
            break
    while True:
        ret, frame = cap.read()
        if frame[0][0][0] != 0:
            cv2.imwrite('cfin.png', frame)
            #cv2.waitKey(0)
            cv2.destroyAllWindows()
            break

    cap.release()

    img = cv2.imread('cfin.png')

    M = cv2.getRotationMatrix2D((640 // 2, 480 // 2), 82, 1.0)
    img = cv2.warpAffine(img, M, (640, 480))

    watchimg(img)

    global imghsv
    count, massmap = -1, []

    segimg(img)

    
    while True:  # по синим фигурам
        # break
        mid_x, mid_y, ids, h, x, y, w = obrezka(img, blu_min, blu_max)
        if ids:
             count += 1
             cv2.rectangle(img, (x+w, y+h), (x, y), (255, 255, 255), -1)
             massmap.append([mid_x, mid_y, 4, h, w, count])
             print(4)
        else:
            break

    while True:  # по зелёным фигурам
        # break
        mid_x, mid_y, ids, h, x, y, w = obrezka(img, gre_min, gre_max)
        if ids:
            count += 1
            cv2.rectangle(img, (x+w, y+h), (x, y), (255, 255, 255), -1)
            massmap.append([mid_x, mid_y, 2, h, w, count])
            print(2)
        else:
            break

    while True:  # по жёлтым фигурам
        # break
        mid_x, mid_y, ids, h, x, y, w = obrezka(img, yel_min, yel_max)
        if ids:
            count += 1
            cv2.rectangle(img, (x+w, y+h), (x, y), (255, 255, 255), -1)
            massmap.append([mid_x, mid_y, 3, h, w, count])
            print(3)
        else:
            break

    imghsv = img.copy()
    
    while True:  # по черным фигурам
        # break
        mid_x, mid_y, ids, h, x, y, w = obrezka(img, blm_min, blm_max)
        if ids:
            if h < 300 and w < 300:
                count += 1
                massmap.append([mid_x, mid_y, 5, h, w, count])
                print(5)
            cv2.rectangle(img, (x+w, y+h), (x, y), (255, 255, 255), -1)
        else:
            break
    cv2.rectangle(img, (0, 0), (5, 5), (20, 20, 255), -1)
    while True:  # по красным фигурам
        # break
        mid_x, mid_y, ids, h, x, y, w = obrezka(img, red_min, red_max)
        if ids:
            count += 1
            cv2.rectangle(img, (x+w, y+h), (x, y), (255, 255, 255), -1)
            massmap.append([mid_x, mid_y, 1, h, w, count])
            print(1)
        else:
            break
    print(massmap)

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
        if (mas[1] > min_y + 0.5 * massmap[0][3]) and (mas[1] < min_y + massmap[0][3] * 1.7):
            mass_two.append(mas)

    build_matrix(mass_two)

    mass_three = []

    for mas in massmap:
        if not(mas[1] < min_y + 1.7 * massmap[0][3]):
            mass_three.append(mas)

    build_matrix(mass_three)


countVar = 0

if flagBGR:
    red_min = np.array((0, 0, 40), np.uint8)  # 142, 100, 46
    red_max = np.array((25, 30, 255), np.uint8) #196, 255, 150
            
    blm_min = np.array((0, 0, 0), np.uint8) #0, 0, 0
    blm_max = np.array((35, 30, 30), np.uint8) #255, 255, 12
    
    gre_min = np.array((60, 194, 15), np.uint8)     #38, 121, 0
    gre_max = np.array((87, 255, 162), np.uint8)  #104, 255, 255
    
    yel_min = np.array((0, 125, 152), np.uint8)  # 11, 116, 125
    yel_max = np.array((54, 255, 217), np.uint8)  # 55, 200, 255
    
    blu_min = np.array((27, 0, 0), np.uint8)  # 111, 176, 41
    blu_max = np.array((255, 28, 31), np.uint8)  #131, 255, 255
    
else:
    red_min = np.array((0, 176, 104), np.uint8)  # 142, 100, 46
    red_max = np.array((179, 255, 255), np.uint8) #196, 255, 150
    
   #  redd_min = np.array((0, 196, 37), np.uint8)  # 142, 100, 46
   #  redd_max = np.array((49, 255, 255), np.uint8) #196, 255, 150
    
    blm_min = np.array((0, 0, 0), np.uint8) #0, 0, 0
    blm_max = np.array((179, 255, 27), np.uint8) #255, 255, 12
         
    gre_min = np.array((38, 121, 15), np.uint8)     #38, 121, 0
    gre_max = np.array((104, 255, 255), np.uint8)  #104, 255, 255
    
    yel_min = np.array((11, 116, 100), np.uint8)  # 11, 116, 125
    yel_max = np.array((55,255, 255), np.uint8)  # 55, 200, 255
    
    blu_min = np.array((100, 176, 16), np.uint8)  # 111, 176, 41
    blu_max = np.array((131, 255, 255), np.uint8)  #131, 255, 255
    
    

while True:
    # ser.write(countVar.encode('utf-8'))  # building_map
    break

while True:
    if ser.in_waiting > 0:
        print(ser.read())
        if countVar == 0:
            matrix_final()
            building_map_fin = ""

            for mas in building_map:
                for i in mas:
                    building_map_fin += str(i)

            print(building_map_fin)

            ser.write(building_map_fin.encode('utf-8'))  # building_map
            countVar += 1

        elif (countVar > 0) and countVar < 6:
            cap = cv2.VideoCapture(cameraPort)
            xx = 0
            while True:
                ret, frame = cap.read()
                cv2.imshow('img1', frame)
                cv2.waitKey(100)
                xx += 1
                if xx > 30:
                    break
            while True:
                ret, frame = cap.read()
                if frame[0][0][0] != 0:
                    #cv2.imshow('img1', frame)
                    cv2.imwrite('c'+str(countVar)+'.png', frame)
                    #cv2.waitKey(0)
                    cv2.destroyAllWindows()
                    break
#
            img = cv2.imread('c'+str(countVar)+'.png')
            materials_matrix(img)
            # hsv_black_blue(imghsv)
            countVar += 1
            if (countVar == 6):
                countVar = 0
                time.sleep(10)
                building_map=[]
            watchimg(img)
            cap.release()




print("--- %s seconds ---" % (time.time() - start_time))

