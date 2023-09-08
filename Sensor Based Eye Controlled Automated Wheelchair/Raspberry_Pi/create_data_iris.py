import cv2 as cv
import cv2
import numpy as np
import mediapipe as mp 
mp_face_mesh = mp.solutions.face_mesh

# left eyes indices
LEFT_EYE =[ 362, 382, 381, 380, 374, 373, 390, 249, 263, 466, 388, 387, 386, 385,384, 398 ]
# right eyes indices
RIGHT_EYE=[ 33, 7, 163, 144, 145, 153, 154, 155, 133, 173, 157, 158, 159, 160, 161 , 246 ] 

# irises Indices list
LEFT_IRIS = [464,465, 466, 467]
RIGHT_IRIS = [459, 460, 461, 462]

cap = cv.VideoCapture(0)
per=0
with mp_face_mesh.FaceMesh(max_num_faces=1,min_detection_confidence=0.5,min_tracking_confidence=0.5) as face_mesh:

    while True:
        ret, frame = cap.read()
        if not ret:
            break
        frame = cv.flip(frame, 1)
        img=frame.copy()
        rgb_frame = cv.cvtColor(frame, cv.COLOR_BGR2RGB)
        img_h, img_w = frame.shape[:2]
        results = face_mesh.process(rgb_frame)
        mask = np.zeros((img_h, img_w), dtype=np.uint8)

        if results.multi_face_landmarks:
            # print((results.multi_face_landmarks[0]))

            # [print(p.x, p.y, p.z ) for p in results.multi_face_landmarks[0].landmark]
            
            mesh_points=np.array([np.multiply([p.x, p.y], [img_w, img_h]).astype(int) 
            for p in results.multi_face_landmarks[0].landmark])

            #cv.polylines(frame, [mesh_points[LEFT_IRIS]], True, (0,255,0), 1, cv.LINE_AA)
            #cv.polylines(frame, [mesh_points[RIGHT_IRIS]], True, (0,255,0), 1, cv.LINE_AA)
            
            (l_cx, l_cy), l_radius = cv.minEnclosingCircle(mesh_points[LEFT_IRIS])
            (r_cx, r_cy), r_radius = cv.minEnclosingCircle(mesh_points[RIGHT_IRIS])
            center_left = np.array([l_cx, l_cy], dtype=np.int32)
            center_right = np.array([r_cx, r_cy], dtype=np.int32)
            cv.circle(frame, center_left, int(l_radius), (0,255,0), 2, cv.LINE_AA)
            x=center_left[0]
            y=center_left[1]
            crop=frame[x:x+100,y:y+100,:]


            #cv.circle(frame, center_right, int(r_radius), (0,255,0), 2, cv.LINE_AA)

            cv.circle(frame, center_left, 1, (0,255,0), -1, cv.LINE_AA)
            #cv.circle(frame, center_right, 1, (0,255,0), -1, cv.LINE_AA)

            # drawing on the mask 
            cv.circle(mask, center_left, int(l_radius), (255,255,255), -1, cv.LINE_AA)
            #cv.circle(mask, center_right, int(r_radius), (255,255,255), -1, cv.LINE_AA)
            contours,_ = cv2.findContours(mask,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
            x,y,w,h = cv2.boundingRect(contours[0]) 
            if w>10 and h>10:
                
                new_img=img[y:y+h,x:x+w,:]
                new_img=cv2.resize(new_img,(60,60))
                cv2.imwrite('./iris_temp/'+str(per)+'.jpg',new_img)
                per=per+1;
           
            
        cv.imshow('Mask', mask)     
        cv.imshow('img', frame)
        key = cv.waitKey(1)
        if key ==ord('q'):
            break
cap.release()
cv.destroyAllWindows()
