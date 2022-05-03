#***********************************************************
# Name: Kristen Ung
# Date: Sept , 2021
# Title: EENG350_Assignment2.py
#
#***********************************************************
#
# Assignment 2 : Using OpenCV
#
# The purpose of this assignment is to gain experience with
# OpenCV and image processing. The assignment is split into
# 6 functions, each with a different purpose. The program
# prompts the user with which program they would like to
# execute.
#
#************************************************************

from picamera.array import PiRGBArray
from picamera import PiCamera
import time
import cv2 as cv
import numpy as np
from matplotlib import pyplot as plt


def cv_exercise1():
    camera = PiCamera()
    rawCapture = PiRGBArray(camera)

    # allow the camera to warmup
    time.sleep(0.1)

    try:
        camera.capture(rawCapture, format="bgr")
        image = rawCapture.array
    except:
        print("Failed to capture")

    try:
        cv.imwrite('cannyEdgeRaw.jpg', image)
    except:
        print('Save Failed')
        pass

    img = cv.imread('cannyEdgeRaw.jpg',0)
    edges = cv.canny(img,100,200)

    plt.subplot(121), plt.imshow(img, cmap='gray')
    plt.title('Original Image'), plt.xticks([]), plt.yticks([])
    plt.subplot(122), plt.imshow(edges, cmap='gray')
    plt.title('Edge Image'), plt.xticks([]), plt.yticks([])
    plt.show()

    return

def cv_exercise2():
    camera = PiCamera()
    rawCapture = PiRGBArray(camera)

    # allow the camera to warmup
    time.sleep(0.1)

    try:
        camera.capture(rawCapture, format="bgr")
        image = rawCapture.array
    except:
        print("Failed to capture")

    try:
        cv.imwrite('colorDetect.jpg', image)
    except:
        print('Save Failed')
        pass

    img = cv.imread('colorDetect.jpg',1)
    img = cv.resize(img, None, fx=0.5, fy=0.5, interpolation = cv.INTER_CUBIC)

    hsv = cv.cvtColor(img, cv.COLOR_BGR2HSV)

    lower = np.array([25, 146, 190], dtype = "uint8")
    upper = np.array([62, 174, 250], dtype = "uint8")

    mask = cv.inRange(hsv, lower, upper)

    res = cv.bitwise_and(img, img, mask = mask)

    cv.imshow('Original', img)
    cv.imshow('Isolated Yellow', res)
    cv.waitKey()
    cv.destroyAllWindows()

    return

def cv_exercise3():

    return

def cv_exercise4():

    return

def cv_exercise5():

    return

def cv_exercise6():

    return

programChoice = input('Which program would you like to execute (1-6, or q to quit)')

while programChoice != 'q':
    if programChoice == '1':
        cv_exercise1()
    elif programChoice == '2':
        cv_exercise2()
    elif programChoice == '3':
        cv_exercise3()
    elif programChoice == '4':
        cv_exercise4()
    elif programChoice == '5':
        cv_exercise5()
    elif programChoice == '6':
        cv_exercise6()
    elif programChoice == 'q':
        break
    else:
        print('Invalid input\n')

    programChoice = input('Which program would you like to execute (1-6, or q to quit)')