#include <JuceHeader.h>
#include "MainComponent.h"

class JUCEAudioPlayerApplication : public juce::JUCEApplication
{
public:
    JUCEAudioPlayerApplication() {}

    const juce::String getApplicationName() override { return "JUCE Audio Player"; }
    const juce::String getApplicationVersion() override { return "1.0"; }

    void initialise(const juce::String&) override
    {
        mainWindow.reset(new MainWindow("JUCE Audio Player", new MainComponent(), *this));
    }

    void shutdown() override { mainWindow = nullptr; }

    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow(juce::String name, juce::Component* c, JUCEAudioPlayerApplication& a)
            : DocumentWindow(name,
                juce::Desktop::getInstance().getDefaultLookAndFeel()
                .findColour(juce::ResizableWindow::backgroundColourId),
                DocumentWindow::allButtons),
            app(a)
        {
            setUsingNativeTitleBar(true);
            setContentOwned(c, true);
            centreWithSize(700, 650);  // 
            setVisible(true);
        }

        void closeButtonPressed() override { app.systemRequestedQuit(); }

    private:
        JUCEAudioPlayerApplication& app;
    };

private:
    std::unique_ptr<MainWindow> mainWindow;
};

START_JUCE_APPLICATION(JUCEAudioPlayerApplication)
