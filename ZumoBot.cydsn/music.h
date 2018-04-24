/* ========================================
 *void PlayPDM(uint16* track, int size);
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#ifndef MUSIC_H
#define MUSIC_H
    
#include <stdbool.h>
#include <stdint.h>
#include <project.h>
    
typedef struct note {
    char note;
    int octave;
    int duration;
    bool legato;
} Note;
    
void play_note(Note note);
//void play_note_and_base(Note note, Note base);
void PlayBufferPDM(uint16* buffer, int size);
void PlayPDM(uint16* track, int size);

void play_music(char* music, float base_duration);
void play_music_with_base(char* music, char* baseline, float base_duration);
#endif
/* [] END OF FILE */
