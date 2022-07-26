import picamera
import time
import board
import digitalio

arduino_pin = digitalio.DigitalInOut(board.D4)
arduino_pin.direction = digitalio.Direction.OUTPUT
print("pre sleep")
time.sleep(2)
print("Setting value to 0")
arduino_pin.value = 0
time.sleep(10)
#camera = picamera.PiCamera()
#camera.resolution = (640, 480)
#camera.start_recording('test_video.h264')

print("Recording...")
# Start Arduino code
arduino_pin.value = 1
# Begin camera recording
#camera.wait_recording(60)
time.sleep(10)
print("Done recording...")
# Stop Arduino code
arduino_pin.value = 0
# Stop camera recording
#camera.stop_recording()
