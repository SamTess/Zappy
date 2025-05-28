/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** IException
*/

#ifndef IEXCEPTION_HPP_
#define IEXCEPTION_HPP_

#include <string>
#include <exception>


class IException : public std::exception {
    public:
        virtual ~IException() noexcept = default;
        const char* what() const noexcept override = 0;
        virtual std::string getMessage() const noexcept = 0;
        virtual std::string getType() const noexcept = 0;
        virtual std::string getFormattedMessage() const noexcept = 0;
};

#endif /* !IEXCEPTION_HPP_ */
