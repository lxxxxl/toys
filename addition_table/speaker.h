#ifndef SPEAKER_H
#define SPEAKER_H

#include <QTextToSpeech>
#include <QTimer>

class Speaker : public QTextToSpeech
{
    Q_OBJECT
public:
    enum SpeakerState {
        Idle,
        SpeakingFirst,
        SpeakingSecond
    };

    Speaker();
    // Speaks two sentences with delay provided
    void SpeakWithDelay(QString firstSentence, QString secondSentence, int delay);

    // current speech state
    SpeakerState SpeakerState;
    // sentences to speak
    QString FirstSentence;
    QString SecondSentence;
    // delay between sentences in ms
    int Delay;
    // timer to implement delay
    QTimer Timer;

public slots:
    // process QTextToSpeech state changes
    void StateChanged(QTextToSpeech::State state);
    // process QTimer timeout
    void SpeakSecondSentence();

signals:
    // emits on speak done
    void SpeakDone();
    // emits on speech syntheiser errors
    void SpeakError();
};


#endif // SPEAKER_H
