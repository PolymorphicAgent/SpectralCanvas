#ifndef AUDIOFILE_H
#define AUDIOFILE_H

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
    bool decode();

signals:

private:
    QByteArray *m_fileData;
    QString m_filePath;

    QAudioDecoder *m_decoder;
};

#endif // AUDIOFILE_H
