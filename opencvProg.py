            xx = 0
            while True:
                ret, frame = cap.read()
                cv2.imshow('img1', frame)
                cv2.waitKey(100)
                xx += 1
                if xx > 30:
                    break
