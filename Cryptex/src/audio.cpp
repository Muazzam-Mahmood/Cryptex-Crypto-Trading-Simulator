#include "audio.h"
#include <QUrl>

IntroSound::IntroSound() {
    player = new QMediaPlayer;
    output = new QAudioOutput;
    player->setAudioOutput(output);
    output->setVolume(0.8f);
    player->setSource(QUrl::fromLocalFile("intro.mp3"));
}
void IntroSound::play() {
    
    player->play();
}
void IntroSound::stop() {
    player->stop();
}



ClickSound::ClickSound() {
    player = new QMediaPlayer;
    output = new QAudioOutput;
    player->setAudioOutput(output);
    output->setVolume(0.8f);
    player->setSource(QUrl::fromLocalFile("newClick2.wav"));
}
void ClickSound::play() {
    player->stop();
    player->play();
}
void ClickSound::stop() {
    player->stop();
}



GraphSound::GraphSound() {
    player = new QMediaPlayer;
    output = new QAudioOutput;
    player->setAudioOutput(output);
    output->setVolume(0.8f);
    player->setSource(QUrl::fromLocalFile("graph.mp3"));
}
void GraphSound::play() {
    player->stop();
    player->play();
}
void GraphSound::stop() {
    player->stop();
}



Transaction::Transaction() {
    player = new QMediaPlayer;
    output = new QAudioOutput;
    player->setAudioOutput(output);
    output->setVolume(0.8f);
    player->setSource(QUrl::fromLocalFile("Transaction.mp3"));
}
void Transaction::play() {

    player->play();
}
void Transaction::stop() {
    player->stop();
}
