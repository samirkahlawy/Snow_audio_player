

#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"

class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener
{
public:
    PlayerGUI(PlayerAudio& player);
    ~PlayerGUI() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
private:
    PlayerAudio& audioPlayer;

    juce::TextButton playButton{ "play" };
    juce::TextButton stopButton{ "stop" };
    juce::TextButton loadButton{ "load" };
    juce::TextButton muteButton{ "mute" };
    juce::TextButton goStartButton{ "go start" };
    juce::TextButton goEndButton{ "go end" };
    juce::ToggleButton repeatingButton{ "repeat" };


    juce::Slider volumeSlider;
    juce::Slider speedSlider;

    juce::FileChooser chooser{ "Select an audio file..." };
};
