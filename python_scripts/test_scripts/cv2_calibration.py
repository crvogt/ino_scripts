import numpy as np
import cv2 as cv 
import glob

'''
See https://stackoverflow.com/questions/66225558/cv2-findchessboardcorners-fails-to-find-corners
and
https://docs.opencv.org/4.x/dc/dbb/tutorial_py_calibration.html
'''

criteria = (cv.TERM_CRITERIA_EPS + cv.TERM_CRITERIA_MAX_ITER, 30, 0.001)

objp = np.zeros((6*7, 3), np.float32)
objp[:,:2] = np.mgrid[0:7, 0:6].T.reshape(-1,2)

objpoints = []
imgpoints = []

images = glob.glob('*.jpg')

for fname in images:
    print(fname)
    img = cv.imread(fname)
    img = cv.resize(img, (img.shape[1]//4, img.shape[0]//4))
    gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
    cv.imshow("gry", gray)
    cv.waitKey(5000)
    ret, corners = cv.findChessboardCorners(gray, (5,3),  None)
    print(ret)
    print(corners)
''' 
    print("Going to if statement...")
    if ret == True:
        objpoints.append(objp)

        corners2 = cv.cornerSubPix(gray, corners, (11,11), (-1, -1), criteria)
        imgpoints.append(corners2)

        cv.drawChessboardCorners(img, (7,6), corners2, ret)
        print("Showing img...")
        cv.imshow('img', img)
        cv.waitKey(15)

cv.destroyAllWindows()'''
