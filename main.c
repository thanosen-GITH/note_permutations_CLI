/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: thanoulijonroth
 *
 * Created on 19 November 2019, 00:20
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#if defined(__APPLE__)
#include <xlocale.h>
#endif
#define MAX_PATH 2048
#define MAX_LINE_LENGTH 128
#define NLIMIT 8

static int fact_cache[13] = {0};
static int fact_inited = 0;

static void init_factorials(void) {
    fact_cache[0] = 1;
    for (int i = 1; i <= 12; i++) fact_cache[i] = fact_cache[i - 1] * i;
    fact_inited = 1;
}

int factorial(int n);
int existsintable(char *item, char table[][3],int N);
void determine_subtable(char subtable[][3],int N,int howmanyfound,
        char alreadyfound[][3], char original[][3]);
void determine_position(char retposition[][3],int N, int serial, char original[][3]);
char determine_note(char *s,int octrange);
int determine_difference(char a[],char b[]);
int determine_octrange(char ar[][4],int d);
int determine_bytes(int n, int ch_mel);
int write_noteon(char note,FILE *fpt,int ch_mel);
int write_noteoff(char note,FILE *fpt,int isfirst, int ch_mel);
void write_header(FILE *fpt,int bytes,int ch_mel);
void write_outro(FILE *fpt,int N);
int write_midi_ch(char *path,char *textfile,int N);
int write_midi_mel(char *path,char *textfile,int N);

/*
 * 
 */
int main(int argc, char** argv) {
    printf("Welcome to combinations!\nPlease give number of notes\n>");
    int N;
    
    scanf("%d",&N);
    
    if(N<1||N>12) {
        printf("Invalid number of notes, must be from 1 to 12\n");
        return 0;
    }
    
    char notes[N][3];
    char tmp[N][3];
    char buffer[128] = "\0";
    int i,j;
    
    for(i=0;i<N;i++) strcpy(notes[i],"\0");
    
    
    printf("Please give notes, one by one (hit enter after each note)\n");
    
    for(i=0;i<N;i++) {
        printf(">");
        scanf("%2s",notes[i]); //printf("%d\n",determine_note(notes[i],4));
        if (strchr(notes[i], '/') || strchr(notes[i], '\\') || strchr(notes[i], '.')) {
            printf("Invalid Note %s! Exiting!\n",notes[i]);
            return -1;
        }
        
        strncpy(tmp[i],notes[i],2);
        if(tmp[i][1]<35||tmp[i][1]>103) tmp[i][1]=' ';
        if(!determine_note(tmp[i],4)) {
            printf("Invalid Note %s! Exiting!\n",tmp[i]);
            return -1;
        }
        for(j=0;j<i;j++) if(determine_note(tmp[j],4)==determine_note(tmp[i],4)) {
            printf("Please don't repeat same notes twice!\nTry another note");
            i--;
            break;
        }
    }


    
    init_factorials();
    int fc,F=factorial(N),FL=factorial(NLIMIT),F11=factorial(11);
    char allnotes[N*3];
    strcpy(allnotes,"\0");
    
    int files = F/FL;
    if(files==0) files=1;
    
    for(i=0;i<N;i++) strcat(allnotes,notes[i]);
    
    char *filenames[files];
    for(i=0;i<files;i++) {
        filenames[i] = malloc(sizeof(char)*64);
        if(!filenames[i]) {
            for(j=0;j<i;j++) free(filenames[j]);
            printf("Error allocating memory for filenames! Exiting\n");
            return -1;
        }
        strcpy(filenames[i],"\0");
    }
    
    for(i=0;i<files;i++) snprintf(filenames[i],64,"%s%05d.txt",allnotes,i);
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    i = mkdir(allnotes);
    #else
    i = mkdir(allnotes, 07777);
    #endif
    if(i!=0) {
        perror("Folder/File creation problem: ");
        struct stat dir;
        if(!stat(allnotes, &dir)) {
            printf("Should pre-existing files be replaced?(y/n)\n>");
            fgets(buffer,8,stdin);

            while (strcasecmp(buffer,"y\n")&&(strcasecmp(buffer,"n\n"))) {
                printf("Please input y or n\n>");
                fgets(buffer,8,stdin);
            }

            if(!strcasecmp(buffer,"n\n")) {
                printf("Canceling...\nBye!\n");
                return (EXIT_SUCCESS);
            }
        }
        else return -1;
        
        
    }
    printf("Would you like to generate MIDI File(s)?\n");
    printf("n/c/m/b (for no, chordal, melodic or both, respectively)\n>");
    fgets(buffer,8,stdin);
    while (strcasecmp(buffer,"n\n")&&(strcasecmp(buffer,"c\n"))
            &&(strcasecmp(buffer,"m\n"))&&(strcasecmp(buffer,"b\n"))) {
                printf("Please input n, c, m or b\n>");
                fgets(buffer,8,stdin);
            }
    
    int ch=0, mel=0;
    if(!strcasecmp(buffer,"b\n")) { ch=1; mel=1; } // both
    else if(!strcasecmp(buffer,"c\n")) ch=1; // only chords
    else if(!strcasecmp(buffer,"m\n")) mel=1; // only melodies
    else { ch=0; mel=0; }
    
    if (N>NLIMIT) printf("Number of notes is bigger than %d, so the material will"
                " be split in %d files of %d!=%d combinations\n",
                NLIMIT,files,NLIMIT,FL);
    
    
    FILE *f;
    fc = 0;
    for (i=0;i<F;i++) {
        if(i%FL==0) {
            //strcpy(buffer,filenames[fc]);
            snprintf(buffer,128,"%s/%s",allnotes,filenames[fc]);
            //printf("filename %s\n",buffer);
            f = fopen(buffer,"w+");
            if (!f) {
                perror(buffer);
                return -1;
            }
            setvbuf(f, NULL, _IOFBF, 1 << 20);
            //printf("press enter\n");
            //getchar();
            
        }
        char position[N][3];
        determine_position(position,N,i,notes);
        int len = snprintf(buffer, sizeof(buffer), "%09d: ", i);
        if (len < 0) len = 0;
        size_t rem = (len >= (int)sizeof(buffer)) ? 0 : sizeof(buffer) - (size_t)len;
        char *p = buffer + len;
        for (j = 0; j < N && rem > 1; j++) {
            char tmp[3] = { ' ', ' ', '\0' };
            size_t nlen = strlen(position[j]);
            if (nlen == 1) {
                tmp[2] = position[j][0];
            } else if (nlen >= 2) {
                tmp[1] = position[j][0];
                tmp[2] = position[j][1];
            }
            if (rem < 3) break;
            memcpy(p, tmp, 3);
            p += 3;
            rem -= 3;
        }
        if (rem > 1) {
            *p++ = '\n';
            *p = '\0';
        } else if (rem == 1) {
            *p = '\0';
        }
        fwrite(buffer, (size_t)(p - buffer), 1, f);
        
        if(i%FL==FL-1) { printf("new file\n");
            fclose(f); f = NULL;
            if(ch==1) write_midi_ch(allnotes,filenames[fc],N);
            if(mel==1) write_midi_mel(allnotes,filenames[fc],N);
            fc++;
        }
        
    }

    if(f) {
        fclose(f);
        //printf("%d: %s\n",fc,filenames[fc]);
        if(N<=NLIMIT) {
            if(ch==1) write_midi_ch(allnotes,filenames[fc],N); // no splitting
            if(mel==1) write_midi_mel(allnotes,filenames[fc],N);
        }
            
    }
    
    
    if(f) fclose(f);

    for(i=0;i<files;i++) if(filenames[i]) free(filenames[i]);
    
    return (EXIT_SUCCESS);
}

int factorial(int n) {
    if (!fact_inited) init_factorials();
    if (n < 0 || n > 12) return 0;
    return fact_cache[n];
}

int existsintable(char *item, char table[][3],int N) {
    if((item==NULL)||(table==NULL)) {
        printf("Cannot check in NULL items/tables\n");
        return -1;
    }
    
    if(N<1) {
        printf("Nonsensical dimension of table\n");
        return -1;
    }
    
    int i,j,ret=0;
    for(i=0;i<N;i++)
        if(!strcmp(item,table[i])) ret=1;
    
    return ret;
    
}

void determine_subtable(char subtable[][3],int N,int howmanyfound,
        char alreadyfound[][3], char original[][3]) {
    if(subtable==NULL) {
        printf("(determine_subtable): Cannot write on NULL subtable!\n");
        return;
    }
    
    if((N<1)||(N>12)) {
        printf("(determine_subtable): Invalid number of notes!\n");
        return;
    }
    
    if((howmanyfound<0)||(howmanyfound>N-1)) {
        printf("(determine_subtable): invalid number of found notes!\n");
        return;
    }
    
    if(alreadyfound==NULL) {
        printf("(determine_subtable): NULL found table!\n");
        return;
    }
    
    if(original==NULL) {
        printf("(determine_subtable): Generator string NULL!\n");
        return;
    }
    
    
    int i,j,k;
    
    
    //printf("\tAlready found %d: ",howmanyfound);
    //for(i=0;i<howmanyfound;i++) printf("%3s",alreadyfound[i]);
    //printf("\n");
    //getchar();
    
    if(howmanyfound==0) {
        for(i=0;i<N;i++) strcpy(subtable[i],original[i]);
        return;
    }
    
    for(i=0;i<N;i++) strcpy(subtable[i],"\0");
    
    j=0;
    
    while(strcmp(alreadyfound[howmanyfound-1],original[j])) j++;
    
    j=(j+1)%N; // next from last found
    
    int flag=0;
    i=0,k=0;
    
    
    
    for(i=0;i<N-howmanyfound;i++) {
        //printf("\t\tchecking if orig(%d) %s is there\n",j,original[j]);
        while ((flag=existsintable(original[j],alreadyfound,howmanyfound))) j=(j+1)%N;
        //printf("\t\tflag %d\n",flag);
        strcpy(subtable[i],original[j]);
        j=(j+1)%N;
    }
    
    if (howmanyfound==N-1) {
        for(i=0;i<N;i++) {
            k=0;
            for(j=0;j<howmanyfound;j++) {
                if(!strcmp(original[i],alreadyfound[j])) {
                    k=1;
                    break;
                }
            }
            if(k==0) {
                strcpy(subtable[0],original[i]);
                return;
            }
        }
            
    }
    
}

void determine_position(char retposition[][3],int N, int serial, char original[][3]) {
    if((N<1)||(N>12)) {
        printf("(determine_position): Invalid number of notes!\n");
        return;
    }
    
    if((serial<0)||(serial>factorial(N)-1)) {
        printf("(determine_position): Serial Number out of bounds!\n");
        return;
    }
    
    if(original==NULL) {
        printf("(determine_position): Generator string NULL!\n");
        return;
    }
    
    int POS[N];
    //char found[N][3];
    int i,j;
    char subtable[N][3];
    
    for(i=0;i<N;i++) {
        
        POS[i] = (serial%factorial(N-i))/factorial(N-i-1);
        
        determine_subtable(subtable,N,i,retposition,original);
        strcpy(retposition[i],subtable[POS[i]]);
        //printf("\n  serial number %d, vector position %d, position %d from subtable (%s)\n",
        //        serial,i,POS[i],subtable[POS[i]]);
       // printf("  Subtable: ");
        
        //for(j=0;j<N-i;j++) printf("%3s",subtable[j]);
        //printf("\n");
    }
    
    return;
}

char determine_note(char *s,int octrange) { //printf("name %s\n",s);
    char note;
    
    if(!strncasecmp(s,"c ",2) || !strncasecmp(s,"b#",2)) note = 0;
    else if(!strncasecmp(s,"c#",2)  || !strncasecmp(s,"db",2)) note = 1;
    else if(!strncasecmp(s,"d ",2)) note = 2;
    else if(!strncasecmp(s,"d#",2)  || !strncasecmp(s,"eb",2)) note = 3;
    else if(!strncasecmp(s,"e ",2)  || !strncasecmp(s,"fb",2)) note = 4;
    else if(!strncasecmp(s,"f ",2)  || !strncasecmp(s,"e#",2)) note = 5;
    else if(!strncasecmp(s,"f#",2)  || !strncasecmp(s,"gb",2)) note = 6;
    else if(!strncasecmp(s,"g ",2)) note = 7;
    else if(!strncasecmp(s,"g#",2)  || !strncasecmp(s,"ab",2)) note = 8;
    else if(!strncasecmp(s,"a ",2)) note = 9;
    else if(!strncasecmp(s,"a#",2)  || !strncasecmp(s,"bb",2)) note = 10;
    else if(!strncasecmp(s,"b ",2)  || !strncasecmp(s,"cb",2)) note = 11;
    else {
        printf("Unable to determine note\n");
        return 0;
    }
    //printf("nnumber %d\n",note); getchar();
    
    //printf("without octrange note is %x\n",note);
    
    if(octrange<=0) {
        printf("Invalid octave range\n");
        return 0;
    }
    else if(octrange<3) note+=0x3c; //+60
    else if(octrange<5) note+=0x30; //+48
    else if(octrange<6) note+=0x24; //+36
    else if(octrange<7) note+=0x18; //+24
    else if(octrange<9) note+=0x0c; //+12
    else ; // octrange 9-12 => +0
    
    
    //printf("with octrange note is %x\n",note);
    return note;
    
}

int determine_difference(char a[],char b[]) {
    char na = determine_note(a,1);
    char nb = determine_note(b,1);
    
    if((int)na-(int)nb>=0) return (int)na-(int)nb;
    else return 12+na-nb;
}

int determine_octrange(char ar[][4],int d) {
    int i,range=0;
    for(i=0;i<d-1;i++) {
        range+=determine_difference(ar[i+1],ar[i]); //printf("-- range %d/%d ",range,range/12+1);
    } //printf("\n");
    
    return range/12+1;
}

int determine_bytes(int n, int ch_mel) {
    
    if((ch_mel!=0)&&(ch_mel!=1)) {
        printf("Invalid chords or melodies argument to determine bytes!\n");
        return -1;
    }
    if((n<0)||(n>12)) {
        printf("Invalid number of notes for determining bytes (%d)\n",n);
        return -1;
    }
    
    int bytes;
    
    
    if(ch_mel) {
        if(n<NLIMIT) bytes = 18 +  9*n*factorial(n); // Track name + outro = 18
        // every note needs 9 bytes (note on, note off)
        else bytes = 18 +  9*n*factorial(NLIMIT);
    }
    else {
        if(n<NLIMIT) bytes = 16 + (8*n+1)*factorial(n); // Track name + outro = 16
        // every note needs 8 bytes (note on, note off) + 1 byte for first note off
        else bytes = 16 + (8*n+1)*factorial(NLIMIT); // for n>8 multiple files of length 8!
    }
    printf("Bytes: %d\n",bytes+64);
    return bytes;
}

int write_noteon(char note,FILE *fpt,int ch_mel) {
    if((int)note<0) {
        printf("Invalid noteon %x\n",note);
        return -1;
    }
    
    if((ch_mel!=0)&&(ch_mel!=1)) {
        printf("Invalid chords or melodies argument for note on %x!\n",note);
        return -1;
    }
    
    if (ch_mel) {
        char noteon[6] = { 0x00, 0x90, note, 0x48, 0xa5, 0x40 }; //eighth notes
        //0x9260 = 16th notes for melody!
        fwrite(noteon,1,6,fpt);
        return 0;
    }
    
    else {
        char noteon[4] = { 0x00, 0x90, note, 0x48 };
        //printf("\t\t\tNote on %x\n",note);
        fwrite(noteon,1,4,fpt);
        return 0;
    }
    
}

int write_noteoff(char note,FILE *fpt,int isfirst,int ch_mel) {
    if((int)note<0) {
        printf("Invalid noteoff %x\n",note);
        return -1;
    }
    
    if( (isfirst<0) || (isfirst>1)) {
        printf("Invalid value of flag isfirst for note off\n");
        return -1;
    }
    
    if((ch_mel!=0)&&(ch_mel!=1)) {
        printf("Invalid chords or melodies argument for note off %x!\n",note);
        return -1;
    }
    if (ch_mel) {
        char noteoff[3] = { 0x90, note, 0x00 };
        //half delta time for melody! 0x96/2 = 0x4b
        //printf("\t\t\tNote off (first) %x\n",note);
        fwrite(noteoff,1,3,fpt);
        return 0;
    }
   
    if((isfirst)) { //&&(ch_mel)
        char noteoff[5] = { 0xcb, 0x00, 0x90, note, 0x00 };
        //printf("\t\t\tNote off (first) %x\n",note);
        fwrite(noteoff,1,5,fpt);
        return 0;
    }

    else {
        char noteoff[4] = { 0x00, 0x90, note, 0x00 };
        //printf("\t\t\tNote off %x\n",note);
        fwrite(noteoff,1,4,fpt);
        return 0;
    }
    
    
}

void write_header(FILE *fpt,int bytes, int ch_mel) {
    if(fpt==NULL) {
        printf("NULL file pointer\n");
        return;
    }
    
    if((ch_mel!=0)&&(ch_mel!=1)) {
        printf("Invalid chords or melodies argument!\n");
        return;
    }
        
    char b1 = bytes % 256;
    char b4 = bytes/16777216;
    char b3 = (bytes - b4*16777216)/65536;
    char b2 = (bytes - b4*16777216 - b3*65536)/256;

    //printf("b4 0x%x b3 0x%x b2 0x%x b1 0x%x\n",b4,b3,b2,b1);
    char hexheader[76] = {
    0x4d, 0x54, 0x68, 0x64, 0x00, 0x00, 0x00, 0x06, 0x00, // 0-8
    0x01, 0x00, 0x02, 0x25, 0x80, // 1=Multitrack format, 2=2 Tracks //9-13
    //001001011000000|0 = 4800 | ticks per quarter note
    // first track is always to set tempo! notes are contained in 2nd
    0x4d, 0x54, 0x72, 0x6b, // standard header "MTrk" // 14-17
    0x00, 0x00, 0x00, 0x22, 0x00, 0xff, // (0x22) 34 bytes to follow till ff2f // 18-23
    0x54, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // SMPTE OFFSET 0 // 24-31
    0xff, 0x51, 0x03, 0x0f, 0x42, 0x40, 0x00, 0xff, //0f4240=1000000us pro quarter // 32-39 
    0x58, 0x04, 0x04, 0x02, 0x18, 0x08, // time signature 4/4 // 40-45
    0x00, 0xff, 0x59, 0x02, 0x00, 0x00, // key signature c major // 46-51
    0x00, 0xff, 0x2f, 0x00, // end of chunk (34 bytes) // 52-55
    0x4d, 0x54, 0x72, 0x6b, // standard header "MTrk" // 56-59
    b4, b3, b2, b1, // #bytes to follow // 60-63
    0x00, 0xff, 0x03, 0x06, // MIDI TRACK NAME MESSAGE ff03, 06 bytes "Chords" // 64-67
    //0x43, 0x68, 0x6f, 0x72, 0x64, 0x73, //Track name "Chords" in hex
    'C', 'h', 'o', 'r', 'd', 's', // 68-73
    //'M', 'I', 'D', 'I' //for track name "MIDI"
    }; // MIDI HEADER

 // FOR "MELODIES"
    if(ch_mel) {
        //hexheader[35]=0x03; // double
        //hexheader[36]=0xd0; // tempo
        //hexheader[37]=0x90; // for melody
        hexheader[67]=8; // 8 bytes for "Melodies"
        hexheader[68]='M';
        hexheader[69]='e';
        hexheader[70]='l';
        hexheader[71]='o';
        hexheader[72]='d';
        hexheader[73]='i';
        hexheader[74]='e';
        hexheader[75]='s';
        fwrite(hexheader,76,1,fpt);
    }
    else
        fwrite(hexheader,74,1,fpt);
    //fwrite(hexheader,72,1,fpt); // this is for "MIDI"
    //http://www.music.mcgill.ca/~ich/classes/mumt306/StandardMIDIfileformat.html#BM1_3
}

void write_outro(FILE *fpt, int N) {
    char outro[6] = { 0x82, 0xac, 0x00, 0xff, 0x2f, 0x00  }; // end of track
    
    if(fpt==NULL) {
        printf("NULL file pointer\n");
        return;
    }
    if(N==1) { // dotted half for 1 note/chord
        outro[0] = 0x81;
        outro[1] = 0xc3;
        outro[2] = 0x00;
    }
    /*
    else if(((!ch_mel)&&(N<4)) || (N==3) ) { //2! ,3! chords: half note at the end
        outro[0] = 0x81;
        outro[1] = 0x96;
        outro[2] = 0x00;
    }
    */
    else ;

    fwrite(outro,6,1,fpt);
}

int write_midi_ch(char *path,char *textfile,int N) {
    if((N<0)||(N>12)) {
        printf("Invalid number of notes for writing midi (%d)!\n",N);
        return -1;
    }
    
    if(strlen(textfile)==0) {
        printf("Empty filename for text file!\n");
        return -1;
    }
    char textfilewithpath[strlen(textfile)+MAX_PATH];
    sprintf(textfilewithpath,"%s/%s",path,textfile);
    FILE *fp = fopen(textfilewithpath,"r");
    char fwrname[strlen(textfile)+MAX_PATH];
    sprintf(fwrname,"%s_ch.mid",textfilewithpath);
    printf("midi file name: %s\n",fwrname);
    FILE *fpr = fopen(fwrname,"wb+");
    if (fpr) setvbuf(fpr, NULL, _IOFBF, 1 << 20);
    
    if( (fp==NULL) || (fpr==NULL)) {
        perror("Cannot open a file for midi write");
        return 0;
    }
    
    int bytes = determine_bytes(N,0); //printf("bytes are %d or 0x%x\n",bytes,bytes);
    write_header(fpr,bytes,0);
    
    char readbuffer[MAX_LINE_LENGTH], notebuffer[N][4]; //maximum line length 48
    int i=0,j,k,c;
    char *ret;
    char note;
    //char chordendnull[1] = "\0";
    
    while ((ret = fgets(readbuffer,MAX_LINE_LENGTH,fp))) {
        //printf("\nLine %d\n%s",i+1,readbuffer);
        
        //read at most MAX_LINE_LENGTH characters till new line
        if (ret==NULL) break; // end of file
        c=10;
        for(j=0;j<N;j++) { //printf("--Note no%d\n",j+1);
            while(readbuffer[c]==' '&& c<MAX_LINE_LENGTH) c++; //skip spaces
            //printf("\tc:%d\n",c);
            strncpy(notebuffer[j],readbuffer+c,2); // copy note
            notebuffer[j][2]='\0'; //printf("notebuffer %s\n",notebuffer[j]);
            if (j==N-1) { //printf("last note of line\n"); // for last note of line
                k=0;
                while(k<strlen(notebuffer[j])) { //printf("\t\tk: %d\n",k);
                    if(notebuffer[j][k]=='\n') notebuffer[j][k]='\0';
                    k++;
                } // replace newline char with null char for the last note!
                
                if(strlen(notebuffer[j])<2) { // for natural notes (1 letter)
                    notebuffer[j][k-1]=' '; // add space
                    notebuffer[j][k]='\0'; // stop
                }
            }
            
            c+=strlen(notebuffer[j]); // go to next note
            //printf("Note: %s.\n", notebuffer[j]);
        }
        
        c = determine_octrange(notebuffer,N); //printf("Octrange %d\n",c);
        for (j=0;j<N;j++) { // NOTE ON MESSAGES
            
            
            if(j==0) note = determine_note(notebuffer[j],c); //getchar();
            // find hex value of first note
                
            else {
                note += determine_difference(notebuffer[j],notebuffer[j-1]);
                // add each interval on top
                if(note>127) { // for octave ranges 11/12, high notes might not fit
                    printf("File %s->Line %d->note no %d->"
                            "Note %c too high, probably too big a range. "
                            "Dropping it an oct\n",textfile,i+1,j+1,note);
                    note -= 12;
                }
                
            }
            write_noteon(note,fpr,0);
            //printf("%d %s %d\n",j,notebuffer[j],note);
        }
        
        for (j=0;j<N;j++) { // NOTE OFF MESSAGES
            
            if(j==0) {
                note = determine_note(notebuffer[j],c);
                write_noteoff(note,fpr,1,0);//first note contains message
            }
            else {
                note += determine_difference(notebuffer[j],notebuffer[j-1]);
                if(note>127) {
                    //printf("File %s->Line %d->note no %d->"
                          //  "Note %c too high, probably too big a range. "
                          //  "Dropping it an oct\n",textfile,i+1,j+1,note);
                    note -= 12;
                }
                write_noteoff(note,fpr,0,0);
            }
        }
        //fwrite(chordendnull,1,1,fpr);
        
        i++; //getchar();
    }
    
    fclose(fp);
    
    write_outro(fpr,N);
    fclose(fpr);
    
    return 0;
    
}

int write_midi_mel(char *path,char *textfile,int N) {
    if((N<0)||(N>12)) {
        printf("Invalid number of notes for writing midi (%d)!\n",N);
        return -1;
    }
    
    if(strlen(textfile)==0) {
        printf("Empty filename for text file!\n");
        return -1;
    }
    char textfilewithpath[strlen(textfile)+MAX_PATH];
    sprintf(textfilewithpath,"%s/%s",path,textfile);
    FILE *fp = fopen(textfilewithpath,"r");
    char fwrname[strlen(textfile)+MAX_PATH];
    sprintf(fwrname,"%s_mel.mid",textfilewithpath);
    printf("midi file name: %s\n",fwrname);
    FILE *fpr = fopen(fwrname,"wb+");
    if (fpr) setvbuf(fpr, NULL, _IOFBF, 1 << 20);
    
    if( (fp==NULL) || (fpr==NULL)) {
        perror("Cannot open a file for midi write");
        return 0;
    }
    
    int bytes = determine_bytes(N,1); //printf("bytes are %d or 0x%x\n",bytes,bytes);
    write_header(fpr,bytes,1);
    
    char readbuffer[MAX_LINE_LENGTH], notebuffer[N][4]; //maximum line length 48
    int i=0,j,k,c;
    char *ret;
    char note;
    //char chordendnull[1] = "\0";
    
    while ((ret = fgets(readbuffer,MAX_LINE_LENGTH,fp))) {
        //printf("\nLine %d\n%s",i+1,readbuffer);
        
        //read at most MAX_LINE_LENGTH characters till new line
        if (ret==NULL) break; // end of file
        c=10;
        for(j=0;j<N;j++) { //printf("--Note no%d\n",j+1);
            while(readbuffer[c]==' ' && c<MAX_LINE_LENGTH) c++; //skip spaces
            //printf("\tc:%d\n",c);
            strncpy(notebuffer[j],readbuffer+c,2); // copy note
            notebuffer[j][2]='\0'; //printf("notebuffer %s\n",notebuffer[j]);
            if (j==N-1) { //printf("last note of line\n"); // for last note of line
                k=0;
                while(k<strlen(notebuffer[j])) { //printf("\t\tk: %d\n",k);
                    if(notebuffer[j][k]=='\n') notebuffer[j][k]='\0';
                    k++;
                } // replace newline char with null char for the last note!
                
                if(strlen(notebuffer[j])<2) { // for natural notes (1 letter)
                    notebuffer[j][k-1]=' '; // add space
                    notebuffer[j][k]='\0'; // stop
                }
            }
            
            c+=strlen(notebuffer[j]); // go to next note
            //printf("Note: %s.\n", notebuffer[j]);
        }
        
        for (j=0;j<N;j++) { // NOTE ON AND OFF MESSAGES
            note = determine_note(notebuffer[j],1);
            // octave range always 1 for melodies
            
            write_noteon(note,fpr,1);
            write_noteoff(note,fpr,1,1);
            //printf("%d %s %d\n",j,notebuffer[j],note);
        }
        
        i++; //getchar();
    }
    
    fclose(fp);
    
    write_outro(fpr,N);
    fclose(fpr);
    
    return 0;
    
}
