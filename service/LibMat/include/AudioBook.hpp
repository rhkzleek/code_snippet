/**
 * @file AudioBook.hpp
 * @author caogh (caoguanghuaplus@163.com)
 * @brief
 * @version 0.1
 * @date 2021-12-22
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef __AUDITBOOK_HPP__
#define __AUDITBOOK_HPP__
#include "Book.hpp"

class AudioBook : public Book
{
public:
    AudioBook(const std::string &title, const std::string &author, const std::string &narrator) : Book(title, author), _narrator(narrator)
    {
        std::cout << "AudioBook::AuditBook(" << _title
                  << ", " << _author << ", " << _narrator << ") constructor" << std::endl;
    }
    virtual ~AudioBook()
    {
        std::cout << "AuditBook::~AuditBook() destructor!\n";
    }

    virtual void print() const
    {
        std::cout << "AudioBook::print() -- I am an AuditBook object !\n"
                  << "My title is:" << _title << '\n'
                  << "My author is:" << _author << '\n'
                  << "My narrator is:" << _narrator << '\n';
    }

    const std::string &narrator() const { return _narrator; }

protected:
    std::string _narrator;
};

#endif