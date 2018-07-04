//
// Created by patates on 7/2/18.
//

#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <stdlib.h>
#include "myether.h"

#define MAXNAME 10
#define ETHERHERNETHEADER 14
#define MAX_IFNAMSIZ 32
#define ETHER_TYPE   ((u_int16_t) 0x1234)
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

typedef struct Node{
    capture_format_t base;
    char **fragments;
    int total_bytes;
    int *sizes;
    struct Node *next;
}files_t;

void write_to_file(many_capture_t *datas, int size);

void capture_file(char *ifname, char *erher_types);

void crc32(const void *data, size_t n_bytes, uint32_t* crc);

int check_if_have(files_t *head,capture_format_t *hdr);

static void usage()
{
    fprintf(stderr, "\nUsage:\n./rx_raw <ifname> <ether_type>\n");
    fprintf(stderr, "Example:\n./rx_raw eth0 0x1234\n");
}

int main(int argc , char *argv[]){
    char ifname[MAX_IFNAMSIZ] = {0};
    int ret;
    //struct timespec sleep_time;
    char *arg_ifname;

    if (argc < 2 || argc > 3) {
        usage();
        goto bail;
    }

    arg_ifname = argv[1];

    snprintf(ifname, MAX_IFNAMSIZ, "%s", arg_ifname);

    if (!net_device_up(ifname)) {
        fprintf(stderr, "%s is not up\n", ifname);
        goto bail;
    }

    capture_file(ifname, NULL);
    return 0;
    bail:
    return -1;
}

void capture_file(char *ifname, char *erher_types) {
    int sfd,ret;
    capture_format_t *hdr;
    char *buffer;
    uint32_t crc_chk;
    files_t *head;

    sfd = net_create_raw_socket(ifname, ETHER_TYPE, 0);
    if (sfd == -1) {
        fprintf(stderr, "failed to init socket\n");
        return;
    }
    head = calloc(1, sizeof(files_t));
    head->next = NULL;

    while(1){
        buffer = malloc(RX_BUF_SIZE);
        hdr = (capture_format_t *) buffer;

        ret = recv(sfd, hdr, RX_BUF_SIZE, 0);
        if(check_if_have(head,hdr) == 0) {

        }
    }

}

int check_if_have(files_t *head,capture_format_t *hdr){

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
    outp = fopen(fname,"w");
    for (int i = 0; i < fragsize; ++i) {
        fwrite(fragments[i],sizes[i],1,outp);
        free(fragments[i]- sizeof(capture_format_t));
    }
    free(sizes);
    free(fragments);
    fclose(outp);
}
