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
    connect(m_decoder, &QAudioDecoder::bufferReady, this, [this](){
        QAudioBuffer b = m_decoder->read();
        Q_UNUSED(b);
        emit bufferReady();
    });
    connect(m_decoder, QOverload<QAudioDecoder::Error>::of(&QAudioDecoder::error), this,
            [=](QAudioDecoder::Error error){
        Q_UNUSED(error);

        qDebug()<<"Decoder error:"<<m_decoder->errorString();

        // Close the progress message box
        if(m_progressBox){
            m_progressBox->close();
            delete m_progressBox;
            m_progressBox = nullptr;
        }

        // Emit the error signal
        emit decodeError(m_decoder->errorString());
    });

    // The private connections
    connect(m_decoder, SIGNAL(positionChanged(qint64)), this, SLOT(onDecoderPositionChange(qint64)));
    connect(m_decoder, SIGNAL(finished()), this, SLOT(onDecodeFinished()));
}

void AudioFile::onDecoderPositionChange(qint64 position){

    //qDebug() << "Decoder position changed:" << position;

    // Get the duration of the audio
    qint64 duration = m_decoder->duration();
    if(duration <= 0) return; // Prevent division by zero

    // Calculate the decoding progress
    int progress = static_cast<int>((static_cast<double>(position) / duration) * 100.0);

    // Update the progress message box
    if(m_progressBox) m_progressBox->setProgress(0, progress);

}

void AudioFile::onDecodeFinished(){
    qDebug() << "Audio decoding finished.";

    // Close the progress message box
    if(m_progressBox){
        m_progressBox->setProgress(0, 100);
        m_progressBox->close();
        delete m_progressBox;
        m_progressBox = nullptr;
    }

    emit decodeFinished();
}

bool AudioFile::isDecoding() {
    if(!m_decoder) return false;
    return m_decoder->isDecoding();
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
    qDebug() << "Starting audio decode for file:" << m_filePath;

    // Check if we have file data
    if (!m_fileData || m_fileData->isEmpty()) {
        qDebug() << "No file data to decode.";
        return false;
    }

    // Set up the decoder
    m_decoder->setSourceDevice(new QBuffer(m_fileData, this));

    // Set up the progress message box
    m_progressBox = new ProgressMessageBox("Decoding Audio...", {"Decoding audio file..."});

    // Start decoding
    m_decoder->start();

    // Execute the progres message box
    m_progressBox->exec();

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
