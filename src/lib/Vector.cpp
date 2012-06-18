#include "cg2/Vector.hpp"

using namespace std;

namespace cg2
{
  void Matrix4f::print()
  {
    for (int i = 0; i < 4; i++)
    {
      for (int j = 0; j < 4; j++)
      {
        cout << get(j,i) << "\t";
      }
      cout << endl;
    }

  }
  /*
    string Point3f::str()
    {
      stringstream ss; ss << "(" << x << "," << y << "," << z << ")";
      return ss.str();
    }

    string Point3f::fromString(string s)
    {

    }
  */


}
