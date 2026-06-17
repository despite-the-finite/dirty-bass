# TODO

- [ ] Fix Font deprecation warnings (JUCE 8 requires FontOptions constructor — affects PluginEditor.cpp:24, 53, 57)
- [ ] Add COPY_PLUGIN_AFTER_BUILD to CMakeLists.txt so the VST3 auto-installs to Common Files\VST3 on build
- [ ] Implement anti-aliasing on the sawtooth oscillators (currently generates aliasing at high pitches)
- [ ] Add a pitch wheel handler in DirtyBassVoice (pitchWheelMoved is stubbed out)
- [ ] Add mono/legato mode option — dirty bass is typically played monophonically
- [ ] Add a second oscillator waveform choice (square/pulse in addition to saw)
- [ ] Consider PolyBLEP anti-aliasing for the oscillators before releasing publicly
- [ ] Test plugin loading in FL Studio after rescanning VST3 path
