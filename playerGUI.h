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
    juce::File getCurrentPlayingFile();


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
    juce::Label artistLabel;

    juce::TextButton playButton{ "Play" };
    juce::TextButton stopButton{ "Stop" };
    juce::TextButton loadButton{ "Load" };
    juce::TextButton muteButton{ "Mute" };
    juce::TextButton goStartButton{ "Go Start" };
    juce::TextButton goEndButton{ "Go End" };
    juce::TextButton goForwardButton{ "10s Forward" };
    juce::TextButton gobackButton{ "10s Backword" };
    juce::ToggleButton repeatingButton{ "Repeat all" };
    juce::TextButton setLoopPointsButton{ "Set loop points" };
    juce::TextButton clearLoopPointsButton{ "Clear loop points" };
    juce::TextButton mixButton{ "Mix two tracks" };

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

    class MixWindow : public juce::Button::Listener, public juce::Component
    {
    public:
        MixWindow(PlayerAudio& player, PlaylistComponent& playlistComp);
        void paint(juce::Graphics& g)override;
        void resized()override;
        void buttonClicked(juce::Button* button)override;

    private:
        PlayerAudio& audioPlayer;
        PlaylistComponent& playlist;

        juce::TextButton  browseButton1{ "Select Track 1 " };
        juce::TextButton  browseButton2{ "Select Track 2 " };
        juce::TextButton  mixButton{ "Mix Tracks" };
        juce::TextButton  cancelButton{ "Cancel" };

        juce::Label track1Label;
        juce::Label track2Label;
        juce::Label infoLabel;

        juce::File track1File;
        juce::File track2File;

        juce::AudioFormatManager formatManager;

        void loudTrackForMixing(int trackNumber);
        void performMixing();

        std::unique_ptr<juce::FileChooser> fileChooser;


        juce::Image PlayButton;
    };
    std::unique_ptr<MixWindow> mixWindow;
};
