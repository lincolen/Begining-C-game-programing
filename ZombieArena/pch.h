// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

#ifndef PCH_H
#define PCH_H

// TODO: add headers that you want to pre-compile here
//reutrns a base if it is the range between low and high, return low or high otherwise
template<typename T> inline T clamp(const T & base, const T & low, const T & high) {
  return std::min(high, std::max(base, low));
}

const double PI = 3.141;

#endif //PCH_H


