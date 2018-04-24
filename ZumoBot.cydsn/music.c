/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

/* [] END OF FILE */

#include "Beep.h"
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include "music.h"
#define FREQ_A 440.0f
#define BASE_OCTAVE (-3)
#define MAX_PERIOD (327.68f / 1000)
#define PERIOD_PWM (65536)

void stop_music(){
    Buzzer_PWM_Stop();
}

void PlayBufferPDM(uint16* buffer, int size)
{
    SPIM_PDM_PutArray(buffer, size);
}

void PlayPDM(uint16* track, int size)
{
    SPIM_PDM_Enable();
    SPIM_PDM_Start();
    
    const int BUFFER_SIZE = 128;
    uint16 buffer[BUFFER_SIZE];
    
    for (int i = 0; i < size; i++){
        buffer[i%BUFFER_SIZE] = track[i];
        if (i % BUFFER_SIZE == BUFFER_SIZE - 1){
          PlayBufferPDM(buffer, BUFFER_SIZE);   
        }
    }
    
    
    SPIM_PDM_Stop();
}

void Beep16(uint32 length, uint16 pitch)
{
    uint16 cmp = pitch / 2;
    Buzzer_PWM_Start();
    Buzzer_PWM_WriteCompare(cmp);
    Buzzer_PWM_WritePeriod(pitch);
    CyDelay(length);
    Buzzer_PWM_Stop();
}

void Beep16L(uint32 length, uint16 pitch)
{
    uint16 cmp = pitch / 2;
    Buzzer_PWM_Start();
    Buzzer_PWM_WriteCompare(cmp);
    Buzzer_PWM_WritePeriod(pitch);
    CyDelay(length);
}

void Beep16B(uint32 length, uint16 pitch, uint16 base)
{
    const float unit_period = MAX_PERIOD / PERIOD_PWM;
    const float period_base = base * unit_period;
    const int   ns          = length / period_base;
    for (int i = 0; i < ns*2; i++){
        if (i % 2 == 0){
            Beep16(length, pitch);
        } else {
            CyDelay(period_base / 2);
        }
    }
}

bool parse_note(Note* note, char cnote, int base_duration){
    char c = cnote;
    
    if (isdigit(c)){
        //Convert the character to int and set the octave
        note->octave = c - '0';
    } else if (c == 'O') {
        note->duration = base_duration * 4;
    } else if (c == 'o') {
        note->duration = base_duration * 2;
    } else if (c == '.') {
        note->duration = (note->duration) * 1.5;
    } else if (c == '-') {
        note->duration = base_duration / 2;
    } else if (c == '=') {
        note->duration = base_duration / 4;
    } else if (c == 'L') {
        note->legato = true;
    } else if (c == ' ') {
        note->duration = base_duration;
        note->legato = false;
    } else {
        note->note = c;
        return true;
        //play_note(c, base, *octave, *length);   
    }
    return false;
}

void play_music_with_base(char* music, char* baseline, float base_duration){
    const float unit_duration = base_duration / 16;
    int i = 0, j = 0;
    bool endmusic = false;
    char c = music[0];
    char b = baseline[0];
    int n_c = 0, n_b = 0;
    Note note, base;
    Note tmp_note, tmp_base;
    
    note.octave = 0;
    note.duration = base_duration;
    note.legato = false;
    
    base.note = '\0';
    base.octave = 0;
    base.duration = base_duration;
    base.legato = false;
    
    while (!endmusic){
        
           //If we successfully parsed a note
            if (c != '\0' && n_c == 0 && parse_note(&note, c, base_duration)){
                //Play it
                n_c = ((note.duration / unit_duration)/2);
                //Override duration
                tmp_note.octave = note.octave;
                tmp_note.duration = unit_duration;
                tmp_note.legato = false;
                tmp_note.note = note.note;
            }
            
            //If we successfully parsed a note
            if (b != '\0' && n_b == 0 && parse_note(&base, b, base_duration)){
                //Play it
                n_b = ((base.duration / unit_duration)/2);
                //Override duration
                tmp_base.octave = base.octave;
                tmp_base.duration = unit_duration;
                tmp_base.legato = false;
                tmp_base.note = base.note;
            }
        
        /*if (n_c >= 2 && n_b == 0){
            tmp_note.legato = true;
            play_note(tmp_note);
            tmp_note.legato = false;
            play_note(tmp_note);
            n_c = n_c - 2;
        } else if (n_b >= 2 && n_c == 0){
            tmp_base.legato = true;
            play_note(tmp_base);
            tmp_base.legato = false;
            play_note(tmp_base);
            n_b = n_b - 2;
        } else */ 
            
        while (n_b != 0 && n_c != 0){
            tmp_note.legato = true;
            play_note(tmp_note);
            tmp_base.legato = true;
            play_note(tmp_base);
            n_b--; n_c--;
        }
        
        while (c == '\0' && n_c == 0 && n_b != 0){
            tmp_base.legato = true;
            play_note(tmp_base);   
            n_b--;
        }
        
        while (b == '\0' && n_b == 0 && n_c != 0){
            tmp_note.legato = true;
            play_note(tmp_note); 
            n_c--;
        }
        /*    
        if (n_b > 1)
            {
                play_note(tmp_note);
                n_b--;
            }
        if (n_b >= 1 && n_c >= 1){
            tmp_note.legato = false;
            play_note(tmp_note);
            tmp_note.legato = false;
            play_note(tmp_base);
            n_c = n_c - 1;
            n_b = n_b - 1;
        } else if (n_c == 1 && n_b == 0){
            tmp_note.duration = unit_duration * 2;
            play_note(tmp_note);
            n_c = n_c - 1;
        } else if (n_b == 1 && n_c == 0){
            tmp_base.duration = unit_duration * 2;
            play_note(tmp_base);
            n_b = n_b - 1;
        }*/
        
        if (c != '\0' && n_c == 0){
            i++;
            c = music[i];
        }
        
        if (b != '\0' && n_b == 0){
            j++;
            b = baseline[j];
        }
        
        endmusic = c == '\0' && b == '\0' && n_c < 1 && n_b < 1;

    }
    
    stop_music();
    
}

void play_music(char* music, float base_duration){
    int i = 0;
    char c = music[0];
    Note note;
    
    note.legato = false;
    note.octave = 0;
    note.duration = base_duration;
    
    while (c != '\0'){
        //If we successfully parsed a note
        if (parse_note(&note, c, base_duration)){
            //Play it
            play_note(note);
        }
        c = music[i];
        i++;
    }
    
    stop_music();
    
}

int note_index(char note){
        int n = 0;
    
    switch(note){
      case 'a':
        n = -1;
        break;
      case 'A':
        n = 0;
        break;
      case 'b':
        n = 1;
        break;
      case 'B':
        n = 2;
        break;
      case 'C':
        n = 3;
        break;
      case 'd':
        n = 4;
        break;
      case 'D':
        n = 5;
        break;
      case 'e':
        n = 6;
        break;
      case 'E':
        n = 7;
        break;
      case 'F':
        n = 8;
        break;
      case 'g':
        n = 9;
        break;
      case 'G':
        n = 10;
        break;     
        
    }
    
    return n;
}

void play_note(Note note){
    int n = note_index(note.note);
    
    if (note.note == 'S'){
        //Silence
        CyDelay(note.duration);
        return;
    }
    
    const float freq = FREQ_A * powf(2, BASE_OCTAVE + note.octave + (float)n / 12);
    
    const float period = 1 / freq;
    const float unit_period = MAX_PERIOD / PERIOD_PWM;
    
    const float pwm = period / unit_period;
    
    
    if (pwm < 1.0f || pwm >= PERIOD_PWM){
     printf("Can not play this note. \n");   
    }
    
    if (note.legato)
        Beep16L(note.duration, (uint16) pwm);
    else
        Beep16(note.duration, (uint16) pwm);

}
/*
void play_note_and_base(Note note, Note base){
   
    //a A b B C d D e E F g G
    int n = note_index(note.note);
    int b = note_index(base.note);
    
    const float freq = FREQ_A * powf(2, BASE_OCTAVE + note.octave + (float)n / 12);
    
    const float period = 1 / freq;
    const float unit_period = MAX_PERIOD / PERIOD_PWM;
    
    const float pwm = period / unit_period;
    
    const float freq_base = FREQ_A * powf(2, BASE_OCTAVE + base.octave + (float)n / 12);
    const float period_base = 1 / freq_base;
    const float pwm_base = period_base / unit_period;
    
    if (pwm < 1.0f || pwm >= PERIOD_PWM){
     printf("Can not play this note. \n");   
    }
    
    if (base.note == '\0')
        Beep16(note.duration, (uint8) pwm);
    else
        Beep16B(note.duration, (uint8) pwm, (uint8) pwm_base);
}*/