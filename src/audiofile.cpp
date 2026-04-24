#include "audiofile.h"

#include <QFile>
#include <QMessageBox>

AudioFile::AudioFile(QObject *parent)
    : QObject{parent}, m_decoder(new QAudioDecoder(this))
{
    connectSignals();
}

// Copy Constructor
AudioFile::AudioFile(const AudioFile &other)
    : QObject(other.parent()),
      m_filePath(other.m_filePath),
      m_fileData(other.m_fileData),
      m_decoder(other.m_decoder),
      m_audioFormat(other.m_audioFormat)
{
    connectSignals();
}

// Initializer Constructor
AudioFile::AudioFile(QString filePath, QAudioFormat fmt, QObject *parent)
    : QObject{parent},
      m_filePath(filePath),
      m_decoder(new QAudioDecoder(this)),
      m_audioFormat(fmt)
{
    connectSignals();

    //load the file
    QFile file = QFile(filePath);
    file.open(QIODevice::ReadOnly);
    if(!file.isOpen()){
        qDebug() << "Unable to open file:" << filePath;
        QMessageBox::critical(nullptr, "Error", "Unable to open file!");
        return;
    }
    m_fileData = new QByteArray(file.readAll());
    file.close();
}

void AudioFile::connectSignals(){
    connect(m_decoder, &QAudioDecoder::bufferReady, this, &AudioFile::onBufferReady);
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

void AudioFile::onBufferReady(){

    // Read the buffer
    QAudioBuffer *buffer = readBuffer();

    // Get information about the buffer
    QAudioFormat format = buffer->format();
    int channels = format.channelCount();
    int frames = buffer->frameCount();
    int bytesPerSample = format.bytesPerFrame() / channels;

    // Append buffer data to m_processedAudio
    const quint8 *raw = reinterpret_cast<const quint8 *>(buffer->constData<void>());

    switch (format.sampleFormat()) {
        case QAudioFormat::Unknown: // If the audio is in an unknown format, we can't process it
            qDebug() << "Unknown audio format.";
            break;
        case QAudioFormat::UInt8: //If the audio is in an unsigned 8-bit format
            for (int i = 0; i < frames * channels; ++i) {
                quint8 sample = raw[i * bytesPerSample];
                m_processedAudio.append((static_cast<long double>(sample) - 128.0) / 128.0); // Normalize to [-1, 1]
            }
            break;
        case QAudioFormat::Int16: //If the audio is in a signed 16-bit format
            for (int i = 0; i < frames * channels; ++i) {
                qint16 sample = *reinterpret_cast<const qint16 *>(raw + i * bytesPerSample);
                m_processedAudio.append(static_cast<long double>(sample) / 32768.0); // Normalize to [-1, 1]
            }
            break;
        case QAudioFormat::Int32: //If the audio is in a signed 32-bit format
            for (int i = 0; i < frames * channels; ++i) {
                qint32 sample = *reinterpret_cast<const qint32 *>(raw + i * bytesPerSample);
                m_processedAudio.append(static_cast<long double>(sample) / 2147483648.0); // Normalize to [-1, 1]
            }
            break;
        case QAudioFormat::Float: //If the audio is in a floating-point format
            for (int i = 0; i < frames * channels; ++i) {
                float sample = *reinterpret_cast<const float *>(raw + i * bytesPerSample);
                m_processedAudio.append(static_cast<long double>(sample)); // Already in [-1, 1]
            }
            break;
        case QAudioFormat::NSampleFormats: // Not a valid format
            qDebug() << "Invalid audio format.";
            break;
        default:
            qDebug() << "Unhandled audio format.";
            break;

    }

    emit bufferReady();
}

// Getters
QString AudioFile::filePath() {
    return m_filePath;
}

// Setters
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
    m_decoder->setAudioFormat(m_audioFormat);
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

bool AudioFile::isDecoding() {
    if(!m_decoder) return false;
    return m_decoder->isDecoding();
}

QAudioBuffer* AudioFile::readBuffer(){
    if(!m_decoder) return nullptr;
    return new QAudioBuffer(m_decoder->read());
}
