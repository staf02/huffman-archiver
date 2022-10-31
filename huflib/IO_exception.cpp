//
// Created by staf02 on 31/10/22.
//
#include "IO_exception.h"

IO_exception::IO_exception(const char *message) : runtime_error(message) {}

IO_exception::IO_exception(std::string const& message) : runtime_error(message) {

}

