#ifndef AUDIO_INTERFACE_DATA_H
#define AUDIO_INTERFACE_DATA_H


#include <QStringList>


class AudioInterfaceData
{
    friend class AudioInterface;
    //list of the audio interface's stringlists - to be used with their respective comboboxes
    private:
        /*!
         * \brief audioCodecList is to be used with audioCodecBox
         */
        //list of available audio codecs
        const QStringList audioCodecList = QStringList() << "Source" << "AAC" << "AC3" << "AC3_Fixed" << "EAC3" << "FLAC" <<"MP3"
                                                   << "WAV" << "Vorbis" << "Opus" << "PCM_s16le" << "PCM_s24le"
                                                   << "PCM_s32le" << "PCM_s16be" << "PCM_s24be" << "PCM_s32be"
                                                   << "PCM_alaw" << "PCM_mulaw" << "PCM_u8";

        /*!
        * \brief audioBitrateList is to be used with audioBitrateBox
        */
        //list of audio bitrate values
        const QStringList audioBitrateList = QStringList() << "Source" << "64" << "80" << "96" << "112" << "128" << "160" <<"192"
                                                     << "224" << "256" << "320" << "384" << "448" << "512"
                                                     << "576" << "640" << "1536";

        /*!
         * \brief audioSamplerateList is to be used with audioSamplerateBox
        */
        //list of audio samplerate values
        const QStringList audioSamplerateList = QStringList() << "Source" << "8000" << "11025" << "12000" << "16000" << "22050"
                                                        << "24000" << "32000" << "44100" << "48000";

        /*!
        * \brief audioChannelList is to be used with audioChannelBox
        */
        //list of audio channels
        const QStringList audioChannelList = QStringList() << "Source" << "Mono" << "Stereo" << "3" << "4" << "5" << "6" << "7"
                                                     << "8";

        /*!
        * \brief audioContainerList is to be used with audioContainerBox
        */
        //list of available audio containers/extensions
        const QStringList audioContainerList = QStringList() << "M4A" << "FLAC" << "MP3" << "WAV" << "OGG" << "OGA" << "AIFF" << "PCM";
};

#endif // AUDIO_INTERFACE_DATA_H
