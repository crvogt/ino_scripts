import picamera
import time
import os

camera = picamera.PiCamera()
iso_val = 100
exp_val = 32000
# Recording time in minutes
# at 1024x600 this is 10M per min at default FPS
recording_time = 90
# Convert to seconds
recording_time *= 60

# How we want to run the code
tune_camera = False
record_vid = True

# Setup video file name
base = "/home/pi/launch_recordings"
# Check if dir exists, if not, create it
if not os.path.isdir(base):
    print("{} does not exist... creating...".format(base))
    os.mkdir(base)
ext = ".h264"
vid_file = "launch_vid_"

# Set iso value and allow values to settle
camera.iso = iso_val
time.sleep(2)

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
    # Create video file, checking it's unique
    dir_objs = os.listdir(base)
    vid_count = 0
    vid_file = vid_file + str(vid_count).zfill(4)
    while True:
        if vid_file in dir_objs:
            vid_count += 1
        else:
            break
    print("Recording to file {}...")
    # camera.start_recording(vid_file)
    # camera.wait_recording(recording_time)
