#include <string.h>
#include <stdlib.h>

#include <curl/curl.h>
#include <cjson/cJSON.h>

#include "api.h"

struct MemoryStruct {
  char *memory;
  size_t size;
};


size_t get_https_result(char* buffer, size_t size, size_t nmemb, void* userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, realsize + 1);
    if(!ptr) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), buffer, realsize);
    mem->size = realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}


cJSON* get_request(char* api_key, char* url) {
    CURL *curl;
    CURLcode res;
    struct curl_slist *list = NULL;
    struct MemoryStruct chunk;
 
    chunk.memory = malloc(1);   /* will be grown as needed by the realloc above */
    chunk.size = 0;             /* no data at this point */
  

    curl = curl_easy_init();
    char auth_header[46] = "Authorization: Bearer ";
    
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        list = curl_slist_append(list, strcat(auth_header, api_key));
        list = curl_slist_append(list, "Accept:");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, get_https_result);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*) &chunk);
 
    
        res = curl_easy_perform(curl);

        /* always cleanup */
        curl_slist_free_all(list);
        curl_easy_cleanup(curl);
    }
    
    if (res == 0) {
        cJSON* result = cJSON_Parse(chunk.memory);
        cJSON_AddNumberToObject(result, "fail", 0);
        return result;
    } else {
        return cJSON_Parse("{\"fail\": 1}");
    }
}
