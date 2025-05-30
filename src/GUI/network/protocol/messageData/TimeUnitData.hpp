/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** TimeUnitData
*/

#ifndef TIME_UNIT_DATA_HPP_
#define TIME_UNIT_DATA_HPP_

#include "IMessageData.hpp"

class TimeUnitData : public IMessageData {
    public:
        explicit TimeUnitData(int timeUnit)
            : _timeUnit(timeUnit) {}
        MessageType getType() const override { return MessageType::TimeUnit; }

        int getTimeUnit() const { return _timeUnit; }

        void setTimeUnit(int value) { _timeUnit = value; }

    private:
        int _timeUnit;
};

#endif /* !TIME_UNIT_DATA_HPP_ */
