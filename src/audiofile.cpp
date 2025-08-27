#include "audiofile.h"

AudioFile::AudioFile(QObject *parent)
    : QObject{parent}, m_decoder(new QAudioDecoder(this))
{}

// Copy Constructor
AudioFile::AudioFile(const AudioFile &other)
    : QObject(other.parent()),
      m_fileData(other.m_fileData),
      m_filePath(other.m_filePath),
      m_decoder(other.m_decoder)
{}

// Initializer Constructor
AudioFile::AudioFile(QByteArray *data, QString filePath, QObject *parent)
    : QObject{parent},
      m_fileData(data),
      m_filePath(filePath),
      m_decoder(new QAudioDecoder(this))
{}

// Getters
QByteArray *AudioFile::fileData() {
    return m_fileData;
}

QString AudioFile::filePath() {
    return m_filePath;
}

// Setters
void AudioFile::setFileData(QByteArray* data) {
    m_fileData = data;
}

void AudioFile::setFilePath(QString file) {
    m_filePath = file;
}

// Other public functions
// Decodes the audio file using QAudioDecoder
bool AudioFile::decode() {

    // Check if we have file data
    if (!m_fileData || m_fileData->isEmpty()) {
        qDebug() << "No file data to decode.";
        return false;
    }

    // Set up the decoder
    m_decoder->setSourceDevice(new QBuffer(m_fileData, this));
    m_decoder->start();

}
