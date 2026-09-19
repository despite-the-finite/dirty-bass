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

## Getting the plugin

### Download a build

Grab the zip for your platform from the
[latest release](../../releases/latest) — that is the permanent download, and
it does not ask you to sign in.

Every push also builds all three platforms. For a build of something newer than
the last release, open the [Actions](../../actions) tab, pick the most recent
green run, and download the artefact for your platform (GitHub asks for a
sign-in before it hands over a run artefact, and those expire; release assets
do not).

### Install it

| Platform | Copy `Dirty Bass.vst3` to |
|---|---|
| Windows | `C:\Program Files\Common Files\VST3\` |
| macOS | `~/Library/Audio/Plug-Ins/VST3/` |
| Linux | `~/.vst3/` |

Then rescan plugins in your DAW.

## Building it yourself

Needs CMake 3.22+ and a C++ compiler — Visual Studio 2022 (Desktop C++
workload) on Windows. JUCE 8.0.4 is downloaded automatically at configure time.

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel
```

The VST3 bundle is output to:
```
build/DirtyBass_artefacts/Release/VST3/Dirty Bass.vst3
```

Already have JUCE checked out? Point at it and skip the download:

```bash
cmake -B build -DJUCE_PATH=C:/dev/oss/JUCE
```

On Linux you will also need:

```bash
sudo apt-get install libasound2-dev libfreetype-dev libfontconfig1-dev \
  libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev \
  libxcomposite-dev libxrender-dev libglu1-mesa-dev
```

## License

MIT
