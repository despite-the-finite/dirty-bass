cmake_minimum_required(VERSION 3.22)

project(DirtyBass VERSION 1.0.0)

# ---------------------------------------------------------------------------
# JUCE
#
# By default JUCE is downloaded at configure time, so a clean checkout (or a
# CI runner) builds with no setup. To build against a local copy instead --
# much faster for repeat builds, and what the build here uses:
#     cmake -B build -A x64 -DJUCE_PATH=C:/dev/oss/JUCE
# ---------------------------------------------------------------------------
if (DEFINED JUCE_PATH AND NOT JUCE_PATH STREQUAL "")
    message(STATUS "DirtyBass: using local JUCE at ${JUCE_PATH}")
    add_subdirectory(${JUCE_PATH} ${CMAKE_BINARY_DIR}/JUCE EXCLUDE_FROM_ALL)
else()
    include(FetchContent)
    message(STATUS "DirtyBass: fetching JUCE 8.0.4")
    FetchContent_Declare(JUCE
        GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
        GIT_TAG        8.0.4
        GIT_SHALLOW    TRUE)
    FetchContent_MakeAvailable(JUCE)
endif()

juce_add_plugin(DirtyBass
    COMPANY_NAME "mm studio"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    PLUGIN_MANUFACTURER_CODE Dvsc
    PLUGIN_CODE Drtb
    FORMATS VST3 Standalone
    PRODUCT_NAME "Dirty Bass")

target_sources(DirtyBass
    PRIVATE
        PluginProcessor.cpp
        PluginEditor.cpp
        DirtyBassVoice.cpp)

target_compile_definitions(DirtyBass
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(DirtyBass
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
