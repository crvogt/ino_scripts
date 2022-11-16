from time import sleep
from picamera import PiCamera

camera = PiCamera()
camera.resolution = camera.MAX_RESOLUTION
print(camera.resolution)
#camera.resolution = (1024, 768)
camera.start_preview(resolution=(1024, 768))

sleep(5)
camera.capture('v2_cam_target.jpg')
