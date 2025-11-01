#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"
#include "PlaylistComponent.h"


class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::Timer
{
public:
    PlayerGUI(PlayerAudio& player);
    ~PlayerGUI() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void timerCallback() override;

    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

    //void timerCallback() override;

private:
    PlayerAudio& audioPlayer;

    PlaylistComponent playlist;
    juce::TextButton loadPlaylistButton{ "Load Playlist" };
    juce::TextButton playSelectedButton{ "Play Selected" };

    juce::Label metadataLabel;

    juce::TextButton playButton{ "Play" };
    juce::TextButton stopButton{ "Stop" };
    juce::TextButton loadButton{ "Load" };
    juce::TextButton muteButton{ "Mute" };
    juce::TextButton goStartButton{ "Go Start" };
    juce::TextButton goEndButton{ "Go End" };
    juce::ToggleButton repeatingButton{ "Repeat" };

    juce::Slider positionSlider;
    juce::Slider volumeSlider;
    juce::Slider speedSlider;

    juce::FileChooser chooser{ "Select an audio file..." };

    juce::Label volumeLabel;
    juce::Label speedLabel;
    juce::Label positionLabel;
};

