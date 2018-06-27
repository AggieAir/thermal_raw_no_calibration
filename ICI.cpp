#include <iostream>
#include "ICI.hpp"
#include "ICIFrameGrabber.h"
int ICI::setup_camera()
{
  iciCameraFinder.GetConnectedCameras( connected_ICI);
  if(connected_ICI.size() == 0)
  {
    std::cerr << "ERROR: Did not find ICI camera" << std::endl;
    return -1;
  }

  this->frame_grabber = connected_ICI.at(0);

  if(this->frame_grabber->StartCamera() != NO_ERROR)
  {
    std::cerr << "ERROR: Failed to connect to ICI camera" << std::endl;
  }

  this->frame_size = this->frame_grabber->GetWidth() * this->frame_grabber->GetHeight();

  return 0;
}

uint16_t * ICI::get_raw()
{
  uint16_t * image_frame = new uint16_t[this->frame_size * sizeof(uint16_t)];
  int * num_bytes_transferred = new int;
  if(this->frame_grabber->GetRawImage(image_frame, num_bytes_transferred) != NO_ERROR)
  {
    std::cerr << "ERROR: ICI camera failed to capture" << std::endl;
  }
  if(this->frame_size*2 + 27 != (unsigned int)(*num_bytes_transferred))
  {
    std::cerr << "ERROR: Did not receive the correct number of bytes from camera at capture" << std::endl;
    std::cerr << this->frame_size *2 +27 << " " << *num_bytes_transferred << std::endl;
  }
  return image_frame;
}
