#include "PlayerGUI.h"

PlayerGUI::PlayerGUI(PlayerAudio& player)
    : audioPlayer(player), playlist(player)

{

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
    addAndMakeVisible(playlist);
    addAndMakeVisible(loadPlaylistButton);
    addAndMakeVisible(playSelectedButton);

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
	loopEndSlider.setRange(0.0,10.0);

    // start timer (interval in ms)
    startTimer(50); // كل 50 ms -> تحديث سلس كفاية
}

PlayerGUI::~PlayerGUI()
{
    // إزالة الـ listeners (حُسن نية، لتفادي dangling listeners)
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
    g.fillAll(juce::Colours::darkgrey);

    // رسم الوقت
    auto bounds = getLocalBounds();
    auto sliderArea = positionSlider.getBounds();

    g.setColour(juce::Colours::whitesmoke);
    g.setFont(14.0f);
}



void PlayerGUI::resized()
{
    auto area = getLocalBounds().reduced(20);

    // ---- Buttons row ----
    int buttonHeight = 36;
    int buttonWidth = 90;
    int gap = 8;

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
	goForwardButton.setBounds(buttonArea.removeFromLeft(buttonWidth));
	buttonArea.removeFromLeft(gap);
	gobackButton.setBounds(buttonArea.removeFromLeft(buttonWidth));
	buttonArea.removeFromLeft(gap);
    repeatingButton.setBounds(buttonArea.removeFromLeft(buttonWidth));
    buttonArea.removeFromLeft(gap);
    setLoopPointsButton.setBounds(buttonArea.removeFromLeft(buttonWidth));
    buttonArea.removeFromLeft(gap);
	clearLoopPointsButton.setBounds(buttonArea.removeFromLeft(buttonWidth));
    
	area.removeFromTop(20);
    loadPlaylistButton.setBounds(20, area.getY(), 120, 30);
    playSelectedButton.setBounds(160, area.getY(), 120, 30);
    area.removeFromTop(40);
    playlist.setBounds(area.removeFromTop(200));

    // أهو هنا بنبعد المنطقة اللي تحت الأزرار شوية
    area.removeFromTop(50); // ← زود الرقم لو عايز مسافة أكبر (مثلاً 80)

    // ---- Sliders + labels ----
    int labelWidth = 80;
    int sliderHeight = 36;

    // Volume
    volumeLabel.setBounds(10, area.getY(), labelWidth, 20);
    volumeSlider.setBounds(100, area.getY() - 10, getWidth() - 120, sliderHeight);
    area.removeFromTop(sliderHeight + 30);

    // Speed
    speedLabel.setBounds(10, area.getY(), labelWidth, 20);
    speedSlider.setBounds(100, area.getY() - 10, getWidth() - 120, sliderHeight);
    area.removeFromTop(sliderHeight + 30);

    // Position
    positionLabel.setBounds(10, area.getY(), labelWidth, 20);
    positionSlider.setBounds(100, area.getY() - 10, getWidth() - 120, sliderHeight);
    area.removeFromTop(sliderHeight + 30);

	//customing loop 
	loopStartLabel.setBounds(10, area.getY(), labelWidth, 20);
	loopStartSlider.setBounds(100, area.getY() - 10, getWidth() - 120, sliderHeight);
    area.removeFromTop(sliderHeight + 30);

	loopEndLabel.setBounds(10, area.getY(), labelWidth, 20);
	loopEndSlider.setBounds(100, area.getY() - 10, getWidth() - 120, sliderHeight);
	area.removeFromTop(sliderHeight + 30);


    // Metadata label at bottom
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
                    audioPlayer.loadURL(juce::URL{ file });

                    if (audioPlayer.isLouded())
                    {
                        // تحديث السلايدر بناءً على مدة الملف
                        double length = audioPlayer.getLengthInSeconds();
                        positionSlider.setRange(0.0, length);
                        positionSlider.setValue(0.0);
                        loopStartSlider.setRange(0.0, length);
                        loopEndSlider.setRange(0.0, length);
                        loopEndSlider.setValue(length);


                        // حساب مدة الملف
                        int totalSeconds = (int)length;
                        int minutes = totalSeconds / 60;
                        int seconds = totalSeconds % 60;
                        juce::String duration = juce::String::formatted("Duration: %02d:%02d", minutes, seconds);

                        // محاولة قراءة metadata
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

                        // دمج المعلومات مع المدة
                        info += duration;

                        // عرضها على اللابل
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
            audioPlayer.setCustomLoopEnabled(false,0,0);
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
        
        audioPlayer.setCustomLoopEnabled(false,0,0);
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

