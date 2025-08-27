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
    explicit AudioFile(QByteArray *data, QString filePath, QObject *parent = nullptr);

    // Getters
    QByteArray *fileData();
    QString filePath();

    // Setters
    void setFileData(QByteArray*);
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

    QByteArray *m_fileData;
    QString m_filePath;

    QAudioDecoder *m_decoder;

    ProgressMessageBox *m_progressBox;

private slots:
    void onDecoderPositionChange(qint64 position);
    void onDecodeFinished();
};

#endif // AUDIOFILE_H
