#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

struct DirtyBassSound : public juce::SynthesiserSound
{
    bool appliesToNote    (int) override { return true; }
    bool appliesToChannel (int) override { return true; }
};

class DirtyBassVoice : public juce::SynthesiserVoice
{
public:
    explicit DirtyBassVoice (juce::AudioProcessorValueTreeState& apvts);

    void prepare (double sampleRate, int samplesPerBlock);

    bool canPlaySound (juce::SynthesiserSound* sound) override;
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound*, int) override;
    void stopNote  (float velocity, bool allowTailOff) override;
    void pitchWheelMoved (int) override {}
    void controllerMoved (int, int) override {}
    void renderNextBlock (juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

private:
    float generateSaw (double& phase, double increment);

    double phase    = 0.0;
    double subPhase = 0.0;
    double phaseIncrement = 0.0;
    float  currentVelocity = 0.0f;

    juce::ADSR adsr;
    juce::dsp::StateVariableTPTFilter<float> filter;
    juce::AudioBuffer<float> renderBuffer;

    juce::AudioProcessorValueTreeState& apvts;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DirtyBassVoice)
};
