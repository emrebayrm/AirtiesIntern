//
// Created by patates on 7/2/18.
//


#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../myether.h"

#define MAXNAME 10
#define ETHERHERNETHEADER 14
#define MAX_IFNAMSIZ 32
#define RX_BUF_SIZE	1500

typedef struct {
    char ethernet_header[ETHERHERNETHEADER];
    char name[MAXNAME];
    char surname[MAXNAME];
    char filename[32];
    uint32_t file_size;
    uint16_t fragment_count;
    uint16_t fragment_index;
    uint32_t fragment_size;
    uint32_t data_crc;
    char data[0];
}__attribute__((packed)) capture_format_t;

typedef struct{
    int *sizes;
    char **fragments;
    int total_bytes;
    int ether_type;
    int frag_count;
    int sfd;
    int counter;
}many_capture_t;


void write_to_file(many_capture_t *datas, int size);

void capture_file(char *ifname, char *ether_types);

void crc32(const void *data, size_t n_bytes, uint32_t* crc);

int missing_index(int *indexes,int size);

static void usage()
{
    fprintf(stderr, "\nUsage:\n./rx_raw <ifname> <ether_types>\n");
    fprintf(stderr, "Example:\n./rx_raw eth0 0x1234,0x4321\n");
}


int main(int argc , char * argv[]){
    char ifname[MAX_IFNAMSIZ] = {0};
    int ret;
    //struct timespec sleep_time;
    char *arg_ifname;

    if (argc != 3) {
        usage();
        goto bail;
    }
    /*
    int *arr;
    arr = calloc(argc - 1, sizeof(int));
    for (int i = 2; argv[i] != NULL; ++i) {
        arr[i] = strtol(argv[i],NULL,16);
    }
    */
    arg_ifname = argv[1];

    snprintf(ifname, MAX_IFNAMSIZ, "%s", arg_ifname);

    if (!net_device_up(ifname)) {
        fprintf(stderr, "%s is not up\n", ifname);
        goto bail;
    }


    capture_file(ifname, argv[2]);
    return 0;
    bail:
    return -1;
}

void capture_file(char *ifname, char *ether_types) {
    int ret = 0;
    many_capture_t many_capture[10];
    int sfd_size = 0;
    capture_format_t *hdr;
    char *buffer;
    int done = 0;
    int frag_count;
    int totalbytes = 0;

    uint32_t crc_chk;
    char *tok;

    tok = strtok(ether_types,",");
    while(tok != NULL){
        many_capture[sfd_size].ether_type = strtol(tok,NULL,16);
        fprintf(stderr," -- %s , %d \n",tok, many_capture[sfd_size].ether_type);
        tok = strtok(NULL,",");
        ++sfd_size;
    }

    for (int i = 0; i < sfd_size; ++i) {
        many_capture[i].sfd = net_create_raw_socket(ifname,many_capture[i].ether_type, 0);
        if (many_capture[i].sfd == -1) {
            fprintf(stderr, "failed to init socket\n");
            return;
        }
    }


    fprintf(stderr,"All Sockets initialized : %d \n" ,sfd_size);
    buffer = malloc(RX_BUF_SIZE + sizeof(capture_format_t));
    hdr = (capture_format_t *) buffer;
    int k;
    for (k = 0; k < sfd_size || ret < 0; ++k) {
        ret = recv(many_capture[k].sfd, hdr, RX_BUF_SIZE  + sizeof(capture_format_t), 0);
        nanosleep((const struct timespec[]){{0, 500000L}}, NULL);
        if(ret > 0)
            break;
        if(k+1 == sfd_size)
            k = 0;
    }
    fprintf(stderr,"First Packet has arrived \n");
    if (ret < 0) {
        fprintf(stderr, "ERROR: recv failed ret: %d, errno: %d\n", ret, errno);

    }
    fprintf(stderr, "%d bytes received\n", ret);
    fprintf(stderr,"fragment infos : fsize %d , name %s , surname %s , fragment size %d .\n",
            hdr->file_size, hdr->name,hdr->surname,hdr->fragment_size);
    //init dataframes
    for (int l = 0; l < sfd_size; ++l) {
        frag_count = many_capture[l].frag_count = hdr->fragment_count;

        many_capture[l].fragments = calloc(frag_count, sizeof(char*));

        many_capture[l].sizes = calloc(frag_count, sizeof(int));
        many_capture[l].counter = 0;
    }
    many_capture[k].fragments[hdr->fragment_index - 1] = hdr->data;
    many_capture[k].sizes[hdr->fragment_index - 1] = hdr->fragment_size;
    many_capture[k].total_bytes += hdr->fragment_size;
    many_capture[k].counter++;
    int old_k;
    old_k = k;
    while (!done) {

        buffer = calloc(RX_BUF_SIZE  + sizeof(capture_format_t),sizeof(char));
//        perror(".. ");
        hdr = (capture_format_t *) buffer;
        ret = -1;
        while(ret < 0) {
            for (k = 0; k < sfd_size; ++k) {
                if (old_k != k)
                    ret = recv(many_capture[k].sfd, hdr, RX_BUF_SIZE + sizeof(capture_format_t), 0);
                if (ret > 0)
                    break;
            }
        }
        old_k = k;

        fprintf(stderr, "%d bytes received\n", ret);
        if (ret < 0) {
            fprintf(stderr, "ERROR: recv failed ret: %d, errno: %d\n", ret, errno);
            free(buffer);
            continue;
        }

        if(hdr->fragment_size > RX_BUF_SIZE){
            fprintf(stderr,"MAX BUFFER SIZE reached. ");
            fprintf(stderr,"Dropping... \n");
            free(buffer);
            continue;
        }
        /*
        if (ret != hdr->fragment_size + sizeof(capture_format_t)){
            fprintf(stderr,"Error recv ret value %d - %d \n",ret ,hdr->fragment_size + sizeof(capture_format_t));
            free(buffer);
            continue;
        }
         */
        if(hdr->fragment_index <= 0 || hdr->fragment_index > hdr->fragment_count){
            fprintf(stderr,"Error , reading fragment index : %d \n",hdr->fragment_index);
            continue;
        }
        crc_chk = 0;
        crc32(hdr->data,hdr->fragment_size,&crc_chk);
        if(crc_chk != hdr->data_crc){
            fprintf(stderr,"CRC ERROR. %d %d \n",crc_chk,hdr->data_crc);
            free(buffer);
            continue;
        }
        //TODO
        for (int i = 0; i < sfd_size; ++i) {
            done = 1;
            if (many_capture[i].counter < many_capture[i].frag_count ){
                done=0;
                break;
            }
        }
        if(many_capture[k].frag_count != hdr->fragment_count){
            many_capture[k].frag_count = hdr->fragment_count;
            many_capture[k].sizes = realloc(many_capture[k].sizes,many_capture[k].frag_count);
            many_capture[k].fragments = realloc(many_capture[k].fragments,many_capture[k].frag_count);
        }
        if(many_capture[k].sizes[hdr->fragment_index - 1] != 0){
            fprintf(stderr,"Same data ! discarding... \n");
            for (int i = 0; i < sfd_size; ++i) {
                fprintf(stderr,"counter : %d , frag count : %d \n",many_capture[i].counter, many_capture[i].frag_count);
                fprintf(stderr,"Missing index %d \n",missing_index(many_capture[i].sizes,many_capture[i].frag_count));
            }
            fprintf(stderr," index %d \n",hdr->fragment_index - 1);

            //free(buffer); //SEGFAULT
            continue;
        }

        many_capture[k].fragments[hdr->fragment_index -1] = hdr->data;
        many_capture[k].sizes[hdr->fragment_index -1] = hdr->fragment_size;
        many_capture[k].total_bytes += hdr->fragment_size;
        totalbytes += hdr->fragment_size;
        fprintf(stderr,"New frag fragment infos : fsize %d , name %s ,surname %s : %d/%d , fragment index %d .\n",
                hdr->file_size, hdr->name,hdr->surname,many_capture[k].counter,hdr->fragment_count,hdr->fragment_index);
        many_capture[k].counter++;
    }
/*
    int totl;
    for (int j = 0; j < sfd_size; ++j) {
        totl += many_capture[j].total_bytes;
    }
    if(totl != totalbytes){
        fprintf(stderr, "ERROR: file size error received: %d total bytes %d \n", totl, totalbytes);
        return;
    }
*/
    write_to_file(many_capture, sfd_size);
    printf("Completed (%s) \n","dowloaded.file");
}

uint32_t crc32_for_byte(uint32_t r)
{
    int j;
    for (j = 0; j < 8; ++j)
        r = (r & 1 ? 0 : (uint32_t)0xEDB88320L) ^ r >> 1;
    return r ^ (uint32_t)0xFF000000L;
}

void crc32(const void *data, size_t n_bytes, uint32_t* crc)
{
    size_t i;
    static uint32_t table[0x100];
    if (!*table)
        for (i = 0; i < 0x100; ++i)
            table[i] = crc32_for_byte(i);
    for (i = 0; i < n_bytes; ++i)
        *crc = table[(uint8_t)*crc ^ ((uint8_t*)data)[i]] ^ *crc >> 8;
}

void write_to_file(many_capture_t *datas, int size) {
    FILE* outp;
    outp = fopen("downloaded.file","w");
    for (int j = 0; j < size; ++j) {
        for (int i = 0; i < datas[j].frag_count; ++i) {
            fwrite(datas[j].fragments[i],datas[j].sizes[i],1,outp);
            free(datas[j].fragments[i]- sizeof(capture_format_t));
        }
        free(datas[j].sizes);
        free(datas[j].fragments);
    }
    fclose(outp);
}


int missing_index(int *indexes,int size){
    for (int i = 0; i < size; ++i) {
        if(indexes[i] == 0)
            return i;
    }
    return -1;
}