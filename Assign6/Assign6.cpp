/* 
 * File:   Assign6.cpp
 * Author: Talik Kasozi
 * 
 * Assignment #6 working w/ struct stat and lstat (see linux man page)
 *
 * Created on December 5, 2014, 12:52 PM
 */

#include <cstdlib>
#include <iostream>
#include <dirent.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h> //lstat
#include <pwd.h>
#include <grp.h>
#include <cstdlib>

using namespace std;

enum case_enum {
    ONE = 0, TWO = 1
};

void printCMD(case_enum, char*, struct stat, char*);

//prints command

void printCMD(case_enum case_, char * file_name, struct stat stat_buf, char* path = NULL) {

    char perms[10];
    struct passwd* pwd;
    struct group* group;

    perms[9] = '\0';

    for (int i = 0; i < 9; i++) {
        perms[i] = '-';
    }


    if (case_ == TWO) {
        if (lstat(path, &stat_buf) == -1) {
            cout << "lstat failed. " << endl;

            exit(2);
        }
    }
    cout << "File: " << file_name << endl;

    cout<<"Type: ";
    switch (stat_buf.st_mode & S_IFMT) {
        case S_IFBLK: cout << "block device" << endl;
            break;
        case S_IFCHR: cout << "character device" << endl;
            break;
        case S_IFDIR: cout << "directory" << endl;
            break;
        case S_IFIFO: cout << "FIFO/pipe" << endl;
            break;
        case S_IFLNK: cout << "symlink" << endl;
            break;
        case S_IFREG: cout << "regular file" << endl;
            break;
        case S_IFSOCK: cout << "socket" << endl;
            break;
        default: cout << "unknown?" << endl;
            break;
    }

    //change permission if permission exists
    if (stat_buf.st_mode & S_IRUSR) {
        perms[0] = 'r';
    }

    if (stat_buf.st_mode & S_IWUSR) {
        perms[1] = 'w';
    }

    if (stat_buf.st_mode & S_IXUSR) {
        perms[2] = 'x';
    }

    if (stat_buf.st_mode & S_IRGRP) {
        perms[3] = 'r';
    }

    if (stat_buf.st_mode & S_IWGRP) {
        perms[4] = 'w';
    }

    if (stat_buf.st_mode & S_IXGRP) {
        perms[5] = 'w';
    }

    if (stat_buf.st_mode & S_IWOTH) {
        perms[6] = 'r';
    }

    if (stat_buf.st_mode & S_IROTH) {
        perms[7] = 'w';
    }
    if (stat_buf.st_mode & S_IXOTH) {
        perms[8] = 'x';
    }

    cout<< "Permissions: "<< perms <<endl;

    // Owner info 
    if ((pwd = getpwuid(stat_buf.st_uid)) == NULL) {
       cout<<"Owner: "<< stat_buf.st_uid<<endl;
    } else {
        cout<<"Owner: "<< pwd -> pw_name<<endl;
    }

    // Pgroup
    if ((group = getgrgid(stat_buf.st_gid)) == NULL) {
        cout<<"Group: "<< stat_buf.st_gid<<endl;
    } else {
        cout<<"Group: "<< group -> gr_name<<endl;
    }

    // Date info 
    cout<<"Last Modified: " << ctime(&(stat_buf.st_mtime));
    cout<<"Links: "<< stat_buf.st_nlink <<endl;

    // Device type if applicable 
    if ((S_ISCHR(stat_buf.st_mode)) || S_ISBLK(stat_buf.st_mode)) {
        cout<<"Major:  "<< major(stat_buf.st_rdev) << " Minor "<<minor(stat_buf.st_rdev) <<endl;
    } else {
       cout<< "Size: " << stat_buf.st_size<<endl;
        cout<< "I-Node Number: "<< stat_buf.st_ino <<endl;
        cout<<endl;
    }
    
}

/*
 * Assignment #6 
 *
 */
int main(int argc, char** argv) {


    struct stat buf;

    if (argc < 2) {

        //display info for this directory
        DIR *dir;
        struct dirent *diren;

        if ((dir = opendir(".")) == NULL) {

            cout << "opening directory failed." << endl;

            exit(3);
        }

        while ((diren = readdir(dir)) != NULL) {

            if (lstat(diren->d_name, &buf) == -1) {
                cout << "lstat failed. " << endl;

                exit(2);
            }
            printCMD(ONE, diren->d_name, buf);
            cout<<endl;
        }
    } else {

        for (int i = 1; i < argc; i++) {
            printCMD(TWO, argv[i], buf, argv[i]);
        }
    }
    return 0;
}
