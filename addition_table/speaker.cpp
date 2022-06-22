#include "speaker.h"

Speaker::Speaker()
{
    connect(this, &QTextToSpeech::stateChanged, this, &Speaker::StateChanged);
    this->SpeakerState = Speaker::Idle;
    this->Delay = 0;
    this->FirstSentence = "";
    this->SecondSentence ="";
    this->Timer.setSingleShot(true);
}
/*
 * Speak 2 sentences with specified delay
 * @param firstSentence first sentence to speak
 * @param secondSentence second sentence to speak
 * @param delay between sentences
 *
 **/
void Speaker::SpeakWithDelay(QString firstSentence, QString secondSentence, int delay)
{
    if (this->SpeakerState != Speaker::Idle)
        return;

    this->FirstSentence = firstSentence;
    this->SecondSentence = secondSentence;
    this->Delay = delay;

    this->SpeakerState = Speaker::SpeakingFirst;
    this->say(this->FirstSentence);
}

/*
 * Overrides QTextToSpeech StateChanged method
 * emits SpeakDone on end of speak or SpeakError on error
 * @param state new TTS state
 **/
void Speaker::StateChanged(QTextToSpeech::State state)
{
    if (state == QTextToSpeech::Speaking)
        return;
    else if (state == QTextToSpeech::Ready) {
        if (this->SpeakerState == Speaker::SpeakingFirst) {
            if (this->Timer.isActive())
                this->Timer.stop();

            this->SpeakerState = Speaker::SpeakingSecond;
            this->Timer.singleShot(this->Delay, this, & Speaker::SpeakSecondSentence);
        }

        else if (this->SpeakerState == Speaker::SpeakingSecond) {
            this->SpeakerState = Speaker::Idle;
            emit SpeakDone();
            return;
        }
    }
    else {
        if (this->Timer.isActive())
            this->Timer.stop();
        this->SpeakerState = Speaker::Idle;
        emit SpeakError();
    }
}

/*
 * Starts to speak second sentence
 **/
void Speaker::SpeakSecondSentence()
{
    this->say(this->SecondSentence);
}
