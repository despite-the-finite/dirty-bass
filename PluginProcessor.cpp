#include "PluginProcessor.h"
#include "PluginEditor.h"

DirtyBassProcessor::DirtyBassProcessor()
    : AudioProcessor (BusesProperties()
                      .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "Parameters", createParameterLayout())
{
    for (int i = 0; i < 8; ++i)
        synthesiser.addVoice (new DirtyBassVoice (apvts));

    synthesiser.addSound (new DirtyBassSound());
}

DirtyBassProcessor::~DirtyBassProcessor() {}

juce::AudioProcessorValueTreeState::ParameterLayout DirtyBassProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        "drive",     "Drive",
        juce::NormalisableRange<float> (0.0f, 1.0f), 0.3f));

    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        "cutoff",    "Cutoff",
        juce::NormalisableRange<float> (20.0f, 18000.0f, 0.0f, 0.3f), 1200.0f));

    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        "resonance", "Resonance",
        juce::NormalisableRange<float> (0.1f, 10.0f, 0.0f, 0.5f), 0.7f));

    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        "attack",    "Attack",
        juce::NormalisableRange<float> (0.001f, 2.0f, 0.0f, 0.5f), 0.01f));

    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        "decay",     "Decay",
        juce::NormalisableRange<float> (0.001f, 2.0f, 0.0f, 0.5f), 0.2f));

    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        "sustain",   "Sustain",
        juce::NormalisableRange<float> (0.0f, 1.0f), 0.7f));

    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        "release",   "Release",
        juce::NormalisableRange<float> (0.001f, 4.0f, 0.0f, 0.5f), 0.2f));

    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        "subMix",    "Sub Mix",
        juce::NormalisableRange<float> (0.0f, 1.0f), 0.3f));

    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        "volume",    "Volume",
        juce::NormalisableRange<float> (0.0f, 1.0f), 0.8f));

    return { params.begin(), params.end() };
}

const juce::String DirtyBassProcessor::getName() const     { return JucePlugin_Name; }
bool   DirtyBassProcessor::acceptsMidi()   const           { return true;  }
bool   DirtyBassProcessor::producesMidi()  const           { return false; }
bool   DirtyBassProcessor::isMidiEffect()  const           { return false; }
double DirtyBassProcessor::getTailLengthSeconds() const    { return 0.5;   }
int    DirtyBassProcessor::getNumPrograms()                { return 1;     }
int    DirtyBassProcessor::getCurrentProgram()             { return 0;     }
void   DirtyBassProcessor::setCurrentProgram  (int)       {}
const juce::String DirtyBassProcessor::getProgramName (int) { return {};  }
void   DirtyBassProcessor::changeProgramName  (int, const juce::String&) {}

void DirtyBassProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synthesiser.setCurrentPlaybackSampleRate (sampleRate);

    for (int i = 0; i < synthesiser.getNumVoices(); ++i)
        if (auto* v = dynamic_cast<DirtyBassVoice*> (synthesiser.getVoice (i)))
            v->prepare (sampleRate, samplesPerBlock);
}

void DirtyBassProcessor::releaseResources() {}

bool DirtyBassProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}

void DirtyBassProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    juce::ScopedNoDenormals noDenormals;
    buffer.clear();
    synthesiser.renderNextBlock (buffer, midi, 0, buffer.getNumSamples());
}

bool DirtyBassProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor* DirtyBassProcessor::createEditor()
{
    return new DirtyBassEditor (*this);
}

void DirtyBassProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    if (auto xml = state.createXml())
        copyXmlToBinary (*xml, destData);
}

void DirtyBassProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary (data, sizeInBytes))
        if (xml->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xml));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DirtyBassProcessor();
}
