#pragma once

#include "PluginProcessor.h"

class DirtyBassEditor final : public juce::AudioProcessorEditor
{
public:
    explicit DirtyBassEditor (DirtyBassProcessor&);
    ~DirtyBassEditor() override;

    void paint   (juce::Graphics&) override;
    void resized () override;

private:
    DirtyBassProcessor& processorRef;

    juce::Slider driveSlider, cutoffSlider, resonanceSlider;
    juce::Slider subMixSlider, volumeSlider;
    juce::Slider attackSlider, decaySlider, sustainSlider, releaseSlider;

    juce::Label driveLabel, cutoffLabel, resonanceLabel;
    juce::Label subMixLabel, volumeLabel;
    juce::Label attackLabel, decayLabel, sustainLabel, releaseLabel;

    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    Attachment driveAttach, cutoffAttach, resonanceAttach;
    Attachment subMixAttach, volumeAttach;
    Attachment attackAttach, decayAttach, sustainAttach, releaseAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DirtyBassEditor)
};
