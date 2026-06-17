cmake_minimum_required(VERSION 3.22)

project(DirtyBass VERSION 1.0.0)

add_subdirectory("C:/dev/oss/JUCE" JUCE)

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
