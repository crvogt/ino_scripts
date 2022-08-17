import cv2
import numpy as np
import apriltag

imagepath = "../../data/apriltags/apriltag_robots.jpg"
image = cv2.imread(imagepath)
gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
print("[INFO] detecting AprilTags...")
detector = apriltag("tag36h11")
results = detector.detect(gray)
print("[INFO] {} total AprilTags detected".format(len(results)))

# Loop and display
for r in results:
#    # extract the bounding box (x, y) coordinates for the apriltag and
#    # convert each of the (x, y) coordinates to integers
    (cX, cY) = (int(r['center'][0]), int(r['center'][1]))
    print("cX {}, cY {}".format(cX, cY))
    cv2.circle(image, (cX, cY), 5, (0, 0, 255), -1)

cv2.imshow("Image", image)
cv2.waitKey(0)
    
