import numpy as np
import serial
import cv2


# red - 1
# green - 2
# yellow - 3
# blue - 4
# black - 5

#ser = serial.Serial('/dev/ttyACM1', 9600)

cameraPort = 0


def watchimg(fram):
    cv2.imshow('frame', fram)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
    pass


def segimg(F):

    pixel = 2
        
    cv2.rectangle(F, (0, 0), (pixel, pixel), (20, 20, 255), -1)
    cv2.rectangle(F, (pixel, 0), (pixel*2, pixel), (0, 0, 0), -1)
    cv2.rectangle(F, (pixel*2, 0), (pixel*3, pixel), (23, 255, 27), -1)
    cv2.rectangle(F, (pixel*3, 0), (pixel*4, pixel), (60, 255, 204), -1)
    cv2.rectangle(F, (pixel*4, 0), (pixel*5, pixel), (255, 28, 31), -1)

    return F
    
    
def obrezka(F, hsv_min, hsv_max):

    F = cv2.cvtColor(F, cv2.COLOR_BGR2HSV)

    F = cv2.inRange(F, hsv_min, hsv_max)

    w, h = 0, 0
    contours = cv2.findContours(F, cv2.RETR_TREE, cv2.CHAIN_APPROX_NONE)
    contours = contours[0]
    if contours:
        contours = sorted(contours, key=cv2.contourArea, reverse=True)
        (_, _, w, h) = cv2.boundingRect(contours[0])

        if h > 10 and w > 10:
            flagError = True
        else:
            flagError = False
    

    return flagError


def obrezkaMat(F, hsv_min, hsv_max):

    F = cv2.cvtColor(F, cv2.COLOR_BGR2HSV)
        
    F = cv2.inRange(F, hsv_min, hsv_max)
    
    w, h = 0, 0
    contours = cv2.findContours(F, cv2.RETR_TREE, cv2.CHAIN_APPROX_NONE)
    contours = contours[0]
    if contours:
        contours = sorted(contours, key=cv2.contourArea, reverse=True)
        (_, _, w, h) = cv2.boundingRect(contours[0])

        if h > 10 and (w > 10):
            flagError = True
        else:
            flagError = False
        

    return flagError


def materials_matrix():

    massmat = []

    imglow = img[228:248, 188:208]
    imglow = segimg(imglow)
    imghight = img[205:225, 335:355]
    imghight = segimg(imghight)
    checkMas = [imglow, imghight]
    
    for image in checkMas:
     # по черным фигурам
        ids = obrezkaMat(image, blm_min, blm_max)
        if ids:
            massmat.append(5)
            continue
    
    # по синим фигурам
        ids = obrezkaMat(image, blu_min, blu_max)
        if ids:
            massmat.append(4)
            continue

    # по зелёным фигурам
        ids = obrezkaMat(image, gre_min, gre_max)
        if ids:
            massmat.append(2)
            continue

    # по жёлтым фигурам
        ids = obrezkaMat(image, yel_min, yel_max)
        if ids:
            massmat.append(3)
            continue
    
    # по красным фигурам
        ids = obrezkaMat(image, red_min, red_max)
        if ids:
            massmat.append(1)
            continue

    #ser.write((str(massmat[1])+str(massmat[0])).encode('utf-8'))
    print((str(massmat[1])+str(massmat[0])).encode('utf-8'))


def matrix_final():
    while True:
        _, frame = cap.read()
        if frame[0][0][0] != 0:
            #cv2.imwrite('cfin.png', frame)
            break

    frame = cv2.imread('cfin.png')

    M = cv2.getRotationMatrix2D((640 // 2, 480 // 2), 82, 1.0)
    img = cv2.warpAffine(frame, M, (640, 480))

    floorOne = 248
    floorTwo = 331
    floorThree = 432

    pillarOne = 210
    pillarTwo = 296
    pillarThree = 385

    finalMap = ""

    img00 = img[floorOne:floorOne+15, pillarOne:pillarOne+15]
    img00 = segimg(img00)
    watchimg(img00)
    img01 = img[floorOne:floorOne+15, pillarTwo:pillarTwo+15, ]
    img01 = segimg(img01)
    img02 = img[floorOne:floorOne+15, pillarThree:pillarThree+15]
    img02 = segimg(img02)

    img10 = img[floorTwo:floorTwo+15, pillarOne:pillarOne+15]
    img10 = segimg(img10)
    img11 = img[floorTwo:floorTwo+15, pillarTwo:pillarTwo+15]
    img11 = segimg(img11)
    img12 = img[floorTwo:floorTwo+15, pillarThree:pillarThree+15]
    img12 = segimg(img12)

    img20 = img[floorThree:floorThree+15, pillarOne:pillarOne+15]
    img20 = segimg(img20)
    img21 = img[floorThree:floorThree+15, pillarTwo:pillarTwo+15]
    img21 = segimg(img21)
    img22 = img[floorThree:floorThree+15, pillarThree:pillarThree+15]
    img22 = segimg(img22)

    checkMas = [img00, img01, img02,
                img10, img11, img12,
                img20, img21, img22]


    for image in checkMas:

    # по синим фигурам
        ids = obrezka(image, blu_min, blu_max)
        if ids:
            finalMap += "4"
            print(4)
            continue

    # по зелёным фигурам
        ids= obrezka(image, gre_min, gre_max)
        if ids:
            finalMap += "2"
            print(2)
            continue

    # по жёлтым фигурам
        ids = obrezka(image, yel_min, yel_max)
        if ids:
            finalMap += "3"
            print(3)
            continue
    
    # по черным фигурам
        ids = obrezka(image, blm_min, blm_max)
        if ids:
            finalMap += "5"
            print(5)
            continue

    # по красным фигурам
        ids = obrezka(image, red_min, red_max)
        if ids:
            finalMap += "1"
            print(1)
            continue

        finalMap += "0"
    
    return finalMap


countVar = 0


red_min = np.array((0, 176, 104), np.uint8)  # 142, 100, 46
red_max = np.array((179, 255, 255), np.uint8) #196, 255, 150
    
blm_min = np.array((0, 0, 0), np.uint8) #0, 0, 0
blm_max = np.array((179, 255, 27), np.uint8) #255, 255, 12
         
gre_min = np.array((38, 121, 15), np.uint8)     #38, 121, 0
gre_max = np.array((104, 255, 255), np.uint8)  #104, 255, 255
    
yel_min = np.array((11, 116, 100), np.uint8)  # 11, 116, 125
yel_max = np.array((55,255, 255), np.uint8)  # 55, 200, 255
    
blu_min = np.array((100, 176, 16), np.uint8)  # 111, 176, 41
blu_max = np.array((131, 255, 255), np.uint8)  #131, 255, 255
    
    

while True:
    #if ser.in_waiting > 0:
        cap = cv2.VideoCapture(cameraPort)
        break

while True:
    #if ser.in_waiting > 0:
        if countVar == 0:
            building_map_fin = matrix_final()

            print(building_map_fin)

            #ser.write(building_map_fin.encode('utf-8'))
            countVar += 1

        elif (countVar > 0) and countVar < 6:
            while True:
                _, frame = cap.read()
                if frame[0][0][0] != 0:
                    #cv2.imwrite('c'+str(countVar)+'.png', frame)
                    break
            img = cv2.imread('c'+str(countVar)+'.png')
            materials_matrix()
            countVar += 1
            if (countVar == 6):
                #countVar = 0
                exit(0)
