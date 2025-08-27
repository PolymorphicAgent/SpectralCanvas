#include "audiofile.h"

AudioFile::AudioFile(QObject *parent)
    : QObject{parent}, m_decoder(new QAudioDecoder(this))
{
    connectSignals();
}

// Copy Constructor
AudioFile::AudioFile(const AudioFile &other)
    : QObject(other.parent()),
      m_fileData(other.m_fileData),
      m_filePath(other.m_filePath),
      m_decoder(other.m_decoder)
{
    connectSignals();
}

// Initializer Constructor
AudioFile::AudioFile(QByteArray *data, QString filePath, QObject *parent)
    : QObject{parent},
      m_fileData(data),
      m_filePath(filePath),
      m_decoder(new QAudioDecoder(this))
{
    connectSignals();
}

void AudioFile::connectSignals(){
    connect(m_decoder, SIGNAL(bufferReady()), this, SIGNAL(bufferReady()));
    connect(m_decoder, SIGNAL(finished()), this, SIGNAL(decodeFinished()));
    connect(m_decoder, QOverload<QAudioDecoder::Error>::of(&QAudioDecoder::error), this,
            [=](QAudioDecoder::Error error){
        Q_UNUSED(error);
        emit decodeError(m_decoder->errorString());
    });
}

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
bool AudioFile::startDecode() {

    // Check if we have file data
    if (!m_fileData || m_fileData->isEmpty()) {
        qDebug() << "No file data to decode.";
        return false;
    }

    // Set up the decoder
    m_decoder->setSourceDevice(new QBuffer(m_fileData, this));
    m_decoder->start();

    return true;
}

bool AudioFile::bufferAvailable() {
    if(!m_decoder) return false;
    return m_decoder->bufferAvailable();
}

QAudioBuffer* AudioFile::readBuffer(){
    if(!m_decoder) return nullptr;
    return new QAudioBuffer(m_decoder->read());
}
