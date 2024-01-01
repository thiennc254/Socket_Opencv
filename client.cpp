#include <iostream>
#include <cstdlib>

//OpenCV LIB
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>

//socket
// ffmpeg
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1" //thay bang ip may ao neu chay = may ao
#define PORT 8080 

using namespace std;
using namespace cv;

int main() {
  cout << "START PROGRAM !!!" << endl;

  VideoCapture cap;
  int apiID = cv::CAP_ANY;
  int deviceID = 0;
  cap.open(deviceID, apiID);

  if (!cap.isOpened()) {
    cerr << "ERROR!!! Deo mo duo cam" << endl;
    return -1;
  }

  Mat frame;
  cap >> frame;

  //Tao socket
  int clientSocket, ret;
  struct sockaddr_in serverAddr;
  char buffer[1024];

  clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (clientSocket < 0) {
    cerr << "ERROR!!! May cook d' mo duoc socket" << endl;
    return -1;
  }

  cout << "Mo socket thanh cong" << endl;

  memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(PORT);
  serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

  ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
  if (ret < 0) {
    cerr << "ERROR !!! May cook d' vao duoc socket";
    return -1;
  }

  long long imgSize = frame.total() * frame.elemSize();
  send(clientSocket, &imgSize, sizeof(imgSize), 0);

  std::vector<uchar> imgBuffer;
  imencode(".jpg", frame, imgBuffer);
  send(clientSocket, imgBuffer.data(), imgBuffer.size(), 0);
  close(clientSocket);

  imshow("SEND PICTURE", frame);
  waitKey(0);
  destroyAllWindows();
  return 0;
}