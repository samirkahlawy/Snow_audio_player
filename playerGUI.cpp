#include "PlayerGUI.h"

PlayerGUI::PlayerGUI(PlayerAudio& player)
    : audioPlayer(player), playlist(player)

{
    formatManager.registerBasicFormats();

    setInterceptsMouseClicks(true, false);

    metadataLabel.setText("No file loaded", juce::dontSendNotification);
    metadataLabel.setJustificationType(juce::Justification::centred);
    metadataLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(metadataLabel);

    volumeLabel.setText("Volume", juce::dontSendNotification);
    speedLabel.setText("Speed", juce::dontSendNotification);
    positionLabel.setText("Position", juce::dontSendNotification);
    loopStartLabel.setText("Loop Start", juce::dontSendNotification);
    loopEndLabel.setText("Loop End", juce::dontSendNotification);

    for (auto* label : { &volumeLabel, &speedLabel, &positionLabel, &loopStartLabel, &loopEndLabel })
    {
        label->setJustificationType(juce::Justification::centredLeft);
        label->setColour(juce::Label::textColourId, juce::Colours::white);
        addAndMakeVisible(*label);
    }
    //playlist
    loadPlaylistButton.setButtonText("Load Playlist");
    addAndMakeVisible(loadPlaylistButton);

    playSelectedButton.setButtonText("Play Selected");
    addAndMakeVisible(playSelectedButton);

  
    addAndMakeVisible(playlist);

    loadPlaylistButton.addListener(this);
    playSelectedButton.addListener(this);

    // Buttons and sliders visible
    addAndMakeVisible(loadButton);
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(muteButton);
    addAndMakeVisible(goStartButton);
    addAndMakeVisible(goEndButton);
    addAndMakeVisible(repeatingButton);
    addAndMakeVisible(setLoopPointsButton);
    addAndMakeVisible(clearLoopPointsButton);
    addAndMakeVisible(goForwardButton);
    addAndMakeVisible(gobackButton);


    addAndMakeVisible(volumeSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(positionSlider);
    addAndMakeVisible(loopStartSlider);
    addAndMakeVisible(loopEndSlider);

    // Listeners
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    muteButton.addListener(this);
    goStartButton.addListener(this);
    goEndButton.addListener(this);
    goForwardButton.addListener(this);
    gobackButton.addListener(this);
    repeatingButton.addListener(this);
    setLoopPointsButton.addListener(this);
    clearLoopPointsButton.addListener(this);

    volumeSlider.addListener(this);
    speedSlider.addListener(this);
    positionSlider.addListener(this);
    loopStartSlider.addListener(this);
    loopEndSlider.addListener(this);


    // Slider ranges / defaults
    volumeSlider.setRange(0.0, 1.0);
    volumeSlider.setValue(0.5);
    speedSlider.setRange(0.5, 2.0);
    speedSlider.setValue(1.0);
    loopStartSlider.setRange(0.0, 10.0);
    loopEndSlider.setRange(0.0, 10.0);

    // start timer (interval in ms)
    startTimer(50); 
}

PlayerGUI::~PlayerGUI()
{
  
    playButton.removeListener(this);
    stopButton.removeListener(this);
    loadButton.removeListener(this);
    muteButton.removeListener(this);
    goStartButton.removeListener(this);
    goEndButton.removeListener(this);
    repeatingButton.removeListener(this);
    setLoopPointsButton.removeListener(this);
    clearLoopPointsButton.removeListener(this);

    volumeSlider.removeListener(this);
    speedSlider.removeListener(this);
    positionSlider.removeListener(this);
    loopStartSlider.removeListener(this);
    loopEndSlider.removeListener(this);
}

void PlayerGUI::timerCallback()
{
    if (audioPlayer.isLouded() && audioPlayer.isPlaying())
    {
        double currentPos = audioPlayer.getPosition();

        positionSlider.setValue(currentPos, juce::dontSendNotification);
        currentPosition = audioPlayer.getCurrentPosition();
        repaint();

        if (audioPlayer.isCustomLoopEnabled())
        {
            metadataLabel.setText("Custom Loop Active: " +
                juce::String(audioPlayer.getLoopStartTime(), 1) + "s - " +
                juce::String(audioPlayer.getLoopEndTime(), 1) + "s",
                juce::dontSendNotification);
        }
    }
}

void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::lightslategrey);

   
    auto bounds = getLocalBounds();
    auto sliderArea = positionSlider.getBounds();

    g.setColour(juce::Colours::whitesmoke);
    g.setFont(14.0f);
    auto area = getLocalBounds().reduced(10);
    auto waveformArea = area.removeFromTop(120);
    drawWaveform(g, waveformArea);

}


void PlayerGUI::resized()
{
    auto area = getLocalBounds().reduced(15);

    // --- 1)  Waveform ---
    auto waveformArea = area.removeFromTop(120);
    positionSlider.setBounds(waveformArea);

    // --- 2) buttons row---
    int buttonHeight = 35;
    int buttonWidth = 80;
    int gap = 7;
    auto buttonArea = area.removeFromTop(buttonHeight);
    juce::Array<juce::Button*> buttons = {
        &loadButton, &playButton, &stopButton, &muteButton,
        &goStartButton, &goEndButton, &goForwardButton, &gobackButton,
        &repeatingButton, &setLoopPointsButton, &clearLoopPointsButton
    };

    for (auto* btn : buttons)
    {
        if (buttonArea.getWidth() < buttonWidth)
            break;
        btn->setBounds(buttonArea.removeFromLeft(buttonWidth));
        buttonArea.removeFromLeft(gap);
    }

    area.removeFromTop(10);

    // --- 3) Playlist ---
    loadPlaylistButton.setBounds(area.removeFromTop(30).removeFromLeft(120));
    playSelectedButton.setBounds(140, area.getY() - 30, 120, 30);
    area.removeFromTop(2);
    playlist.setBounds(area.removeFromTop(150));

    area.removeFromTop(20);

    // --- 4) Sliders ---
    int labelWidth = 90;
    int sliderHeight = 30;
    int spaceBetweenRows = 10;

    auto row1 = area.removeFromTop(sliderHeight);
    volumeLabel.setBounds(row1.removeFromLeft(labelWidth));
    volumeSlider.setBounds(row1.removeFromLeft((getWidth() / 2) - labelWidth - 20));
    speedLabel.setBounds(row1.removeFromLeft(labelWidth));
    speedSlider.setBounds(row1);

    area.removeFromTop(spaceBetweenRows);

    auto row2 = area.removeFromTop(sliderHeight);
    loopStartLabel.setBounds(row2.removeFromLeft(labelWidth));
    loopStartSlider.setBounds(row2.removeFromLeft((getWidth() / 2) - labelWidth - 20));
    loopEndLabel.setBounds(row2.removeFromLeft(labelWidth));
    loopEndSlider.setBounds(row2);

    area.removeFromTop(spaceBetweenRows);

    positionLabel.setBounds(10, area.getY(), labelWidth, 20);
    positionSlider.setBounds(100, area.getY() - 5, getWidth() - 120, sliderHeight);
    area.removeFromTop(sliderHeight + 10);

    metadataLabel.setBounds(10, getHeight() - 40, getWidth() - 20, 30);
}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &playButton)
    {
        audioPlayer.start();
    }
    else if (button == &stopButton)
    {
        audioPlayer.stop();
    }
    else if (button == &goForwardButton)
    {
        audioPlayer.forward10Sec();
    }
    else if (button == &gobackButton)
    {
        audioPlayer.back10Sec();
    }
    else if (button == &loadPlaylistButton)
    {
        playlist.loadFiles();
    }
    else if (button == &playSelectedButton)
    {
        playlist.playSelectedTrack();
    }

    else if (button == &loadButton)
    {
        chooser.launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                juce::File file = fc.getResult();
                if (file.existsAsFile())
                {
                    thumbnail.clear();
                    thumbnail.setSource(new juce::FileInputSource(file));
                    totalLength = audioPlayer.getLengthInSeconds();
                    repaint(); 
                    audioPlayer.loadURL(juce::URL{ file });

                    if (audioPlayer.isLouded())
                    {
                        double length = audioPlayer.getLengthInSeconds();
                        positionSlider.setRange(0.0, length);
                        positionSlider.setValue(0.0);
                        loopStartSlider.setRange(0.0, length);
                        loopEndSlider.setRange(0.0, length);
                        loopEndSlider.setValue(length);


                        int totalSeconds = (int)length;
                        int minutes = totalSeconds / 60;
                        int seconds = totalSeconds % 60;
                        juce::String duration = juce::String::formatted("Duration: %02d:%02d", minutes, seconds);

                        juce::AudioFormatManager formatManager;
                        formatManager.registerBasicFormats();
                        std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));

                        juce::String info;

                        if (reader != nullptr && reader->metadataValues.size() > 0)
                        {
                            info = "Metadata:\n";
                            for (auto& key : reader->metadataValues.getAllKeys())
                                info += key + ": " + reader->metadataValues[key] + "\n";
                        }
                        else
                        {
                            info = "File: " + file.getFileName() + "\n";
                        }

                        info += duration;

                        metadataLabel.setText(info, juce::dontSendNotification);
                    }
                }
            });
    }

    else if (button == &muteButton)
    {
        static bool isMute = false;
        isMute = !isMute;
        audioPlayer.setMute(isMute);
        muteButton.setButtonText(isMute ? "Unmute" : "Mute");
    }
    else if (button == &goStartButton)
    {
        audioPlayer.goStart();
    }
    else if (button == &goEndButton)
    {
        audioPlayer.goEnd();
    }
    else if (button == &repeatingButton)
    {
        audioPlayer.repeat(repeatingButton.getToggleState());
        if (repeatingButton.getToggleState())
        {
            audioPlayer.setCustomLoopEnabled(false, 0, 0);
        }
    }
    else if (button == &setLoopPointsButton)
    {

        double start = loopStartSlider.getValue();
        double end = loopEndSlider.getValue();

        if (start < end)
        {
            audioPlayer.setCustomLoopEnabled(true, start, end);
            repeatingButton.setToggleState(false, juce::dontSendNotification);
            metadataLabel.setText("Custom Loop: " + juce::String(start, 1) + "s - " + juce::String(end, 1) + "s",
                juce::dontSendNotification);
        }
    }
    else if (button == &clearLoopPointsButton)
    {

        audioPlayer.setCustomLoopEnabled(false, 0, 0);
        repeatingButton.setToggleState(false, juce::dontSendNotification);
        metadataLabel.setText("Loop cleared", juce::dontSendNotification);
    }

}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
    {
        audioPlayer.setGain(slider->getValue());
    }
    else if (slider == &speedSlider)
    {
        audioPlayer.setSpeed(slider->getValue());
    }
    else if (slider == &positionSlider)
    {
        audioPlayer.setPosition(slider->getValue());
    }
    else if (slider == &loopStartSlider)
    {
        if (loopStartSlider.getValue() >= loopEndSlider.getValue())
        {
            loopEndSlider.setValue(loopStartSlider.getValue() + 1.0);
        }
    }
    else if (slider == &loopEndSlider)
    {
        if (loopEndSlider.getValue() <= loopStartSlider.getValue())
        {
            loopStartSlider.setValue(loopEndSlider.getValue() - 1.0);
        }
    }
}
void PlayerGUI::drawWaveform(juce::Graphics& g, juce::Rectangle<int> area)
{
    g.setColour(juce::Colours::darkgrey);
    g.fillRect(area);

    if (thumbnail.getNumChannels() == 0)
    {
        g.setColour(juce::Colours::white);
        g.drawFittedText("No waveform loaded", area, juce::Justification::centred, 1);
        return;
    }

    // 
    g.setColour(juce::Colours::skyblue);
    thumbnail.drawChannels(g, area, 0.0, totalLength, 1.0f);

    // 
    g.setColour(juce::Colours::red);
    int playheadX = area.getX() + (int)((currentPosition / totalLength) * area.getWidth());
    g.drawLine((float)playheadX, (float)area.getY(), (float)playheadX, (float)area.getBottom(), 2.0f);
}
void PlayerGUI::mouseDown(const juce::MouseEvent& event)
{
    auto area = getLocalBounds().reduced(10).removeFromTop(120);
    if (area.contains(event.getPosition()))
    {
        double clickPosition = (event.x - area.getX()) / (double)area.getWidth();
        audioPlayer.setPosition(clickPosition * totalLength);
    }
}




