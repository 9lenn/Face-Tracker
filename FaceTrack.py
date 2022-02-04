#import libraries
import cv2
import time
import numpy
from multiprocessing import Process
from multiprocessing import Queue
from functools import partial
from picamera.array import PiRGBArray
from picamera import PiCamera
import multiprocessing as mp
import os

# Load the face detection model
net = cv2.dnn.readNet('/home/pi/buildnew/face-detection-adas-0001.xml', '/home/pi/buildnew/face-detection-adas-0001.bin')


# Specify target device
net.setPreferableTarget(cv2.dnn.DNN_TARGET_MYRIAD)
if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyACM1', 28800)
    ser.flush()

# Misc vars
font = cv2.FONT_HERSHEY_SIMPLEX
frameWidth = 320
frameHeight = 240


#Start and warm iup camera
camera = PiCamera()
camera.rotation = 180
camera.resolution = (320,240)
camera.framerate = 35
rawCapture = PiRGBArray(camera, size=(320,240))
time.sleep(0.1)

"""
# USBcam
cap = cv2.VideoCapture(0)

#Get the camera data:
def capProperties():
print("[info] W, H, FPS")
print(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
print(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
print(cap.get(cv2.CAP_PROP_FPS))

capProperties()
cap.set(cv2.CAP_PROP_FRAME_WIDTH, frameWidth)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, frameHeight)
"""

#time the frame rate....
start = time.time()
frames = 0

def classify_frame(net, inputQueue, outputQueue):
#loop the function
while True:
if not inputQueue.empty():
# grab the frame from camera and create a blob
frame = inputQueue.get()
frame = cv2.resize(frame, (300, 300))

blob = cv2.dnn.blobFromImage(frame, size=(300, 300), ddepth=cv2.CV_8U)
net.setInput(blob)
out = net.forward()

#Outputting detections
outputQueue.put(out)

#initializing queue and fps
inputQueue = Queue(maxsize=1)
outputQueue = Queue(maxsize=1)
out = None

# constructing arg child process
print("[INFO] starting process...")
p = Process(target=classify_frame, args=(net,inputQueue,outputQueue,))
p.daemon = True
p.start()
print("[INFO] starting capture...")
face_count = 0

for frame in camera.capture_continuous(rawCapture, format="rgb", use_video_port=True):
#while(True):#USBcam
# Capture frame-by-frame
#ret, frame = cap.read()
frame = frame.array

#if statement to check if queue empty or not
if inputQueue.empty():
inputQueue.put(frame)
if not outputQueue.empty():
out = outputQueue.get()


#if statement to check if out of detection
if out is not None:
#loop and draw the detections
for detection in out.reshape(-1, 7):

confidence = float(detection[2])

xmin = int(detection[3] * frame.shape[1])
ymin = int(detection[4] * frame.shape[0])
xmax = int(detection[5] * frame.shape[1])
ymax = int(detection[6] * frame.shape[0])

if confidence > 0.9:
face_count += 1
print(face_count)
#placing border around face
cv2.rectangle(frame, (xmin, ymin), (xmax, ymax), color=(230, 0, 0))

#labeling border and serial code
cv2.rectangle(frame, (xmin-1, ymin-1),\
(xmin+60, ymin-10), (0,255,255), -1)
#labeltext
cv2.putText(frame,'Face'+ str(face_count) +':' + str(round(confidence,2)),\
(xmin,ymin-2), font, 0.3,(0,0,0),1,cv2.LINE_AA)

#if statement for only motors move when facecount =  1

if facecount == 1:
    string='{0:d}:{1:d}'.format((xmax-xmin//2),(ymax-ymin//2))
    print(string)
    ser.write(string.encode('utf-8'))
    line = ser.readline().decode('utf-8').rstrip()
    print(string)
elif
    print('waiting for one person only')
                   
     
blob = cv2.dnn.blobFromImage(facearea, size=(62, 62), ddepth=cv2.CV_8U)

net1.setInput(blob)

out1 = net1.forward()

num_age = out1[0][0][0][0]

num_sex = out1[0][1][0][0]

age = int(num_age*100)


'''
# Display the resulting frame
cv2.namedWindow('frame',cv2.WINDOW_NORMAL)
cv2.resizeWindow('frame',frameWidth,frameHeight)
newframe = cv2.cvtColor(frame,cv2.COLOR_BGR2RGB)
cv2.imshow('frame',newframe)
rawCapture.truncate(0)

frames+=1

#press q to stop script
if cv2.waitKey(1) & 0xFF == ord('q'):
        break

#tracks fps
end = time.time()
seconds = end-start
fps = frames/seconds
print("Average FPS: "+str(fps))

#end the script
#cap.release()
cv2.destroyAllWindows()
