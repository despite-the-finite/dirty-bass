#include "DirtyBassVoice.h"

DirtyBassVoice::DirtyBassVoice (juce::AudioProcessorValueTreeState& apvtsRef)
    : apvts (apvtsRef)
{
    filter.setType (juce::dsp::StateVariableTPTFilterType::lowpass);
}

void DirtyBassVoice::prepare (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate      = sampleRate;
    spec.maximumBlockSize = (juce::uint32) samplesPerBlock;
    spec.numChannels     = 1;

    filter.prepare (spec);
    filter.reset();
    adsr.setSampleRate (sampleRate);
    renderBuffer.setSize (1, samplesPerBlock);
}

bool DirtyBassVoice::canPlaySound (juce::SynthesiserSound* sound)
{
    return dynamic_cast<DirtyBassSound*> (sound) != nullptr;
}

void DirtyBassVoice::startNote (int midiNoteNumber, float velocity,
                                juce::SynthesiserSound*, int /*currentPitchWheelPosition*/)
{
    currentVelocity  = velocity;
    phase    = 0.0;
    subPhase = 0.0;
    phaseIncrement = juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber) / getSampleRate();

    juce::ADSR::Parameters params;
    params.attack  = apvts.getRawParameterValue ("attack")->load();
    params.decay   = apvts.getRawParameterValue ("decay")->load();
    params.sustain = apvts.getRawParameterValue ("sustain")->load();
    params.release = apvts.getRawParameterValue ("release")->load();
    adsr.setParameters (params);
    adsr.noteOn();
}

void DirtyBassVoice::stopNote (float /*velocity*/, bool allowTailOff)
{
    if (allowTailOff)
        adsr.noteOff();
    else
    {
        adsr.reset();
        clearCurrentNote();
    }
}

float DirtyBassVoice::generateSaw (double& ph, double increment)
{
    float sample = (float) (2.0 * ph - 1.0);
    ph += increment;
    if (ph >= 1.0) ph -= 1.0;
    return sample;
}

void DirtyBassVoice::renderNextBlock (juce::AudioBuffer<float>& outputBuffer,
                                      int startSample, int numSamples)
{
    if (! adsr.isActive())
    {
        clearCurrentNote();
        return;
    }

    const float drive     = apvts.getRawParameterValue ("drive")->load();
    const float cutoff    = apvts.getRawParameterValue ("cutoff")->load();
    const float resonance = apvts.getRawParameterValue ("resonance")->load();
    const float subMix    = apvts.getRawParameterValue ("subMix")->load();
    const float volume    = apvts.getRawParameterValue ("volume")->load();

    // Update ADSR in real time so tweaks take effect immediately
    juce::ADSR::Parameters params;
    params.attack  = apvts.getRawParameterValue ("attack")->load();
    params.decay   = apvts.getRawParameterValue ("decay")->load();
    params.sustain = apvts.getRawParameterValue ("sustain")->load();
    params.release = apvts.getRawParameterValue ("release")->load();
    adsr.setParameters (params);

    filter.setCutoffFrequency (cutoff);
    filter.setResonance (resonance);

    // Compute drive normalisation constant once per block (not per sample)
    const float driveGain = 1.0f + drive * 18.0f;
    const float driveNorm = std::tanh (driveGain);

    // Use pre-allocated mono render buffer (avoids heap allocation on audio thread)
    renderBuffer.setSize (1, numSamples, false, false, true);
    renderBuffer.clear();
    auto* mono = renderBuffer.getWritePointer (0);

    for (int i = 0; i < numSamples; ++i)
    {
        float saw = generateSaw (phase,    phaseIncrement);
        float sub = generateSaw (subPhase, phaseIncrement * 0.5);

        // Mix saw + sub oscillator, keep level normalised
        float mixed = (saw + sub * subMix) / (1.0f + subMix);

        // Soft-clip waveshaper for the "dirty" character
        float driven = std::tanh (mixed * driveGain) / driveNorm;

        mono[i] = driven * adsr.getNextSample() * currentVelocity * volume;
    }

    // Low-pass filter
    juce::dsp::AudioBlock<float> block (renderBuffer);
    juce::dsp::ProcessContextReplacing<float> ctx (block);
    filter.process (ctx);

    // Add mono result to every output channel (stereo spread handled by host)
    for (int ch = 0; ch < outputBuffer.getNumChannels(); ++ch)
        outputBuffer.addFrom (ch, startSample, renderBuffer, 0, 0, numSamples);

    if (! adsr.isActive())
        clearCurrentNote();
}
