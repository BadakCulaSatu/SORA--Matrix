SORA Matrix

Background audio router & plugin host for macOS. A MenuBar application with system-wide capture and multi-output routing.

Features

· MenuBar-only app - Runs in background, no Dock icon
· System-wide audio capture - Works with Blackhole 2ch driver (optional)
· Multi-output routing - Split one input to multiple outputs simultaneously
· Per-route audio processing - Mute, volume control, and 16-band parametric EQ
· EQ presets - Save and load custom EQ configurations
· Auto-launch - Remembers last configuration on startup
· Universal binary - Supports Intel and Apple Silicon (macOS 11+)

Requirements

· macOS 11.0 or later
· Blackhole 2ch (optional, for system-wide capture)

Installation

1. Download the latest .dmg from Releases
2. Install Blackhole 2ch if you need system audio capture
3. Drag SORA Matrix to your Applications folder

Usage

Click the menu bar icon to:

· Select audio input/output devices
· Add multiple routing mappings
· Control per-route volume, mute, and EQ
· Access parametric EQ editor (16 bands: Gain -24/+24, Freq 0-20kHz, Q 0.4-40)
· Quit the application

Building from Source

Requires JUCE Framework and Xcode command line tools.

```bash
git clone https://github.com/YOUR_GITHUB_USERNAME/sora-matrix
# Open SORA_Matrix.jucer in Projucer
# Export to Xcode project and build
```

License

GNU General Public License Versiin 3 - See LICENSE file for details.

Version

Current: 0.1.0 (Initial Release)
