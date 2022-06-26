/*
==========================================
Author: Çağrı Anıl Erbey
Date:   1 May 2021
==========================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct record {
    char name[64]; //utf16
    char surname[32]; //utf8
    char gender;
    char email[32];
    char phone_number[16];
    char address[32];
    char level_of_education[8];
    unsigned int income_level; // given little-endian
    unsigned int expenditure; // given big-endian
    char currency_unit[16];
    char currentMood[32];
    float height;
    unsigned int weight;
};
typedef struct record records;

struct headers {
    char name[64]; //utf16
    char surname[32]; //utf8
    char gender;
    char email[32];
    char phone_number[16];
    char address[32];
    char level_of_education[8];
    unsigned int income_level; // given little-endian
    unsigned int expenditure; // given big-endian
    char currency_unit[16];
    char currentMood[32];
    float height;
    unsigned int weight;
};
typedef struct headers tag;


void createdata(FILE *fp,records rows,int id,int bigEnd, int le_to_bg, records header)
{
    
        fprintf ( fp,"      <row id=\"%d\">\n", id);
        fprintf ( fp,"            <%s>%s</%s>\n",header.name, rows.name, header.name);
        fprintf ( fp,"            <%s>%s</%s>\n",header.surname,rows.surname,header.surname);
        fprintf ( fp,"            <gender>%c</gender>\n",rows.gender);
        fprintf ( fp,"            <%s>%s</%s>\n",header.email,rows.email, header.email);
        fprintf ( fp,"            <%s>%s</%s>\n",header.phone_number,rows.phone_number, header.phone_number);
        fprintf ( fp,"            <%s>%s</%s>\n",header.address,rows.address, header.address);
        if(strcmp(header.level_of_education, "level_of_education")){
            fprintf ( fp,"            <level_of_education>%s</level_of_education>\n",rows.level_of_education);
        }
        fprintf ( fp,"            <income_level bigEnd=\"%ld\">%d</income_level>\n",le_to_bg,rows.income_level);
        fprintf ( fp,"            <expenditure bigEnd=\"%ld\">%d</expenditure>\n",bigEnd,rows.expenditure);
        fprintf ( fp,"            <%s>%s</%s>\n",header.currency_unit,rows.currency_unit,header.currency_unit);
        fprintf ( fp,"            <%s>%s</%s>\n",header.currentMood,rows.currentMood ,header.currentMood);
        fprintf ( fp,"            <height>%.2f</height>\n",rows.height);
        fprintf ( fp,"            <weight>%u</weight>\n",rows.weight);
        fprintf ( fp,"      </row>\n");

}
void string2hexString(char* input, char* output)
{
    int loop;
    int i; 
    
    i=0;
    loop=0;
    
    while(input[loop] != '\0')
    {
        sprintf((char*)(output+i),"%02X", input[loop]);
        loop+=1;
        i+=2;
    }
    //insert NULL at the end of the output string
    output[i++] = '\0';
}
/*void hex2str_UTF16(char hex_str[]){ // to work this funciton use terminal with gcc <inputfile>.c -o <outputfile> -lm(avoid error:"undefined reference to `pow'"")
    int len=strlen(hex_str)/4;
    char new_odd[len+1];
    char new_even[len];
    int sum;
    if(strlen(hex_str) % 2 != 0)
    {
        for (int j = 0; j < len; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                sum += (int)hex_str[j] * pow(16,3-k);
            }
        new_odd[j]=(char)sum;
        sum=0;
        } 

        char kalan;
        for (int i = 0; i < strlen(hex_str)-len*4; i++)
        {
            sum += (int)hex_str[i] * pow(16,3-i);

        }
        kalan = (char)sum;
        sum = 0;
        
        new_odd[len+1]= kalan;
        printf("%s",new_odd);
    }
    else{
        for (int j = 0; j < len; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                sum += (int)hex_str[j] * pow(16,3-k);
            }
        new_even[j]=(char)sum;
        sum=0;
        } 
        printf("%s",new_even);
    }   
}*/

int converter_BE (int a,int b){ //income level-from le to bg         
    long sum=0;

    if(a <= 9){//0-9
        
        sum += a * pow(16,6);
    }
    else{
        sum += (a / 16) * pow(16,7) ;
        sum += (a % 16) * pow(16,6);
    }

    if(b <= 9){//0-9

        sum += (b % 16) * pow(16,4);
    }
    else{
        sum += (b / 16) * pow(16,5) ;
        sum += (b % 16) * pow(16,4);
    }
    return sum;

}

int converter_LE (int a,int b){ //expenditure-from bg to le         
    long sum=0;

    if(a <= 9){//0-9
        
        sum += (a % 16) * pow(16,2);
    }
    else{
        sum += (a / 16) * pow(16,3) ;
        sum += (a % 16) * pow(16,2);
    }

    if(b <= 9){//0-9

        sum += b % 16;
    }
    else{
        sum += (b / 16) * 16 ;
        sum += b % 16 ;
    }
    return sum;

}

int main(){
  
    FILE *fp;

    records rows[51];
    records headers;
    char first_command[15];
    char file_name_read[20];
    char file_name_write[20];
    printf("Please enter command like this -> Bin2XML <inputfile> <outputfile> ");
    scanf("%s",&first_command);
    scanf("%s",&file_name_read);
    scanf("%s",&file_name_write);

    if(strcmp(first_command, "Bin2XML")!=0){
        printf("Operation command is wrong!\n");
        exit(1);
    }
    if(fp == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    if (strcmp(file_name_read, "records.dat")!=0)
    {
        printf("Wrong inputfile name!\n");
        exit(1);
    }
    if (strcmp(file_name_write, "records.xml")!=0)
    {
        printf("Wrong outpufile name!\n");
        exit(1);
    }
    
    fp = fopen(file_name_read, "rb");
    for (int i = 0; i < 51; i++)//read data from .dot file
    {
        
        fread(&rows[i], sizeof(records), 1, fp);
      
    }
    fclose(fp);

    headers = rows[0];  //headers initiliaze 
   


    fp=fopen(file_name_write,"w");
    fprintf ( fp,"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
    fprintf ( fp,"<Records>\n");//root element

    union exp// was used expenditure to convert
    {
        unsigned int e;
        unsigned char ch[4];
    }ex;

    union income// was used income_level to convert
    {
        unsigned int income;
        unsigned char ch[4];
    }in;

    int big_endian=0;
    int little_endian_to_be=0;
    char for_utf16_name[20];
    
    

    for (int i = 1; i < 51; i++)
    {   
        big_endian = rows[i].expenditure;
        ex.e=rows[i].expenditure;
        rows[i].expenditure = converter_LE((int)(ex.ch[2]),(int)(ex.ch[3]));
        little_endian_to_be = rows[i].income_level;
        in.income=rows[i].income_level;
        little_endian_to_be = converter_BE((int)(in.ch[0]),(int)(in.ch[1]));
        createdata(fp,rows[i],i,big_endian,little_endian_to_be, headers);
                  
    }

    for (int i = 1; i < 51; i++)//is used for names to convert hex string and utf-16
    {
        strcpy(for_utf16_name,rows[i].name);
        int len=strlen(for_utf16_name);
        char hex_str[len*2+1];
        string2hexString(for_utf16_name,hex_str);
        //hex2str_UTF16(hex_str);
    }
     
    
    fprintf ( fp,"</Records>\n");
    
    fclose(fp);
    printf("Record was written to <%s>...\n",file_name_write);
    return(0);
}
