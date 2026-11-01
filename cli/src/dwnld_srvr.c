#include "config.h"

/*
#include <miniz/miniz.h>
*/
// Callback function to write incoming data chunks directly to a file
size_t write_to_file(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
  return fwrite(ptr, size, nmemb, stream);
}

int extrct_bds(const char *inst_path, const char *output, const char *vers)
{
  printf("Disclaimer! This part uses unzip and system(); because I cannot figure out how to do it otherwise. This makes it exclusive to Debuntu Distros\n");

  if (system("unzip > /dev/null") != 0)
  {
    printf("Unzip not installed! Installing now...\n");
    if (system("sudo apt install zip unzip -y") != 0)
    {
      printf("Failed to install unzip\n");
      return -1;
    }
  }

  char dwnld[512];
  snprintf(dwnld, sizeof(dwnld), "wget %s/%s.zip -O %s/server.zip", BEDROCK_REPOS, vers, inst_path);
  if (system(dwnld) != 0)
  {
    printf("Failed to download BDS\n");
    return -1;
  }

  char extrct[512];
  snprintf(extrct, sizeof(extrct), "unzip -d %s/server.zip %s/", inst_path, inst_path);
  if (system(extrct) != 0)
  {
    printf("Failed to extract BDS zip\n");
    return -1;
  }

  return 0;
}

/*
int extrct_bds(const char *inst_path,const char *output)
{
  mz_zip_archive zip;
  memset(&zip, 0, sizeof(zip));

  // Initialize the zip reader
  if (!mz_zip_reader_init_file(&zip, output, 0))
  {
    fprintf(stderr, "Failed to open zip file: %s\n", output);
    return 1;
  }

  int file_count = (int)mz_zip_reader_get_num_files(&zip);
  for (int i = 0; i < file_count; i++)
  {
    mz_zip_archive_file_stat file_stat;
    if (!mz_zip_reader_file_stat(&zip, i, &file_stat))
    {
      continue;
    }

    // Build the target output file path
    char full_path[512];
    snprintf(full_path, sizeof(full_path), "%s/%s", inst_path, file_stat.m_filename);

    // If it's a directory entry, create it or let extract handle paths
    if (mz_zip_reader_is_file_a_directory(&zip, i))
    {
      // miniz handles directory creation in newer versions or you can skip
      continue;
    }

    // Extract individual file to destination path
    if (!mz_zip_reader_extract_to_file(&zip, i, full_path, 0))
    {
      fprintf(stderr, "Failed to extract file: %s\n", file_stat.m_filename);
      mz_zip_reader_end(&zip);
      return 1;
    }
  }

  mz_zip_reader_end(&zip);
  printf("Bedrock zip successfully extracted!\n");
  return 0;
}

*/

int dwnld_srvr(const char *inst_path, const char *vers, const char *edition)
{
  char output[512];
  char url[512];
  if (strcmp(edition, "java") == 0 || strcmp(edition, "Java") == 0)
  {
    snprintf(output, sizeof(output), "%s/server_%s.jar", inst_path, vers);
    snprintf(url, sizeof(url), "%s/server_%s.jar", JAVA_REPOS, vers);
  }

  if (strcmp(edition, "bedrock") == 0 || strcmp(edition, "Bedrock") == 0)
  {
    snprintf(output, sizeof(output), "%s/server.zip", inst_path);
    snprintf(url, sizeof(url), "%s/%s.zip", BEDROCK_REPOS, vers);
  }

  CURL *curl = curl_easy_init();
  if (!curl)
  {
    fprintf(stderr, "Failed to initialize cURL\n");
    return 1;
  }
    
  FILE *fp = fopen(output, "wb");
  if (!fp)
  {
    perror("Failed to open output file");
    curl_easy_cleanup(curl);
    return 1;
  }

  // Configure cURL options
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_file);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // Automatically follow HTTP redirects (crucial for Mojang/paper download links)
  curl_easy_setopt(curl, CURLOPT_USERAGENT, "DaemonCraft/1.0");

  // Perform the download
  CURLcode res = curl_easy_perform(curl);
    
  // Cleanup
  fclose(fp);
  curl_easy_cleanup(curl);

  if (res != CURLE_OK)
  {
    fprintf(stderr, "Download failed: %s\n", curl_easy_strerror(res));
    return -1;
  }

  if (strcmp(edition, "bedrock") == 0 || strcmp(edition, "Bedrock") == 0)
  {
    if (extrct_bds(inst_path, output, vers) != 0)
    {
      printf("Failed to download BDS zip\n");
      return -1;
    }
    else printf("Downloaded BDS sucessfully!\n");
  }

  if (strcmp(edition, "java") == 0 || strcmp(edition, "Java") == 0) printf("Successfully downloaded JDS to %s\n", output);

  return 0;
}