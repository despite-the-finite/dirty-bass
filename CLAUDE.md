# Dirty Bass — Claude Code context

## Project

JUCE VST3 synthesiser plugin. Manufacturer: mm studio.

## Build

```powershell
# Configure (run once)
& "C:\Program Files\CMake\bin\cmake.exe" -B build -A x64

# Build
& "C:\Program Files\CMake\bin\cmake.exe" --build build --config Release
```

Output: `build\DirtyBass_artefacts\Release\VST3\Dirty Bass.vst3`

JUCE is at `C:\dev\oss\JUCE` (hardcoded in CMakeLists.txt).

## File map

| File | Role |
|------|------|
| `CMakeLists.txt` | Build config — plugin metadata, format targets, linked modules |
| `PluginProcessor.h/cpp` | `juce::AudioProcessor` — owns the `juce::Synthesiser`, defines all APVTS parameters |
| `PluginEditor.h/cpp` | `juce::AudioProcessorEditor` — 9-knob rotary UI, dark theme |
| `DirtyBassVoice.h/cpp` | `juce::SynthesiserVoice` + `DirtyBassSound` — all DSP lives here |

## DSP notes

- Oscillators: manual phase accumulation (sawtooth), sub runs at `phaseIncrement * 0.5`
- Waveshaper: `tanh(x * gain) / tanh(gain)` — `driveNorm` is computed once per block, not per sample
- Filter: `juce::dsp::StateVariableTPTFilter<float>` (lowpass), prepared per-voice in `DirtyBassVoice::prepare()`
- Envelope: `juce::ADSR` — parameters updated each block so live tweaks take effect immediately
- `renderBuffer` is pre-allocated in `prepare()` and reused via `setSize(..., avoidReallocating=true)` to avoid heap allocation on the audio thread

## Parameters (APVTS IDs)

`drive`, `cutoff`, `resonance`, `subMix`, `volume`, `attack`, `decay`, `sustain`, `release`

## Git

Remote: `git@github.com:despite-the-finite/dirty-bass.git`
