#pragma once
#include <JuceHeader.h>

class AppConfig
{
public:
    struct AudioMapping
    {
        String mappingId;
        String inputDeviceId;
        String outputDeviceId;
        bool isMuted = false;
        float volume = 1.0f;
        String eqPresetName;
    };

    static AppConfig& getInstance();

    void load();
    void save();

    bool getLaunchAtStartup() const;
    void setLaunchAtStartup(bool enable);

    Array<AudioMapping> getAudioMappings() const;
    void setAudioMappings(const Array<AudioMapping>& mappings);

    String getLastInputDevice() const;
    void setLastInputDevice(const String& deviceId);

    String getLastOutputDevice() const;
    void setLastOutputDevice(const String& deviceId);

private:
    AppConfig() = default;
    PropertiesFile& getPropertiesFile();

    std::unique_ptr<PropertiesFile> propertiesFile;
};
