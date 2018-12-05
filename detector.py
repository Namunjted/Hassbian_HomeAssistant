import sys
sys.path.insert(0,'person')

from person import *
import cv2,os
import numpy as np
from PIL import Image 
import pickle
import time
import json

Master = ["khanh","nam","vung"]


recognizer = cv2.createLBPHFaceRecognizer()
recognizer.load('trainer/trainer.yml')
cascadePath = "Classifiers/face.xml"
faceCascade = cv2.CascadeClassifier(cascadePath);
path = 'dataSet'
data = {"person":""}

cam = cv2.VideoCapture(0)
font = cv2.cv.InitFont(cv2.cv.CV_FONT_HERSHEY_SIMPLEX, 1, 1, 0, 1, 1) #Creates a font
count = 0
count1 = 0
stt = 0
while True:
    ret, im =cam.read()
    gray=cv2.cvtColor(im,cv2.COLOR_BGR2GRAY)
    faces=faceCascade.detectMultiScale(gray, scaleFactor=1.2, minNeighbors=5, minSize=(100, 100), flags=cv2.CASCADE_SCALE_IMAGE)
    for(x,y,w,h) in faces:
        nbr_predicted, conf = recognizer.predict(gray[y:y+h,x:x+w])
        cv2.rectangle(im,(x-50,y-50),(x+w+50,y+h+50),(225,0,0),2)
        if(conf < 70):
            nbr_predicted = person.person[nbr_predicted]["name"]
           # if (nbr_predicted in Master):
            #    count = count + 1;
                # time.sleep(1);
             #   print("detec"+ str(count))
          #  if(count==30):
           #     stt = stt + 1
              #print(nbr_predicted);
             #  data["person"] = nbr_predicted
                #client.publish('person/status',nbr_predicted);
              #  time.sleep(10);
               # cam.release()
                #cv2.destroyAllWindows()
                #exit(0)
        else:
            nbr_predicted = "unknown"
           # if(nbr_predicted not in Master):
            #   print("un detect" + str(count1))
            #if(count1==100):
             #   stt = stt + 1
              #  print(nbr_predicted);
              #  print(stt);
                #client.publish('person/status',nbr_predicted);
            #    time.sleep(10);
             #   cam.release()
              #  cv2.destroyAllWindows()
               # exit(0)
        cv2.cv.PutText(cv2.cv.fromarray(im),str(nbr_predicted)+"--"+str(conf), (x,y+h),font, 255) #Draw the text       
        cv2.imshow('im',im)
        if cv2.waitKey(1)&0xFF == ord('q'):
            cam.release()
            cv2.destroyAllWindows()
            exit(0)
# client.loop_stop()
# client.disconnect()
