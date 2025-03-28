#ifndef INSTR_H
#define INSTR_H

#include <functional>
#include <map>

namespace instr
{
// clang-format off
extern const std::map<int,std::map<int,std::function<void(signed int &s1, signed int &s2, signed int &s3)>>> instr_map;
// clang-format on
} // namespace instr

#endif /* INSTR_H_INCLUDED */
