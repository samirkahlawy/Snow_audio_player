
#include "PlayerGUI.h"


PlayerGUI::PlayerGUI(PlayerAudio& player) : audioPlayer(player)
{
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(volumeSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(muteButton);
    addAndMakeVisible(goStartButton);
    addAndMakeVisible(goEndButton);
    addAndMakeVisible(repeatingButton);
    addAndMakeVisible(positionSlider);

    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    volumeSlider.addListener(this);
    speedSlider.addListener(this);
    muteButton.addListener(this);
    goStartButton.addListener(this);
    goEndButton.addListener(this);
    repeatingButton.addListener(this);
    positionSlider.addListener(this);

    volumeSlider.setRange(0.0, 1.0);
    volumeSlider.setValue(0.5);
    speedSlider.setRange(0.5, 2.0);
    speedSlider.setValue(1.0);


    startTimer(10);
}

PlayerGUI::~PlayerGUI() {}


void PlayerGUI::timerCallback()
{
    if (audioPlayer.isLouded() && audioPlayer.isPlaying())
    {
        double currentPos = audioPlayer.getPosition();
        positionSlider.setValue(currentPos, juce::dontSendNotification);
    }
}


void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.drawText("Audio Player", getLocalBounds(), juce::Justification::centredTop);
}

void PlayerGUI::resized()
{
    auto area = getLocalBounds().reduced(20);

    int buttonHeight = 40;
    int buttonWidth = 80;

    int gap = 10; 

    auto buttonArea = area.removeFromTop(buttonHeight);

    loadButton.setBounds(buttonArea.removeFromLeft(buttonWidth));
    buttonArea.removeFromLeft(gap);
    playButton.setBounds(buttonArea.removeFromLeft(buttonWidth));
    buttonArea.removeFromLeft(gap);
    stopButton.setBounds(buttonArea.removeFromLeft(buttonWidth));
    buttonArea.removeFromLeft(gap);
    muteButton.setBounds(buttonArea.removeFromLeft(buttonWidth));
    buttonArea.removeFromLeft(gap);
    goStartButton.setBounds(buttonArea.removeFromLeft(buttonWidth));
    buttonArea.removeFromLeft(gap); 
    goEndButton.setBounds(buttonArea.removeFromLeft(buttonWidth));
    buttonArea.removeFromLeft(gap);
    repeatingButton.setBounds(buttonArea.removeFromLeft(buttonWidth));
    buttonArea.removeFromLeft(gap);

    volumeSlider.setBounds(area.removeFromTop(40));
    area.removeFromTop(10); 
    speedSlider.setBounds(area.removeFromTop(40));
    area.removeFromTop(10);
    positionSlider.setBounds(area.removeFromTop(40));
    
}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &playButton)
        audioPlayer.start();
    else if (button == &stopButton)
        audioPlayer.stop();
    else if (button == &loadButton) {
        chooser.launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                juce::File file = fc.getResult();
                if (file.existsAsFile())
                {
                    audioPlayer.loadURL(juce::URL{ file });

                    double length = audioPlayer.getLengthInSeconds();
                    positionSlider.setRange(0.0, length);
                    positionSlider.setValue(0.0);
                }
            });
    }

    else if (button == &muteButton) {
        static bool isMute = false;
        isMute = !isMute;
        audioPlayer.setMute(isMute);
        muteButton.setButtonText(isMute?"un mute":"mute");
    }
    else if (button == &goStartButton) {
        audioPlayer.goStart();
    }
    else if (button == &goEndButton) {
        audioPlayer.goEnd();
    }
    else if (button == &repeatingButton) {
        audioPlayer.repeat(repeatingButton.getToggleState());
    }
      
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
        audioPlayer.setGain(slider->getValue());
    else if (slider == &speedSlider)
        audioPlayer.setSpeed(slider->getValue());
    else if (slider == &positionSlider)
    {
        audioPlayer.setPosition(slider->getValue());
    }
}






