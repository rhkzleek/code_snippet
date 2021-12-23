/**
 * @file LibMat.hpp
 * @author caogh (caoguanghuaplus@163.com)
 * @brief
 * @version 0.1
 * @date 2021-12-22
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef __LIBMAT_HPP__
#define __LIBMAT_HPP__
#include <iostream>
#include <string>
class LibMat
{
public:
    LibMat() { std::cout << "LibMat::LibMat() defult constructor!\n"; }

    virtual ~LibMat() { std::cout << "LibMat::~LibMat() destructor!\n"; }

    virtual void print() const { std::cout << "LibMat::print() -- I am a libmat!\n"; }
};

#endif