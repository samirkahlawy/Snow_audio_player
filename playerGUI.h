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
    void mouseDown(const juce::MouseEvent& event);

private:
    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbnailCache{ 5 }; 
    juce::AudioThumbnail thumbnail{ 512, formatManager, thumbnailCache };

    double currentPosition = 0.0;
    double totalLength = 0.0;

    void drawWaveform(juce::Graphics& g, juce::Rectangle<int> area);

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
    juce::TextButton goForwardButton{ "10s Forward" };
    juce::TextButton gobackButton{ "10s Fackword" };
    juce::ToggleButton repeatingButton{ "Repeat all" };
    juce::TextButton setLoopPointsButton{ "Set loop points" };
    juce::TextButton clearLoopPointsButton{ "Clear loop points" };

    juce::Slider loopStartSlider;
    juce::Slider loopEndSlider;

    juce::Label loopStartLabel;
    juce::Label loopEndLabel;

    double tempLoopStart = 0.0;
    double tempLoopEnd = 0.0;

    juce::Slider positionSlider;
    juce::Slider volumeSlider;
    juce::Slider speedSlider;

    juce::FileChooser chooser{ "Select an audio file..." };

    juce::Label volumeLabel;
    juce::Label speedLabel;
    juce::Label positionLabel;
};
