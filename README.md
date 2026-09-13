# Snow Audio Player

A C++ desktop audio player application built using the **JUCE framework** as part of the **CS213 – Object-Oriented Programming** course at the Faculty of Computers and Artificial Intelligence, Cairo University.

The project was developed as a **team application**, with a focus on applying Object-Oriented Programming concepts to a real GUI-based application, handling audio playback, user interaction, file management, and audio processing.

---

## 🎯 Project Overview

Snow Audio Player allows users to load and control audio files through a graphical user interface.

The application supports common audio-player functionality such as:

* Loading audio files
* Playing and pausing tracks
* Stopping and restarting playback
* Volume control
* Mute / Unmute
* Playback speed control
* Seeking through the audio
* Continuous looping
* A–B segment looping
* Audio metadata display
* Playlist management
* Waveform visualization
* Mixing two audio tracks
* A redesigned graphical user interface

The project was also an opportunity to apply **OOP, event-driven programming, GUI development, file handling, and audio processing** using C++ and JUCE.

---

## ✨ Features

| #  | Feature                                    | Difficulty         | Implemented By |
| -- | ------------------------------------------ | ------------------ | -------------- |
| 1  | Load, restart, and stop audio              | Easy               | **Team Work**  |
| 2  | Pause / Play                               | Easy               | **Samir**      |
| 3  | Mute / Unmute                              | Easy               | **Abdalrhman** |
| 4  | Continuous looping (Repeat)                | Easy               | **Abdallah**   |
| 5  | Display metadata (title, author, duration) | Medium             | **Samir**      |
| 6  | Speed control (Playback Rate)              | Medium             | **Samir**      |
| 7  | Position slider (Seek / Time Display)      | Medium             | **Abdallah**   |
| 8  | Playlist support (Multiple Files)          | Hard               | **Samir**      |
| 9  | Progress bar / Waveform Display            | Hard               | **Samir**      |
| 10 | A–B Segment Looping                        | Hard               | **Abdallah**   |
| 11 | Player UI Redesign                         | Design             | **Abdallah**   |
| 12 | Mix Tracks                                 | Additional Feature | **Abdallah**   |

---

## 🎵 Additional Features

### Mix Tracks

An additional audio-processing feature was implemented to allow users to select **two audio tracks and mix them into a single audio file**.

The mixing functionality:

1. Allows the user to select two audio files.
2. Reads the audio data from both files.
3. Processes their audio samples.
4. Combines the samples into a single track.
5. Saves the resulting audio as a WAV file.
6. Loads the generated file into the player.
7. Adds the mixed track to the playlist.

This feature provided additional experience with **audio buffers, sample-level processing, file handling, and JUCE's audio APIs**.

### UI Redesign

The original player interface was redesigned to provide a more organized and user-friendly experience.

The redesigned interface brings together the main playback controls, audio information, waveform, playlist, and additional controls in a clearer layout.

---

## 🏗️ Project Architecture

The application is divided into multiple components, with each component responsible for a specific part of the application.

| Component           | Responsibility                                                              |
| ------------------- | --------------------------------------------------------------------------- |
| `MainComponent`     | Main application component and connection between the audio and GUI layers  |
| `PlayerAudio`       | Handles audio playback, audio sources, volume, speed, position, and looping |
| `PlayerGUI`         | Handles the graphical interface and user interaction                        |
| `PlaylistComponent` | Manages multiple audio files and playlist interaction                       |
| `MixWindow`         | Provides the interface and functionality for mixing two audio tracks        |

### Main Architecture

```text
                    MainComponent
                         │
              ┌──────────┴──────────┐
              │                     │
         PlayerGUI              PlayerAudio
              │                     │
       ┌──────┴──────┐        Audio Processing
       │             │
 PlaylistComponent  MixWindow
```

The application uses JUCE's audio and GUI abstractions to separate responsibilities between **audio processing, interface management, playlist handling, and additional functionality**.

---

## 🧩 Object-Oriented Programming

The project was developed to apply Object-Oriented Programming concepts in a real application.

Some of the concepts used include:

* Classes and Objects
* Inheritance
* Encapsulation
* Composition
* Separation of Responsibilities
* Event-driven programming
* Interface-based interaction through JUCE classes

Examples include:

* `PlayerAudio` inheriting from JUCE audio-related classes.
* `PlayerGUI` handling GUI components and user interactions.
* Separate components for playlist management and audio mixing.
* Composition between the main application component and its functional components.

---

## 🛠️ Technologies & Tools

* **C++**
* **JUCE Framework**
* **Object-Oriented Programming**
* **GUI Development**
* **Audio Processing**
* **Event-Driven Programming**
* **File I/O**
* **Git**
* **GitHub**
* **Visual Studio 2022**

---

## 👥 Team Contributions

This project was developed collaboratively as a university team project.

### Samir

Worked on:

* Pause / Play functionality
* Audio metadata display
* Playback speed control
* Playlist support
* Waveform / progress visualization

### Abdalrhman

Worked on:

* Mute / Unmute functionality

### Abdallah

Worked on:

* Continuous looping
* Position / seek slider
* A–B segment looping
* Player UI redesign
* Additional Mix Tracks functionality

### Team Work

The initial audio loading, restarting, and stopping functionality was developed through **teamwork**.

The project also involved collaboration through **GitHub** during development and integration.

---

## 📸 Screenshots

### Main Player

*Add a screenshot of the main player interface here.*

### Waveform & Playback Controls

*Add a screenshot showing the waveform, playback controls, and position slider here.*

### Playlist / Mix Window

*Add a screenshot showing the playlist or Mix Tracks interface here.*

---

## 🎥 Demo

A demonstration of the application is available here:

[Watch the Demo](https://drive.google.com/file/d/1_0i_y0IfCWUiU-UUHa18vu7zEgsk6-4v/view?usp=sharing)

---

## ⚙️ Setup & Running

### Requirements

* Visual Studio 2022
* C++ development environment
* JUCE Framework
* Projucer or the provided JUCE project configuration

### Steps

1. Clone the repository.
2. Open the project using the configured JUCE / Visual Studio setup.
3. Make sure the required JUCE modules are available.
4. Build the project using Visual Studio.
5. Run the application.
6. Load an audio file and start using the player.

---

## 🎓 Academic Context

**Course:** CS213 – Object-Oriented Programming
**Institution:** Faculty of Computers and Artificial Intelligence, Cairo University
**Project:** Assignment 2
**Language:** C++
**Framework:** JUCE

This project was developed as part of the practical application of Object-Oriented Programming concepts learned throughout the course.

---

## 📌 Notes

This is an **academic team project** developed for learning and demonstrating C++ and Object-Oriented Programming concepts.

The project is not intended to be a production-grade commercial audio application, but rather a practical implementation of a desktop application using **C++, JUCE, GUI programming, and audio processing**.
