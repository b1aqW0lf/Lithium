#ifndef FFMPEG_COMMANDS_LIST_H
#define FFMPEG_COMMANDS_LIST_H

/***********************************************************************
** This is a list of ffmpeg commands to be used in the application
** ffmpeg sytntax:
** ffmpeg [global_options] {[input_file_options] -i ‘input_file1’} ...
**                        {[output_file_options] ‘output_file1’}
************************************************************************/
                  /* Command List */
                  /****************/

//1. Merge video and audio file - copy, no transcodinng
//video and audio files are separate
//ffmpeg -i input_video -i input_audio -c:v copy -c:a copy -map 0:0 -map 1:0
//                                                                 output_video
//2. Extract audio from video file
//input audio format MUST match output audio format for no transcoding copy
//ffmpeg -i input_video -vn -c:a codec_name output_audio
//
//3. Get the number of frames of a video and outputs to console - via ffmpeg
//ffmpeg -i input_video -f null /dev/null
//
//4. Get video fps - via ffprobe
//ffprobe -v error -select_streams v -of default=noprint_wrappers=1:nokey=1
//                              -show_entries stream=r_frame_rate input_file
//
//5. Version 1 of transcoding command
//args <<"-i"<< input_file1 << -c:v << "libx264" << "-preset" << "medium"
//<< "-crf" << "23" << -c:a << "aac" << "-b:a" << "128k" << output_file;
//
//6. Version 2 of transcoding command
//args <<"-i"<< input_file1 << output_file;
//
//7. Version 3 of transcoding command
//args << "-v" << warning << hide_banner << stats << "-i" << input_file1
//     << output_file;
//
//8. Check input stream with ffprobe
//ffprobe -i input_file -show_streams -select_streams a -loglevel error
//
//9. Get video resolution from input video stream using ffprobe
//ffprobe -v error -select_streams v:0 -show_entries stream=width,height -of
// csv=s=x:p=0 input_file1
//https://ottverse.com/change-resolution-resize-scale-video-using-ffmpeg/
//
//10. Get video codec from input video stream using ffprobe
//ffprobe -v error -select_streams v:0 -show_entries stream=codec_name -of
//default=noprint_wrappers=1:nokey=1 input_file
//
//11. Split the video without re-encoding it [use the copy codec for audio and video]
//ffmpeg -i input_file -ss 00:00:00 -t 00:50:00 -c copy output_file0 -ss 00:50:00
//-c copy output_file1 -ss 00:50:00 -c copy output_file2...
//or
//ffmpeg -i input_file -t 00:50:00 -c copy output_file1 -ss 00:50:00 -c copy output_file2
//-ss 00:50:00 -c copy output_file3..
//
//12. Merge/Concat media files made from exactly the same codec and codec parameters
//#1. Create a file file_name.txt with all the files you want to have concatenated in
//the following form (lines starting with a # are ignored):
//
//# this is a comment
//file '/path/to/file1.wav'
//file '/path/to/file2.wav'
//file '/path/to/file3.wav'...and so on if need be
//
//#2. Then you can stream copy or re-encode your files:
//ffmpeg -f concat -safe 0 -i file_name.txt -c copy output.wav
//The -safe 0 above is not required if the paths are relative. (I say always use it)
//please see: https://trac.ffmpeg.org/wiki/Concatenate for more
//Note: If output file name/path has spaces, wrap the path, including file_name in double-quotes
//
/*13. connect(ui->ffEncodeButton, SIGNAL(clicked()),
        this, SLOT(encoding_started()));*/
/*connect(ui->ffEncodeButton, SIGNAL(clicked()),
        this, SLOT(frame_time_parse()));*/
//
/*14. connect(ui->ffCancelButton, SIGNAL(clicked()),
        this, SLOT(encoding_cancelled()));*/
//
//15. Get the total number of frames of a video file using ffprobe
//ffprobe -v error -count_frames -select_streams v:0 -show_entries stream=nb_read_frames -of csv=p=0
//input_file
//
// 2-Pass Encoding : Works!
//16. ffmpeg -i /home/Wolf/Videos/Test.x265-BvS.mkv -sn -vcodec libx264 -pix_fmt yuv420p -b:v 10000k -preset slow -map_metadata 0 -y -passlogfile /home/Wolf/Downloads/passlogfile -pass 1 -an -f mp4 /dev/null &&
//    ffmpeg -i /home/Wolf/Videos/Test.x265-BvS.mkv -sn -vcodec libx264 -pix_fmt yuv420p -b:v 10000k -preset slow -map_metadata 0 -y -passlogfile /home/Wolf/Downloads/passlogfile -pass 2 -c:a copy -f mp4 /home/Wolf/Videos/Test.x264_1-BvS.mkv

#endif // FFMPEG_COMMANDS_LIST_H
