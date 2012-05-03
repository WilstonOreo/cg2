#include <tbd/declare_property.h>

namespace cg2 
{
  #define RND float(rand())/float(RAND_MAX)

  #define U8(a) (a) > 1.0 ? 255 : (a) < 0.0 ? 0 : (u8)((a)*255.0f)

/*
  template<class T> T getComp(string& s, int n)
{
	istringstream iss(s);
	vector<string> tokens;
	copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter<vector<string> >(tokens));

	if (n >= tokens.size()) return string();
	return tokens[n];
}*/

}

