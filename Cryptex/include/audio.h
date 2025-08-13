#pragma once

#include <QMediaPlayer>
#include <QAudioOutput>

class IntroSound {
public:
    IntroSound();
    void play();
    void stop();
private:
    QMediaPlayer* player;
    QAudioOutput* output;
};

class ClickSound {
public:
    ClickSound();
    void play();
    void stop();
private:
    QMediaPlayer* player;
    QAudioOutput* output;
};

class GraphSound {
public:
    GraphSound();
    void play();
    void stop();
private:
    QMediaPlayer* player;
    QAudioOutput* output;
};



class Transaction {
public:
    Transaction();
    void play();
    void stop();
private:
    QMediaPlayer* player;
    QAudioOutput* output;
};
