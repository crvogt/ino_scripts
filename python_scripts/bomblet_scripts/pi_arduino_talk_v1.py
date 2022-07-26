import picamera
import time
import board
import digitalio
import cv2
import numpy as np

arduino_pin = digitalio.DigitalInOut(board.D4)
arduino_pin.direction = digitalio.Direction.OUTPUT

camera = picamera.PiCamera()
tune_camera = False
exp_val = 32000
iso_Val = 100
res_w = 640
res_h = 480
num_chan = 3

# Image values to say we've left rocket bay
threshold_met = False
# This will change based on resolution!
threshold_val = 500000

# Timing values to tell us when to stop recording
start_time = 0.0
current_time = 0.0
elapsed_time = 0.0
time_to_record = 30
# Convert to seconds
time_to_record *= 60

# Prep opencv image
cv_img = np.empty((res_w * res_h * num_chan), dtype=np.uint8)

if tune_camera:
    print("Camera resolution ", str(camera.resolution))
    print("Current iso value is ", str(camera_iso))
    camera.shutter_speed = camera.exposure_speed
    print("Camera shutter speed ", str(camera.exposure_speed))
    g = camera.awb_gains
    print("awb_gains ", str(g))
    camera.start_preview()
    time.sleep(20)
else:
    camera.resolution = (res_w, res_h)
    camera.exposure_mode = 'off'
    camera.shutter_speed = exp_val

    arduino_pin.value = 0

    while not threshold_met:
        print("In the dark...")
        camera.capture(cv_img, 'bgr')
        if cv_img.sum() > threshold_val:
            threshold_met = True
            arduino_pin.value = 1
            start_time = time.clock_gettime()
    
    img_count = 0
    while elapsed_time < time_to_record: 
        elapsed_time = time.clock_gettime() - start_time
        filename = str(img_count).zfill(5) + '.jpg'
        camera.capture(filename)
        img_count += 1

    # Stop Arduino code
    arduino_pin.value = 0
