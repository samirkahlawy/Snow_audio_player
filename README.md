#  JUCE Audio Player

This project is a **C++ Audio Player Application** built using the **JUCE framework**, developed as part of the **CS213: Object-Oriented Programming Assignment 2** at **Cairo University, Faculty of Computing and Artificial Intelligence**.

The project demonstrates **modular design**, **object-oriented programming principles**, and **event-driven programming** using JUCE’s high-level abstractions for GUI and audio processing.

---

##  Overview

The JUCE Audio Player is capable of:

* Loading, playing, restarting, and stopping audio files.
* Handling both WAV and MP3 formats.
* Managing audio playback through modular components.
* Providing a user-friendly and extensible GUI.

As part of the assignment, the project will be **extended** with multiple new features such as pause, mute, looping, metadata display, and waveform visualization.
---

##  Project Architecture

To maintain modularity and separation of concerns, the application is divided into three main components:

| Component         | Responsibility                                                                                  |
| ----------------- | ----------------------------------------------------------------------------------------------- |
| **MainComponent** | The root JUCE component that displays the Player GUI.                                           |
| **PlayerGUI**     | Manages user interface elements (buttons, sliders, etc.) and sends user actions to PlayerAudio. |
| **PlayerAudio**   | Handles all audio logic: loading, playing, pausing, looping, and managing audio sources.        |

This structure follows **good OOP practices**:

* **Encapsulation:** Each class handles its own logic.
* **Separation of concerns:** GUI logic is separated from audio processing.
* **Modularity:** Easier to extend and maintain.

---

##  Features (Current & Planned)

| #  | Feature                                    | Difficulty | Status          |
| -- | ------------------------------------------ | ---------- | --------------- |
| 1  | Load, restart, and stop audio              | Easy       |  Implemented   |
| 2  | Pause / Play button                        | Easy       |  To Implement |
| 3  | Mute / Unmute toggle                       | Easy       |  To Implement |
| 4  | Continuous looping (repeat)                | Easy       |  To Implement |
| 5  | Display metadata (title, author, duration) | Medium     |  To Implement |
| 6  | Speed control (playback rate)              | Medium     |  To Implement |
| 7  | Position slider (seek / time display)      | Medium     |  To Implement |
| 8  | Playlist support (multiple files)          | Hard       |  To Implement |
| 9  | Progress bar / waveform display            | Hard       |  To Implement |
| 10 | A–B segment looping                        | Hard       |  To Implement |
| 11 | Redesign player UI (professional look)     | Design     |  To Implement |

---

##  Setup Instructions

### 1. Install Prerequisites

* **JUCE Framework:** [https://juce.com/download/](https://juce.com/download/)
* **CMake (optional but recommended):** [https://cmake.org/download/](https://cmake.org/download/)
* **Visual Studio 2022 (Community Edition)**

### 2. Create Project

Using **Projucer**:

1. Open **Projucer**.
2. Create a new **GUI Application**.
3. Enable the following JUCE modules:

   * `juce_audio_basics`
   * `juce_audio_formats`
   * `juce_audio_devices`
   * `juce_audio_utils`
   * `juce_gui_extra`
4. Export the project to **Visual Studio 2022**.
5. Click **Open in IDE**.

### 3. Build and Run

1. In Visual Studio, select the **Debug** configuration.
2. Click **Build → Build Solution**.
3. Run the project — a basic JUCE window with audio controls should appear.

---

##  Project Structure

```
JUCE-AudioPlayer/
│
├── Source/
│   ├── Main.cpp
│   ├── MainComponent.h / .cpp
│   ├── PlayerGUI.h / .cpp
│   ├── PlayerAudio.h / .cpp
│
├── Resources/
│   ├── icons/
│   ├── audio_samples/
│
├── Builds/
│   ├── VisualStudio2022/
│
└── README.md
```

---

##  Development Workflow (GitHub)

Each team member should work on their assigned features **in separate branches** and then merge changes to the `main` branch after testing.

| Member Role     | Tasks          |
| --------------- | -------------- |
| **Samir** | Tasks 2, 5, 8  |
| ** Abdo**   | Tasks 3, 6, 9  |
| **Abdallah**  | Tasks 4, 7, 10 |

### Git Workflow Example

```bash
git clone https://github.com/yourusername/JUCE-AudioPlayer.git
git checkout -b feature/pause-button
# make your changes
git add .
git commit -m "Added pause/play functionality"
git push origin feature/pause-button
# create pull request and merge
```

---

##  Example: Audio Flow in Code

```cpp
juce::AudioFormatManager formatManager;
formatManager.registerBasicFormats();

juce::File file("song.wav");
auto* reader = formatManager.createReaderFor(file);

std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
readerSource.reset(new juce::AudioFormatReaderSource(reader, true));

juce::AudioTransportSource transportSource;
transportSource.setSource(readerSource.get());
transportSource.start(); // begins playback
```

---

##  UI Design Guidelines

* Use **minimalistic colors** (dark mode recommended).
* Buttons should have clear icons (play, pause, stop, loop, mute).
* Add **sliders** for speed and position control.
* Display **metadata** and **progress visualization**.
* Ensure it’s **responsive and easy to use**.

---

##  Notes

* Always commit working versions only.
* Keep GUI and audio code **independent**.
* Use **OOP best practices** for clean and maintainable code.
* Use **Projucer** to manage modules and project settings.

---

##  License

This project is developed for educational purposes at **Cairo University, Faculty of Computing and AI** under the course **CS213: Object-Oriented Programming**.

