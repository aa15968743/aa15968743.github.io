#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_COMMAND_LENGTH 100

// 执行命令并处理错误
void execute_command(const char *command) {
    int status = system(command);
    if (status!= 0) {
        printf("命令执行失败，请检查命令和参数。\n");
    }
}

// 检查文件或目录是否存在
int file_exists(const char *path) {
    return access(path, F_OK) == 0;
}

int main() {
    char command[MAX_COMMAND_LENGTH];
    while (1) {
        printf("请输入命令（支持cp、rm、tar -czvf、tar -xzvf，输入exit退出）：");
        fgets(command, MAX_COMMAND_LENGTH, stdin);
        command[strcspn(command, "\n")] = '\0';  // 移除换行符

        if (strcmp(command, "exit") == 0) {
            break;
        }

        char *token = strtok(command, " ");
        if (token == NULL) {
            printf("无效的命令，请重新输入。\n");
            continue;
        }

        if (strcmp(token, "cp") == 0) {
            char *source = strtok(NULL, " ");
            char *destination = strtok(NULL, " ");
            if (source == NULL || destination == NULL) {
                printf("cp命令格式错误，应为cp source destination\n");
            } else if (!file_exists(source)) {
                printf("源文件不存在。\n");
            } else {
                char cp_command[MAX_COMMAND_LENGTH];
                snprintf(cp_command, MAX_COMMAND_LENGTH, "cp %s %s", source, destination);
                execute_command(cp_command);
            }
        } else if (strcmp(token, "rm") == 0) {
            char *file_to_delete = strtok(NULL, " ");
            if (file_to_delete == NULL) {
                printf("rm命令格式错误，应为rm file_to_delete\n");
            } else if (!file_exists(file_to_delete)) {
                printf("要删除的文件不存在。\n");
            } else {
                char rm_command[MAX_COMMAND_LENGTH];
                snprintf(rm_command, MAX_COMMAND_LENGTH, "rm %s", file_to_delete);
                execute_command(rm_command);
            }
        } else if (strcmp(token, "tar") == 0) {
            char *option = strtok(NULL, " ");
            if (option == NULL) {
                printf("tar命令格式错误，应为tar -czvf archive.tar.gz file_or_directory 或 tar -xzvf archive.tar.gz -C destination_directory\n");
                continue;
            }
            if (strcmp(option, "-czvf") == 0) {
                char *archive_name = strtok(NULL, " ");
                char *file_or_directory = strtok(NULL, " ");
                if (archive_name == NULL || file_or_directory == NULL) {
                    printf("tar -czvf命令格式错误，应为tar -czvf archive.tar.gz file_or_directory\n");
                } else if (!file_exists(file_or_directory)) {
                    printf("要压缩的文件或目录不存在。\n");
                } else {
                    char tar_compress_command[MAX_COMMAND_LENGTH];
                    snprintf(tar_compress_command, MAX_COMMAND_LENGTH, "tar -czvf %s %s", archive_name, file_or_directory);
                    execute_command(tar_compress_command);
                }
            } else if (strcmp(option, "-xzvf") == 0) {
                char *archive_name = strtok(NULL, " ");
                char *destination = strtok(NULL, " ");
                if (archive_name == NULL || destination == NULL || strcmp(destination, "-C")!= 0) {
                    printf("tar -xzvf命令格式错误，应为tar -xzvf archive.tar.gz -C destination_directory\n");
                } else {
                    destination = strtok(NULL, " ");
                    if (destination == NULL) {
                        printf("tar -xzvf命令格式错误，应为tar -xzvf archive.tar.gz -C destination_directory\n");
                    } else if (!file_exists(archive_name)) {
                        printf("要解压的压缩包不存在。\n");
                    } else {
                        char tar_decompress_command[MAX_COMMAND_LENGTH];
                        snprintf(tar_decompress_command, MAX_COMMAND_LENGTH, "tar -xzvf %s -C %s", archive_name, destination);
                        execute_command(tar_decompress_command);
                    }
                }
            } else {
                printf("不支持的tar选项，请使用-czvf或-xzvf。\n");
            }
        } else {
            printf("无效的命令，请重新输入。\n");
        }
    }
    return 0;
}
