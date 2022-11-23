import cv2
import numpy as np
import apriltag

imagepath = "../../data/imgs/v2_3280_2464_april.jpg"
image = cv2.imread(imagepath)
image = cv2.resize(image, [image.shape[1]//10, image.shape[0]//10])
gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
print("[INFO] detecting AprilTags...")
options = apriltag.DetectorOptions(families="tag36h11")
detector = apriltag.Detector(options)
results = detector.detect(gray)
print("[INFO] {} total AprilTags detected".format(len(results)))

print(type(results))

# Loop and display
for r in results:
    # extract the bounding box (x, y) coordinates for the apriltag and
    # convert each of the (x, y) coordinates to integers
    (ptA, ptB, ptC, ptD) = r.corners
    ptB = (int(ptB[0]), int(ptB[1]))
    ptC = (int(ptC[0]), int(ptC[1]))
    ptD = (int(ptD[0]), int(ptD[1]))
    ptA = (int(ptA[0]), int(ptA[1]))

    # draw the bounding box
    cv2.line(image, ptA, ptB, (0, 255, 0), 2)
    cv2.line(image, ptB, ptC, (0, 255, 0), 2)
    cv2.line(image, ptC, ptD, (0, 255, 0), 2)
    cv2.line(image, ptD, ptA, (0, 255, 0), 2)

    # draw the center coord
    (cX, cY) = (int(r.center[0]), int(r.center[1]))
    print("cX {}, cY {}".format(cX, cY))
    cv2.circle(image, (cX, cY), 5, (0, 0, 255), -1)

cv2.imshow("Image", image)
cv2.waitKey(0)

cv2.imwrite("v2_april_highlight.png", image)
    
