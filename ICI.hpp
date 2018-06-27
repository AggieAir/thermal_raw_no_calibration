#ifndef ICI_HPP
#define ICI_HPP
#include "ICIFrameGrabber.h"
#include <fstream>
#include <string>
#include <chrono>
#include <vector>

class ICI
{
  public:
    int setup_camera();
    uint16_t * get_raw();

  private:
    ICIFrameGrabber * frame_grabber;
    unsigned int frame_size;
    std::vector<ICIFrameGrabber *> connected_ICI;
    ICICameraFinder iciCameraFinder;
};
#endif
