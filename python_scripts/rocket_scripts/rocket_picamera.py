import picamera
import time

camera = picamera.PiCamera()
iso_val = 100
exp_val = 32000
# Recording time in minutes
# at 1024x600 this is 10M per min at default FPS
recording_time = 30
# Convert to seconds
recording_time *= 60

tune_camera = False
flash_led = False
record_vid = True

# Set iso value and allow values to settle
camera.iso = iso_val
time.sleep(2)

if flash_led:
    print("Flashing LED...")
    counter = 0
    while counter < 10:
        camera.led = True
        time.sleep(1)
        camera.led = False
        time.sleep(1)
        counter += 1

if tune_camera:
    print("Camera resolution ", str(camera.resolution))
    print("Current iso value is ", str(camera.iso))
    camera.shutter_speed = camera.exposure_speed
    print("Camera shutter speed ", str(camera.exposure_speed))
    camera.exposure_mode = 'off'
    g = camera.awb_gains
    print("awb_gains ", str(g))
    camera.awb_mode = 'off'
    camera.awb_gains = g

elif record_vid:
    camera.exposure_mode = 'off'
    camera.shutter_speed = exp_val
    camera.start_recording('test_video
