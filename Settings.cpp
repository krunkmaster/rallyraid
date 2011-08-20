
#include "Settings.h"
#include "ConfigFile.h"
#include "StringConverter.h"

Settings* Settings::settings = 0;
const std::string Settings::settingsFilename = "data/Dakar2012_settings.cfg";

void Settings::initialize()
{
    if (settings == 0)
    {
        settings = new Settings();
    }
}

void Settings::finalize()
{
    if (settings)
    {
        delete settings;
        settings = 0;
    }
}


Settings::Settings()
    : preloadObjects(false),
      cacheObjects(true),
      targetFps(60),
      nonshaderLight(false),
      objectWireSize(250),
      objectWireNum(7),
      objectDensity(10),
      useTerrainDetail(true),
      showNames(true),
      difficulty(2),
      navigationAssistant(true),
      resolutionX(1280),
      resolutionY(800),
      displayBits(16),
      driverType("opengl"),
      fullScreen(true),
      vsync(false),
      scanForJoystick(true),
      shadowMapSize(2048),
      joystickDeadZone(0.01f),
      manualGearShifting(false),
      sequentialGearShifting(true)
{
    read();
}

Settings::~Settings()
{
}

void Settings::read()
{
    ConfigFile cf;
    cf.load(settingsFilename);

    dprintf(MY_DEBUG_NOTE, "Read settings file:\n");
    // Go through all sections & settings in the file
    ConfigFile::SectionIterator seci = cf.getSectionIterator();

    std::string secName, keyName, valName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        dprintf(MY_DEBUG_NOTE, "\tSection: %s\n", secName.c_str());
        ConfigFile::SettingsMultiMap *settings = seci.getNext();
        ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            keyName = i->first;
            valName = i->second;
            dprintf(MY_DEBUG_NOTE, "\t\ttype: %s, arch: %s\n", keyName.c_str(), valName.c_str());

            if (keyName == "preload_objects")
            {
                preloadObjects = StringConverter::parseBool(valName, false);
            } else if (keyName == "cache_objects")
            {
                cacheObjects = StringConverter::parseBool(valName, true);
            } else if (keyName == "target_fps")
            {
                targetFps = StringConverter::parseUnsignedInt(valName, 60);
            } else if (keyName == "nonshader_light")
            {
                nonshaderLight = StringConverter::parseBool(valName, false);
            //} else if (keyName == "object_wire_size")
            //{
            //    objectWireSize = StringConverter::parseUnsignedInt(valName, 250);
            //} else if (keyName == "object_wire_num")
            //{
            //    objectWireNum = StringConverter::parseUnsignedInt(valName, 7);
            } else if (keyName == "object_density")
            {
                objectDensity = StringConverter::parseUnsignedInt(valName, 10);
            } else if (keyName == "use_terrain_detail")
            {
                useTerrainDetail = StringConverter::parseBool(valName, true);
            } else if (keyName == "show_names")
            {
                showNames = StringConverter::parseBool(valName, true);
            } else if (keyName == "difficulty")
            {
                difficulty = StringConverter::parseUnsignedInt(valName, 2);
                if (difficulty > 4) difficulty = 4;
            } else if (keyName == "navigation_assistant")
            {
                navigationAssistant = StringConverter::parseBool(valName, true);
            } else if (keyName == "resolution_x")
            {
                resolutionX = StringConverter::parseUnsignedInt(valName, 1280);
            } else if (keyName == "resolution_y")
            {
                resolutionY = StringConverter::parseUnsignedInt(valName, 800);
            } else if (keyName == "display_bits")
            {
                displayBits = StringConverter::parseUnsignedInt(valName, 16);
                if (displayBits != 16 || displayBits != 32)
                {
                    displayBits = 16;
                }
            } else if (keyName == "driver_type")
            {
                driverType = valName;
                if (driverType == "D3D9" || driverType == "d3d9")
                {
                    driverType = "d3d9";
                }
                else
                {
                    driverType = "opengl";
                }
            } else if (keyName == "full_screen")
            {
                fullScreen = StringConverter::parseBool(valName, true);
            } else if (keyName == "vsync")
            {
                vsync = StringConverter::parseBool(valName, false);
            } else if (keyName == "scan_for_joystick")
            {
                scanForJoystick = StringConverter::parseBool(valName, true);
            } else if (keyName == "shadow_map_size")
            {
                shadowMapSize = StringConverter::parseUnsignedInt(valName, 2048);
            } else if (keyName == "joystick_dead_zone")
            {
                joystickDeadZone = StringConverter::parseFloat(valName, 0.01f);
            } else if (keyName == "manual_gear_shifting")
            {
                manualGearShifting = StringConverter::parseBool(valName, false);
            } else if (keyName == "sequential_gear_shifting")
            {
                sequentialGearShifting = StringConverter::parseBool(valName, true);
            }
        }
    }
}

void Settings::write()
{
    FILE* f;
    int ret = 0;

    dprintf(MY_DEBUG_NOTE, "Write settings file:\n");

    errno_t error = fopen_s(&f, settingsFilename.c_str(), "w+");
    if (error)
    {
        dprintf(MY_DEBUG_ERROR, "Unable to open settings file for write: %s\n", settingsFilename.c_str());
        return;
    }

    ret = fprintf(f, "preload_objects=%s\n", preloadObjects?"yes":"no");
    ret = fprintf(f, "cache_objects=%s\n", cacheObjects?"yes":"no");
    ret = fprintf(f, "target_fps=%u\n", targetFps);
    ret = fprintf(f, "nonshader_light=%s\n", nonshaderLight?"yes":"no");
    ret = fprintf(f, "object_wire_size=%u\n", objectWireSize);
    ret = fprintf(f, "object_wire_num=%u\n", objectWireNum);
    ret = fprintf(f, "object_density=%u\n", objectDensity);
    ret = fprintf(f, "use_terrain_detail=%s\n", useTerrainDetail?"yes":"no");
    ret = fprintf(f, "show_names=%s\n", showNames?"yes":"no");
    ret = fprintf(f, "difficulty=%u\n", difficulty);
    ret = fprintf(f, "navigation_assistant=%s\n", navigationAssistant?"yes":"no");
    ret = fprintf(f, "resolution_x=%u\n", resolutionX);
    ret = fprintf(f, "resolution_y=%u\n", resolutionY);
    ret = fprintf(f, "display_bits=%u\n", displayBits);
    ret = fprintf(f, "driver_type=%s\n", driverType.c_str());
    ret = fprintf(f, "full_screen=%s\n", fullScreen?"yes":"no");
    ret = fprintf(f, "vsync=%s\n", vsync?"yes":"no");
    ret = fprintf(f, "scan_for_joystick=%s\n", scanForJoystick?"yes":"no");
    ret = fprintf(f, "shadow_map_size=%u\n", shadowMapSize);
    ret = fprintf(f, "joystick_dead_zone=%f\n", joystickDeadZone);
    ret = fprintf(f, "manual_gear_shifting=%s\n", manualGearShifting?"yes":"no");
    ret = fprintf(f, "sequential_gear_shifting=%s\n", sequentialGearShifting?"yes":"no");

    fclose(f);
}
