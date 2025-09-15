#ifndef AUDIO_COMMANDS_H
#define AUDIO_COMMANDS_H


#include <QStringList>


class AudioCommands
{
    friend class AudioInterface;

    //list of ffmpeg audio transcode commands
    private:
        const QString copy_command = "copy";
        const QString audio_codec_flag = "-codec:a";
        const QString audio_bitrate_flag = "-b:a";
        const QString audio_samplerate_flag = "-ar";
        const QString audio_channels_flag = "-ac";

};

#endif // AUDIO_COMMANDS_H
