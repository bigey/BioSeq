#ifndef SA_H_
#define SA_H_

#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>

struct SAcomp;
std::vector<size_t> buildSA(const std::string&);
std::vector<size_t> buildLCP(const std::string& s, std::vector<size_t>& sa);

#endif /* SA_H_ */
