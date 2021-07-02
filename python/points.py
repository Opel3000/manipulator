import cv2


def check(image):
    image = cv2.circle(image, (188,248), radius=1, color=(0, 0, 255), thickness=-1)
    image = cv2.circle(image, (208,248), radius=1, color=(0, 0, 255), thickness=-1)
    image = cv2.circle(image, (188,228), radius=1, color=(0, 0, 255), thickness=-1)
    image = cv2.circle(image, (208,228), radius=1, color=(0, 0, 255), thickness=-1)
    image = cv2.circle(image, (355,225), radius=1, color=(0, 0, 255), thickness=-1)
    image = cv2.circle(image, (335,205), radius=1, color=(0, 0, 255), thickness=-1)
    image = cv2.circle(image, (355,205), radius=1, color=(0, 0, 255), thickness=-1)
    image = cv2.circle(image, (335,225), radius=1, color=(0, 0, 255), thickness=-1)
    return image


def checkfin(image):
    cv2.rectangle(image, (210+15, 248+15), (210, 248), (0, 0, 255), -1)
    cv2.rectangle(image, (296+15, 248+15), (296, 248), (0, 0, 255), -1)
    cv2.rectangle(image, (385+15, 248+15), (385, 248), (0, 0, 255), -1)
    
    cv2.rectangle(image, (210+15, 331+15), (210, 331), (0, 0, 255), -1)
    cv2.rectangle(image, (296+15, 331+15), (296, 331), (0, 0, 255), -1)
    cv2.rectangle(image, (385+15, 331+15), (385, 331), (0, 0, 255), -1)
    
    cv2.rectangle(image, (210+15, 432+15), (210, 432), (0, 0, 255), -1)
    cv2.rectangle(image, (296+15, 432+15), (296, 432), (0, 0, 255), -1)
    cv2.rectangle(image, (385+15, 432+15), (385, 432), (0, 0, 255), -1)

    return image


img = cv2.imread('c1.png')
img = check(img)
img1 = cv2.imread('c2.png')
img1 = check(img1)
img2 = cv2.imread('c3.png')
img2 = check(img2)
img3 = cv2.imread('c4.png')
img3 = check(img3)
img4 = cv2.imread('c5.png')
img4 = check(img4)
img5 = cv2.imread('cfin.png')
M = cv2.getRotationMatrix2D((640 // 2, 480 // 2), 82, 1.0)
img5 = cv2.warpAffine(img5, M, (640, 480))
img5 = checkfin(img5)



cv2.imshow('img', img)
cv2.imshow('img1', img1)
cv2.imshow('img2', img2)
cv2.imshow('img3', img3)
cv2.imshow('img4', img4)
cv2.imshow('img4', img4)
cv2.imshow('img5', img5)
cv2.waitKey(0)
cv2.destroyAllWindows()
