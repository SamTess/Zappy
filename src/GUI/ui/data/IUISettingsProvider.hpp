/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** IUISettingProvider
*/

#ifndef IUI_SETTINGS_PROVIDER_HPP_
#define IUI_SETTINGS_PROVIDER_HPP_

namespace GUI {

class IUISettingsProvider {
    public:
        virtual ~IUISettingsProvider() = default;

        virtual bool getSfxEnabled() const = 0;
        virtual float getMusicVolume() const = 0;
        virtual void setSfxEnabled(bool enabled) = 0;
        virtual void setMusicVolume(float volume) = 0;
};

} // namespace GUI

#endif /* !IUI_SETTINGS_PROVIDER_HPP_ */
