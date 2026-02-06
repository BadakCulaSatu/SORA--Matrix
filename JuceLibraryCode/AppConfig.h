#pragma once

// =============================================================================
// Project Info
#define JUCE_PROJUCER_VERSION 0x70006

#define JucePlugin_Build_VST    0
#define JucePlugin_Build_VST3   0
#define JucePlugin_Build_AU     0
#define JucePlugin_Build_AUv3   0
#define JucePlugin_Build_RTAS   0
#define JucePlugin_Build_AAX    0
#define JucePlugin_Build_Standalone 1
#define JucePlugin_Build_Unity  0
#define JucePlugin_Build_LV2    0

// =============================================================================
// App Properties
#define JUCE_APPLICATION_NAME_STRING  "SORA Matrix"
#define JUCE_APPLICATION_VERSION_STRING "0.1.0"
#define JUCE_APPLICATION_VERSION 0x010000

// =============================================================================
// macOS Specific
#define JUCE_MAC                1
#define JUCE_WINDOWS           0
#define JUCE_LINUX             0
#define JUCE_IOS               0
#define JUCE_ANDROID           0

// Target Deployment
#define MACOSX_DEPLOYMENT_TARGET 110000
#define JUCE_PUSH_NOTIFICATIONS 0
#define JUCE_USE_COREMEDIA      1

// Architectures
#define JUCE_ARCH_x86_64       1
#define JUCE_ARCH_ARM64        1

// =============================================================================
// Bundle Identifier (Github username)
// GANTI "YOUR_GITHUB_USERNAME" dengan username GitHub Anda
#define JUCE_BUNDLE_IDENTIFIER  "com.github.YOUR_GITHUB_USERNAME.soramatrix"

// =============================================================================
// Audio Features
#define JUCE_MODULE_AVAILABLE_juce_audio_basics         1
#define JUCE_MODULE_AVAILABLE_juce_audio_devices        1
#define JUCE_MODULE_AVAILABLE_juce_audio_formats        1
#define JUCE_MODULE_AVAILABLE_juce_audio_processors     1
#define JUCE_MODULE_AVAILABLE_juce_audio_utils          1
#define JUCE_MODULE_AVAILABLE_juce_core                 1
#define JUCE_MODULE_AVAILABLE_juce_cryptography         0
#define JUCE_MODULE_AVAILABLE_juce_data_structures      1
#define JUCE_MODULE_AVAILABLE_juce_events               1
#define JUCE_MODULE_AVAILABLE_juce_graphics             1
#define JUCE_MODULE_AVAILABLE_juce_gui_basics           1
#define JUCE_MODULE_AVAILABLE_juce_gui_extra            1
#define JUCE_MODULE_AVAILABLE_juce_opengl               0
#define JUCE_MODULE_AVAILABLE_juce_osc                  0
#define JUCE_MODULE_AVAILABLE_juce_product_unlocking    0
#define JUCE_MODULE_AVAILABLE_juce_video                0

// =============================================================================
// UI Features
#define JUCE_WEB_BROWSER       0
#define JUCE_USE_CURL          0
#define JUCE_USE_MP3AUDIOFORMAT 0

// =============================================================================
// Code Signing (optional for Hackintosh)
#define JUCE_CODE_SIGNING     0
