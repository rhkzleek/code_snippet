/**
 * @file Book.hpp
 * @author caogh (caoguanghuaplus@163.com)
 * @brief
 * @version 0.1
 * @date 2021-12-22
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef __BOOK_HPP__
#define __BOOK_HPP__
#include "LibMat.hpp"

class Book : public LibMat
{
public:
    Book(const std::string &title, const std::string &author) : _title(title), _author(author)
    {
        std::cout << "Book::Book(" << _title << ", " << _author << ") constructor\n";
    }
    virtual ~Book()
    {
        std::cout << "Book::Book() destructor\n";
    }

    virtual void print() const
    {
        std::cout << "Book::print() -- I am a Book object\n"
                  << "My title is: " << _title << '\n'
                  << "My author is: " << _author << std::endl;
    }

    const std::string &title() const { return _title; }
    const std::string &author() const { return _author; }

protected:
    std::string _title;
    std::string _author;
};

#endif