//
// Created by vhs on 11/2/21.
//

#ifndef MYNESEMULATOR_TYPES_H
#define MYNESEMULATOR_TYPES_H
#define MAX_GAME_SIZE 0xBFE0
#include <array>
#include <vector>
#include <string>
#include <memory>
typedef char Byte;
using std::array;
using std::vector;
using std::string;
using std::shared_ptr;

enum FileFormats{

    iNES,
    NES_20

};
#endif //MYNESEMULATOR_TYPES_H
