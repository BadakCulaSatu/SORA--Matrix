#include "AppConfig.h"

AppConfig& AppConfig::getInstance()
{
    static AppConfig instance;
    return instance;
}

PropertiesFile& AppConfig::getPropertiesFile()
{
    if (!propertiesFile)
    {
        PropertiesFile::Options options;
        options.applicationName = "SORA Matrix";
        options.filenameSuffix = ".settings";
        options.osxLibrarySubFolder = "Application Support";
        propertiesFile = std::make_unique<PropertiesFile>(options);
    }
    return *propertiesFile;
}

void AppConfig::load()
{
    auto& props = getPropertiesFile();
    // Load akan dipanggil saat startup
}

void AppConfig::save()
{
    auto& props = getPropertiesFile();
    props.saveIfNeeded();
}

bool AppConfig::getLaunchAtStartup() const
{
    return getPropertiesFile().getBoolValue("launchAtStartup", false);
}

void AppConfig::setLaunchAtStartup(bool enable)
{
    getPropertiesFile().setValue("launchAtStartup", enable);
    save();
}

Array<AppConfig::AudioMapping> AppConfig::getAudioMappings() const
{
    Array<AudioMapping> mappings;
    auto xmlStr = getPropertiesFile().getValue("audioMappings");
    
    if (auto xml = parseXML(xmlStr))
    {
        for (auto* child : xml->getChildIterator())
        {
            AudioMapping mapping;
            mapping.mappingId = child->getStringAttribute("id");
            mapping.inputDeviceId = child->getStringAttribute("input");
            mapping.outputDeviceId = child->getStringAttribute("output");
            mapping.isMuted = child->getBoolAttribute("muted");
            mapping.volume = (float)child->getDoubleAttribute("volume", 1.0);
            mapping.eqPresetName = child->getStringAttribute("eqPreset");
            mappings.add(mapping);
        }
    }
    
    return mappings;
}

void AppConfig::setAudioMappings(const Array<AudioMapping>& mappings)
{
    auto xml = std::make_unique<XmlElement>("Mappings");
    
    for (const auto& mapping : mappings)
    {
        auto* child = xml->createNewChildElement("Mapping");
        child->setAttribute("id", mapping.mappingId);
        child->setAttribute("input", mapping.inputDeviceId);
        child->setAttribute("output", mapping.outputDeviceId);
        child->setAttribute("muted", mapping.isMuted);
        child->setAttribute("volume", mapping.volume);
        child->setAttribute("eqPreset", mapping.eqPresetName);
    }
    
    getPropertiesFile().setValue("audioMappings", xml->toString());
    save();
}

String AppConfig::getLastInputDevice() const
{
    return getPropertiesFile().getValue("lastInputDevice");
}

void AppConfig::setLastInputDevice(const String& deviceId)
{
    getPropertiesFile().setValue("lastInputDevice", deviceId);
    save();
}

String AppConfig::getLastOutputDevice() const
{
    return getPropertiesFile().getValue("lastOutputDevice");
}

void AppConfig::setLastOutputDevice(const String& deviceId)
{
    getPropertiesFile().setValue("lastOutputDevice", deviceId);
    save();
}
