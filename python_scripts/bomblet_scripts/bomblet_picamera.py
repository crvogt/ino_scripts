import picamera
import time
import os
import RPi.GPIO as GPIO

# Setup video file name
base = "/home/pi/launch_recordings/"
# Check if dir exists, if not, create it
if not os.path.isdir(base):
    os.mkdir(base)
ext = ".h264"
vid_start = "bomblet_vid_"
vid_file = ""

# Setup LED indicator
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)
GPIO.setup(11, GPIO.OUT, initial=GPIO.LOW)

camera = picamera.PiCamera()
# Recording time in minutes
# at 1024x600 this is 10M per min at default FPS
recording_time = 90
# Convert to seconds
recording_time *= 60

# Allow values to settle
time.sleep(2)

# Create video file, checking it's unique
dir_objs = os.listdir(base)
vid_count = 0
while True:
    vid_file = vid_start + str(vid_count).zfill(4) + ext
    if vid_file in dir_objs:
        vid_count += 1
    else:
        break
vid_file = base + vid_file

camera.start_recording(vid_file)
GPIO.output(11, GPIO.HIGH)
camera.wait_recording(recording_time)
GPIO.output(11, GPIO.LOW)
