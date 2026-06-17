# Dirty Bass

A VST3 synthesiser plugin built with [JUCE](https://github.com/juce-framework/JUCE). Designed for gritty, saturated bass sounds.

## Sound design

Each voice runs:
1. **Sawtooth oscillator** — fundamental bass tone
2. **Sub oscillator** — one octave below, mixed in via the Sub knob
3. **Tanh waveshaper** — soft-to-hard clipping for the "dirty" character
4. **ADSR envelope**
5. **State-variable low-pass filter** — tone shaping

8 voices of polyphony.

## Parameters

| Knob | Range | Description |
|------|-------|-------------|
| Drive | 0–1 | Waveshaper gain (1× to 19×). Higher = more saturation and harmonic content |
| Cutoff | 20–18000 Hz | Low-pass filter cutoff |
| Reso | 0.1–10 | Filter resonance (Q factor) |
| Sub | 0–1 | Sub oscillator level (one octave below) |
| Vol | 0–1 | Master output volume |
| Attack | 1ms–2s | Envelope attack time |
| Decay | 1ms–2s | Envelope decay time |
| Sustain | 0–1 | Envelope sustain level |
| Release | 1ms–4s | Envelope release time |

## Building

**Requirements:**
- Windows 10+
- Visual Studio 2022 (Desktop C++ workload)
- CMake 3.22+
- JUCE cloned somewhere on disk

```bash
# Clone JUCE if you haven't already
git clone https://github.com/juce-framework/JUCE.git C:/dev/oss/JUCE

# Configure (update the JUCE path in CMakeLists.txt if yours differs)
cmake -B build -A x64

# Build
cmake --build build --config Release
```

The VST3 bundle is output to:
```
build/DirtyBass_artefacts/Release/VST3/Dirty Bass.vst3
```

Copy that folder to `C:\Program Files\Common Files\VST3\` and rescan plugins in your DAW.

## License

MIT
