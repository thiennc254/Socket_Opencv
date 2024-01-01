# python 2 3
# pip
# opencv-python
# numpy
# socket

import socket
import numpy as np
import cv2

PORT = 8080
SERVER_IP = "127.0.0.1"

def server():
  server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
  server_socket.bind((SERVER_IP,PORT))
  server_socket.listen(5)

  print(f"[*] Listening on {SERVER_IP}:{PORT}")

  client_socket, addr = server_socket.accept()
  print(f"[*] Connect from {addr[0]}:{addr[1]}")

  imgSize = client_socket.recv(1024)
  imgSize = int.from_bytes(imgSize, byteorder='big')
  data = b""

  while len(data) < imgSize:
    package = client_socket.recv(min(imgSize - len(data), 1024))
    if not package:
      break
    data += package

  print(f"Data size: {len(data)}")
  np_data = np.frombuffer(data, dtype=np.uint8)

  try:
    received_img = cv2.imdecode(np_data, cv2.IMREAD_UNCHANGED)
    if received_img is not None:
      cv2.imshow("RECEIVED IMAGE", received_img)
      cv2.waitKey(0)
      cv2.destroyAllWindows()
    else:
      print(f"ERROR decoding image")
  except Exception as e:
    print(f"ERROR: {e}")
    
  client_socket.close()
  server_socket.close()
if __name__  == "__main__":
  while True:
    server()