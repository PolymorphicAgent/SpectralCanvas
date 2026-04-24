#ifndef AUDIOFILE_H
#define AUDIOFILE_H

#include "progressmessagebox.h"

#include <QAudioDecoder>
#include <QObject>
#include <QBuffer>
#include <QDebug>

class AudioFile : public QObject
{
    Q_OBJECT
public:

    explicit AudioFile(QObject *parent = nullptr);

    // Copy Constructor
    explicit AudioFile(const AudioFile &other);

    // Initializer Constructor
    explicit AudioFile(QString filePath, QAudioFormat fmt, QObject *parent = nullptr);

    // Getters
    QString filePath();

    // Setters
    void setFilePath(QString);

    // Other public functions

    // Decodes the audio file using QAudioDecoder
    bool startDecode();

    // Checks if a buffer is available to be read from the decoder
    bool bufferAvailable();

    // Checks if the decoder is currently decoding
    bool isDecoding();

    // Reads the audio buffer from the decoder. You should either respond to the bufferReady() signal or check the bufferAvailable() function before calling.
    QAudioBuffer* readBuffer();

signals:
    void decodeError(QString error);
    void bufferReady();
    void decodeFinished();

private:
    // Should be called in each constructor
    void connectSignals();

    QVector<long double> m_processedAudio;
    QString m_filePath;
    QByteArray *m_fileData;

    QAudioDecoder *m_decoder;

    ProgressMessageBox *m_progressBox;

    // This member describes the audio format
    QAudioFormat m_audioFormat;

private slots:
    void onDecoderPositionChange(qint64 position);
    void onDecodeFinished();
    void onBufferReady();
};

#endif // AUDIOFILE_H
