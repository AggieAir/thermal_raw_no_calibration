#include "ICI.hpp"

int main()
{
  ICI ici;
  ici.setup_camera();
  for(int i = 0; i < 20; i++)
  {
    ici.get_raw();
  }
  return 0;
}
