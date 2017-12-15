#ifndef __VEDIO_H__
#define __VEDIO_H__

//音乐的播放，参数为要播放的文件名
extern void music_play(const char *pmusic_path);

//音乐暂停
extern void music_pause(void); 

//音乐继续
extern void music_continue(void);

//音乐的停止
extern void music_stop(void);

//视频播放，参数为要播放的文件名
extern void video_play(const char *video_path); 

//视频暂停
extern void vedio_pause(void);

//视频继续
extern void vedio_continue(void);

//视频的停止
extern void vedio_stop(void);	

#endif