/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** Buffer implementation
*/
#pragma once
#include <string>
#include "../interfaces/IBuffer.hpp"

namespace Network {
namespace Implementation {

/**
 * @brief Implémentation concrète d'un buffer
 */
class Buffer : public IBuffer {
private:
    std::string _data;
    size_t _capacity;

public:
    explicit Buffer(size_t initialSize = 1024);

    std::string& getData() override;
    const std::string& getData() const override;
    void setData(const std::string& data) override;
    size_t size() const override;
    void resize(size_t newSize) override;
    void clear() override;
    std::string toString() const override;
};

} // namespace implementation
} // namespace Network
