/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** AException
*/

#ifndef AEXCEPTION_HPP_
#define AEXCEPTION_HPP_

#include "IException.hpp"
#include <string>

class AException : public IException {
    public:
        AException(const std::string& type, const std::string& message);
        virtual ~AException() noexcept = default;

        const char* what() const noexcept override;
        std::string getType() const noexcept override;
        std::string getMessage() const noexcept override;
        std::string getFormattedMessage() const noexcept override;

    private:
        std::string _message;
        std::string _type;
};

#endif /* !AEXCEPTION_HPP_ */
