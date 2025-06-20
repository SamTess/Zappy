/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ITeam - Interface pour les équipes
*/

#ifndef ITEAM_HPP_
#define ITEAM_HPP_

#include <string>

class ITeam {
public:
    virtual ~ITeam() = default;
    
    virtual const std::string& getName() const = 0;
    virtual int getRemainingSlots() const = 0;
    
    virtual void setName(const std::string& name) = 0;
    virtual void setRemainingSlots(int slots) = 0;
};

#endif /* !ITEAM_HPP_ */
