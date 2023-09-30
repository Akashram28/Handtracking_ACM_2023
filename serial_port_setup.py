import serial
import random
import time
# Define the serial port and baud rate
ser = serial.Serial('COM4', 9600)  # Replace 'COMX' with your Arduino's serial port

import cv2
import mediapipe as mp
mp_drawing = mp.solutions.drawing_utils
mp_drawing_styles = mp.solutions.drawing_styles
mp_hands = mp.solutions.hands


import tkinter as tk
from tkinter import messagebox


def show_popup(n):
    popup = tk.Tk()
    popup.attributes("-fullscreen", True)
    popup.title("Guess Result")

    # Configure the font and background color
    label = tk.Label(popup, text=f"You Guessed:", font=("Helvetica", 32), foreground="#3498db")
    label.pack(expand=True, fill="both")

    # Center the label
    label.place(relx=0.5, rely=0.35, anchor="center")

    result_label = tk.Label(popup, text=f"{n} correctly", font=("Helvetica", 48), foreground="#2ecc71")
    result_label.pack(expand=True, fill="both")

    # Center the result label
    result_label.place(relx=0.5, rely=0.55, anchor="center")

    popup.after(5000, popup.destroy)  # Automatically close the popup after 3 seconds

    popup.mainloop()




cap = cv2.VideoCapture(0)
rand_int = random.randint(1,10)
ser.write(str(rand_int).encode() + b'\n')
n = 0

timeStamps = []

with mp_hands.Hands(
    model_complexity=0,
    min_detection_confidence=0.5,
    min_tracking_confidence=0.5) as hands:
  start = time.time()
  while cap.isOpened():
    if time.time() - start >= 20:
      break
    success, image = cap.read()
    if not success:
      print("Ignoring empty camera frame.")
      continue

    
    image.flags.writeable = False
    image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    results = hands.process(image)

    # Draw the hand annotations on the image.
    image.flags.writeable = True
    image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

    # Initially set finger count to 0 for each cap
    fingerCount = 0

    if results.multi_hand_landmarks:

      for hand_landmarks in results.multi_hand_landmarks:
        # Get hand index to check label (left or right)
        handIndex = results.multi_hand_landmarks.index(hand_landmarks)
        handLabel = results.multi_handedness[handIndex].classification[0].label

        # Set variable to keep landmarks positions (x and y)
        handLandmarks = []

        # Fill list with x and y positions of each landmark
        for landmarks in hand_landmarks.landmark:
          handLandmarks.append([landmarks.x, landmarks.y])

        if handLabel == "Left" and handLandmarks[4][0] > handLandmarks[3][0]:
          fingerCount = fingerCount+1
        elif handLabel == "Right" and handLandmarks[4][0] < handLandmarks[3][0]:
          fingerCount = fingerCount+1

        if handLandmarks[8][1] < handLandmarks[6][1]:       #Index finger
          fingerCount = fingerCount+1
        if handLandmarks[12][1] < handLandmarks[10][1]:     #Middle finger
          fingerCount = fingerCount+1
        if handLandmarks[16][1] < handLandmarks[14][1]:     #Ring finger
          fingerCount = fingerCount+1
        if handLandmarks[20][1] < handLandmarks[18][1]:     #Pinky
          fingerCount = fingerCount+1

        # Draw hand landmarks 
        mp_drawing.draw_landmarks(
            image,
            hand_landmarks,
            mp_hands.HAND_CONNECTIONS,
            mp_drawing_styles.get_default_hand_landmarks_style(),
            mp_drawing_styles.get_default_hand_connections_style())

    # Display finger count
    

    # Display image
    
    image = cv2.flip(image, 1)
    cv2.putText(image, str(fingerCount), (50, 450), cv2.FONT_HERSHEY_SIMPLEX, 3, (255, 0, 0), 10)
    cv2.putText(image, str(int(time.time()-start)), (50, 100), cv2.FONT_HERSHEY_SIMPLEX, 2, (0, 255, 0), 5)
    cv2.imshow('MediaPipe Hands', image)
    if cv2.waitKey(10) & 0xFF == ord('q'):
      
      break
    
    # Send the number of blinks to Arduino
    if fingerCount == rand_int:
        if n==0:
          timeStamps.append(time.time()-start)
        else:
          timeStamps.append(time.time() - timeStamps[n-1])
        n+=1
        temp = random.randint(1,10)
        while temp== rand_int:
          temp = random.randint(1,10)
        rand_int = temp
        ser.write(str(rand_int).encode() + b'\n')
# while True:
#     num = int(input("Enter number : "))
#     # if num == rand_int:
#     rand_int = random.randint(1,10)
#     ser.write(str(rand_int).encode() + b'\n')
#     if num == 0:
#         break
# cap.release()
ser.close()
cv2.destroyAllWindows()
cap.release()
show_popup(n)
print(timeStamps)
