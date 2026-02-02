import serial
from vpython import *
import time

# --- CONNECTION ---
try:
    arduinoData = serial.Serial('COM4', 115200)
    time.sleep(2)
except:
    print("PORT ERROR: Close Serial Monitor!")
    exit()

# --- SCENE ---
scene.title = "MPU6050 orientation"
scene.width = 1000
scene.height = 700
scene.range = 5
scene.forward = vector(0, -0.5, -1)

# Initialize box
myBox = box(length=4, height=0.2, width=2, color=color.cyan, axis=vector(0,0,1))
xAxis = arrow(axis=vector(0,0,1), color=color.red, shaftwidth=0.1)

while True:
    try:
        if arduinoData.in_waiting > 0:
            line = arduinoData.readline().decode('utf-8').strip()
            splitData = line.split(',')
            
            if len(splitData) == 3:
                pitch = -float(splitData[1]) * (pi/180.0) 
                roll  = float(splitData[0]) * (pi/180.0) 
                yaw   = float(splitData[2]) * (pi/180.0)

               
                k = vector(-sin(yaw)*cos(pitch), sin(pitch), -cos(yaw)*cos(pitch))
                
                y_ref = vector(0, 1, 0)
                side = cross(k, y_ref)
                up_stabilized = cross(side, k)
                
                # Apply rotations
                myBox.axis = k
                myBox.up = up_stabilized.rotate(angle=roll, axis=k)
                xAxis.axis = k * 1.5
                
    except Exception as e:
        print(f"Error: {e}")
        break
