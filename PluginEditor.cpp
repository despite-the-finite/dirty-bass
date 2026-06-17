#include "PluginProcessor.h"
#include "PluginEditor.h"

DirtyBassEditor::DirtyBassEditor (DirtyBassProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p),
      driveAttach     (p.apvts, "drive",     driveSlider),
      cutoffAttach    (p.apvts, "cutoff",    cutoffSlider),
      resonanceAttach (p.apvts, "resonance", resonanceSlider),
      subMixAttach    (p.apvts, "subMix",    subMixSlider),
      volumeAttach    (p.apvts, "volume",    volumeSlider),
      attackAttach    (p.apvts, "attack",    attackSlider),
      decayAttach     (p.apvts, "decay",     decaySlider),
      sustainAttach   (p.apvts, "sustain",   sustainSlider),
      releaseAttach   (p.apvts, "release",   releaseSlider)
{
    auto setupKnob = [this] (juce::Slider& s, juce::Label& l, const juce::String& name)
    {
        s.setSliderStyle (juce::Slider::RotaryVerticalDrag);
        s.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 64, 16);
        addAndMakeVisible (s);

        l.setText (name, juce::dontSendNotification);
        l.setJustificationType (juce::Justification::centred);
        l.setFont (juce::Font (11.0f, juce::Font::bold));
        l.setColour (juce::Label::textColourId, juce::Colour (0xffaaaacc));
        addAndMakeVisible (l);
    };

    setupKnob (driveSlider,     driveLabel,     "DRIVE");
    setupKnob (cutoffSlider,    cutoffLabel,    "CUTOFF");
    setupKnob (resonanceSlider, resonanceLabel, "RESO");
    setupKnob (subMixSlider,    subMixLabel,    "SUB");
    setupKnob (volumeSlider,    volumeLabel,    "VOL");
    setupKnob (attackSlider,    attackLabel,    "ATTACK");
    setupKnob (decaySlider,     decayLabel,     "DECAY");
    setupKnob (sustainSlider,   sustainLabel,   "SUSTAIN");
    setupKnob (releaseSlider,   releaseLabel,   "RELEASE");

    setSize (500, 300);
}

DirtyBassEditor::~DirtyBassEditor() {}

void DirtyBassEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff12121e));

    // Title bar
    g.setColour (juce::Colour (0xff1e1e32));
    g.fillRect (0, 0, getWidth(), 38);

    g.setColour (juce::Colour (0xffe94560));
    g.setFont (juce::Font (20.0f, juce::Font::bold));
    g.drawText ("DIRTY BASS", 0, 0, getWidth(), 38, juce::Justification::centred);

    // Section labels
    g.setFont (juce::Font (9.5f, juce::Font::bold));
    g.setColour (juce::Colour (0xff505070));
    g.drawText ("TONE",     12,  42, 80, 12, juce::Justification::left);
    g.drawText ("ENVELOPE", 12, 180, 80, 12, juce::Justification::left);

    // Separator between sections
    g.setColour (juce::Colour (0xff2a2a45));
    g.drawHorizontalLine (176, 12.0f, (float) getWidth() - 12.0f);
}

void DirtyBassEditor::resized()
{
    const int knobW  = 88;
    const int knobH  = 88;
    const int labelH = 14;
    const int gap    = 8;

    int x = 12, y = 54;

    auto placeKnob = [&] (juce::Slider& s, juce::Label& l)
    {
        s.setBounds (x, y, knobW, knobH);
        l.setBounds (x, y + knobH + 2, knobW, labelH);
        x += knobW + gap;
    };

    // Row 1 — tone controls
    placeKnob (driveSlider,     driveLabel);
    placeKnob (cutoffSlider,    cutoffLabel);
    placeKnob (resonanceSlider, resonanceLabel);
    placeKnob (subMixSlider,    subMixLabel);
    placeKnob (volumeSlider,    volumeLabel);

    // Row 2 — envelope
    x = 12;
    y = 192;

    placeKnob (attackSlider,  attackLabel);
    placeKnob (decaySlider,   decayLabel);
    placeKnob (sustainSlider, sustainLabel);
    placeKnob (releaseSlider, releaseLabel);
}
