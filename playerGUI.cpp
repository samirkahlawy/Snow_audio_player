#include"playerGUI.h"

PlayerGUI::PlayerGUI(PlayerAudio & player)
    : audioPlayer(player), playlist(player)
{
    formatManager.registerBasicFormats();

    setInterceptsMouseClicks(true, false);

    metadataLabel.setText("No file loaded", juce::dontSendNotification);
    metadataLabel.setJustificationType(juce::Justification::centred);
    metadataLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(metadataLabel);

    artistLabel.setText("Artist: Unknown", juce::dontSendNotification);
    artistLabel.setJustificationType(juce::Justification::centred);
    artistLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(artistLabel);


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
    addAndMakeVisible(mixButton);


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
    mixButton.addListener(this);

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
    startTimer(10);
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
    mixButton.removeListener(this);

    volumeSlider.removeListener(this);
    speedSlider.removeListener(this);
    positionSlider.removeListener(this);
    loopStartSlider.removeListener(this);
    loopEndSlider.removeListener(this);
}

void PlayerGUI::timerCallback()
{
    static bool wasLoaded = false;
    bool isNowLoaded = audioPlayer.isLouded();

    if (isNowLoaded && !wasLoaded)
    {
        double length = audioPlayer.getLengthInSeconds();
        positionSlider.setRange(0.0, length);
        loopStartSlider.setRange(0.0, length);
        loopEndSlider.setRange(0.0, length);
        loopEndSlider.setValue(length);

    }

    wasLoaded = isNowLoaded;

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
        &goStartButton, &goEndButton,  &gobackButton,& goForwardButton,
        &repeatingButton,&setLoopPointsButton,&clearLoopPointsButton ,&mixButton
		
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

    area.removeFromTop(spaceBetweenRows);//audioPlayer.loadURL

    positionLabel.setBounds(10, area.getY(), labelWidth, 20);
    positionSlider.setBounds(100, area.getY() - 5, getWidth() - 120, sliderHeight);
    area.removeFromTop(sliderHeight + 10);

    int labelHeight = 30;
    int spacing = 5;

    metadataLabel.setBounds(10, getHeight() - (labelHeight * 2 + spacing + 10), getWidth() - 20, labelHeight);
    artistLabel.setBounds(10, metadataLabel.getBottom() + spacing, getWidth() - 20, labelHeight);

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

        juce::Timer::callAfterDelay(200, [this]()
            {
                if (audioPlayer.isLouded())
                {
                    double length = audioPlayer.getLengthInSeconds();
                    positionSlider.setRange(0.0, length);
                    positionSlider.setValue(0.0);
                    loopStartSlider.setRange(0.0, length);
                    loopEndSlider.setRange(0.0, length);
                    loopEndSlider.setValue(length);

                    DBG("Sliders updated for playlist track");

                    juce::File selectedFile = playlist.getSelectedFile();
                    if (selectedFile.existsAsFile())
                    {
                        thumbnail.clear();
                        thumbnail.setSource(new juce::FileInputSource(selectedFile));
                        totalLength = length;
                        repaint();

                        juce::String fileName = selectedFile.getFileName();
                        metadataLabel.setText("Loaded from playlist: " + fileName, juce::dontSendNotification);

                        DBG("Waveform loaded for: " + fileName);
                    }
                }
            });
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

                    artistLabel.setText("Artist: " + audioPlayer.getArtist(), juce::dontSendNotification);


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
    else if (button == &mixButton)
    {
        mixWindow = std::make_unique<MixWindow>(audioPlayer, playlist);
        mixWindow->setSize(400, 300);
        mixWindow->setCentrePosition(getWidth() / 2, getHeight() / 2);
        mixWindow->addToDesktop(0);
        mixWindow->setVisible(true);
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

    g.setColour(juce::Colours::skyblue);
    thumbnail.drawChannels(g, area, 0.0, totalLength, 1.0f);

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


juce::File PlayerGUI::getCurrentPlayingFile()
{
    juce::File playlistFile = playlist.getSelectedFile();
    if (playlistFile.existsAsFile())
    {
        return playlistFile;
    }
    return juce::File();
}

PlayerGUI::MixWindow::MixWindow(PlayerAudio& player, PlaylistComponent& playlistComp)
    : audioPlayer(player), playlist(playlistComp)
{
    formatManager.registerBasicFormats();
    track1Label.setText("Track 1: No file selected", juce::dontSendNotification);
    track2Label.setText("Track 2: No file selected", juce::dontSendNotification);
    infoLabel.setText("Select two tracks to mix", juce::dontSendNotification);

    for (auto* label : { &track1Label, &track2Label, &infoLabel })
    {
        label->setJustificationType(juce::Justification::centredLeft);
        addAndMakeVisible(*label);
    }

    for (auto* button : { &browseButton1, &browseButton2, &mixButton, &cancelButton })
    {
        addAndMakeVisible(*button);
        button->addListener(this);
    }

    setSize(400, 300);
}

void PlayerGUI::MixWindow::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkslategrey);
    g.setColour(juce::Colours::white);
    g.setFont(16.0f);
    g.drawText("Mix Two Tracks", getLocalBounds().removeFromTop(40), juce::Justification::centred);
}

void PlayerGUI::MixWindow::resized()
{
    auto area = getLocalBounds().reduced(20);
    area.removeFromTop(40);

    int buttonHeight = 30;
    int gap = 10;

    auto row1 = area.removeFromTop(buttonHeight);
    browseButton1.setBounds(row1.removeFromLeft(120));
    row1.removeFromLeft(10);
    track1Label.setBounds(row1);

    area.removeFromTop(gap);

    auto row2 = area.removeFromTop(buttonHeight);
    browseButton2.setBounds(row2.removeFromLeft(120));
    row2.removeFromLeft(10);
    track2Label.setBounds(row2);

    area.removeFromTop(20);

    infoLabel.setBounds(area.removeFromTop(60));

    area.removeFromTop(20);

    auto buttonRow = area.removeFromTop(buttonHeight);
    mixButton.setBounds(buttonRow.removeFromLeft(120));
    buttonRow.removeFromLeft(20);
    cancelButton.setBounds(buttonRow.removeFromLeft(120));
}

void PlayerGUI::MixWindow::buttonClicked(juce::Button* button)
{
    if (button == &browseButton1)
    {
        loudTrackForMixing(1);
    }
    else if (button == &browseButton2)
    {
        loudTrackForMixing(2);
    }
    else if (button == &mixButton)
    {
        if (track1File.existsAsFile() && track2File.existsAsFile())
        {
            performMixing();
        }
        else
        {
            infoLabel.setText("Please select both tracks first", juce::dontSendNotification);
        }
    }
    else if (button == &cancelButton)
    {
        setVisible(false);
    }
}

void PlayerGUI::MixWindow::loudTrackForMixing(int trackNumber)
{
    auto callback = [this, trackNumber](const juce::FileChooser& chooser)
        {
            auto result = chooser.getResult();
            if (result.existsAsFile())
            {
                if (trackNumber == 1)
                {
                    track1File = result;
                    track1Label.setText("Track 1: " + result.getFileName(), juce::dontSendNotification);
                }
                else
                {
                    track2File = result;
                    track2Label.setText("Track 2: " + result.getFileName(), juce::dontSendNotification);
                }

                repaint();
            }
        };

    fileChooser = std::make_unique<juce::FileChooser>("Select audio file",
        juce::File{},
        "*.wav;*.mp3;*.aiff");

    fileChooser->launchAsync(juce::FileBrowserComponent::openMode |
        juce::FileBrowserComponent::canSelectFiles,
        callback);
}

void PlayerGUI::MixWindow::performMixing()
{
    try
    {
        infoLabel.setText("Starting mix process...", juce::dontSendNotification);

        auto* reader1 = formatManager.createReaderFor(track1File);
        auto* reader2 = formatManager.createReaderFor(track2File);

        if (reader1 == nullptr || reader2 == nullptr)
        {
            infoLabel.setText("Error: Cannot read one of the files", juce::dontSendNotification);
            return;
        }

        auto minLength = juce::jmin(reader1->lengthInSamples, reader2->lengthInSamples);
        auto numChannels = juce::jmax(reader1->numChannels, reader2->numChannels);

        if (minLength == 0)
        {
            infoLabel.setText("Error: Files have no audio data", juce::dontSendNotification);
            delete reader1;
            delete reader2;
            return;
        }

        infoLabel.setText("Creating mix buffer...", juce::dontSendNotification);

        juce::AudioBuffer<float> mixBuffer(numChannels, minLength);

        juce::AudioBuffer<float> buffer1(reader1->numChannels, minLength);
        juce::AudioBuffer<float> buffer2(reader2->numChannels, minLength);

        reader1->read(&buffer1, 0, minLength, 0, true, true);
        reader2->read(&buffer2, 0, minLength, 0, true, true);

        infoLabel.setText("Mixing audio data...", juce::dontSendNotification);

        for (int channel = 0; channel < numChannels; ++channel)
        {
            auto channel1 = channel % reader1->numChannels;
            auto channel2 = channel % reader2->numChannels;

            auto* dest = mixBuffer.getWritePointer(channel);
            auto* src1 = buffer1.getReadPointer(channel1);
            auto* src2 = buffer2.getReadPointer(channel2);

            for (int i = 0; i < minLength; ++i)
            {
                dest[i] = (src1[i] + src2[i]) * 0.5f;
            }
        }

        infoLabel.setText("Saving mixed file...", juce::dontSendNotification);

        juce::File documentsDir = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory);
        juce::File outputFile = documentsDir.getChildFile("mixed_output.wav");

        int counter = 1;
        while (outputFile.exists())
        {
            outputFile = documentsDir.getChildFile("mixed_output_" + juce::String(counter) + ".wav");
            counter++;
        }

        juce::WavAudioFormat format;

        if (auto fileStream = std::unique_ptr<juce::FileOutputStream>(outputFile.createOutputStream()))
        {
            if (auto writer = std::unique_ptr<juce::AudioFormatWriter>(
                format.createWriterFor(fileStream.get(),
                    reader1->sampleRate,
                    numChannels,
                    16,
                    juce::StringPairArray(),
                    0)))
            {
                fileStream.release();

                writer->writeFromAudioSampleBuffer(mixBuffer, 0, mixBuffer.getNumSamples());

                audioPlayer.loadURL(juce::URL(outputFile));
                playlist.addFileToPlaylist(outputFile);

                infoLabel.setText("Success! Mixed file saved and added to playlist.", juce::dontSendNotification);

                juce::Timer::callAfterDelay(2000, [this]() {
                    setVisible(false);
                    });
            }
            else
            {
                infoLabel.setText("Error: Could not create writer", juce::dontSendNotification);
            }
        }
        else
        {
            infoLabel.setText("Error: Could not create file", juce::dontSendNotification);
        }

        delete reader1;
        delete reader2;
    }
    catch (const std::exception& e)
    {
        infoLabel.setText(juce::String("Error: ") + e.what(), juce::dontSendNotification);
    }
}
