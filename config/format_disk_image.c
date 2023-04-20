#include "format_disk_image.h"

/* format_disk_image.c - Program that formats a disk image with a FS. */

int main(int args, char *argv[])
{
    if(args < 4)
    {
        fprintf(stderr, "Expected at least three arguments: [disk_image_output] [type_of_filesystem] [size_of_filesystem]\n\tWhere `size_of_filesystem` is `2G`, `3G` or `4G`(depending on type of OS you are running. `2G`/`3G` for 32-bit, any for 64-bit).\n");
        fprintf(stderr, "For more information, run `FAMP_fdi --h`. FAMP tools should be installed if you followed documentation and ran `make tools`. If you did not do this, it is adviced to go ahead and do it. If you have already ran your build simply run `make clean`, enter the directory\n`boot_protocol` and run `make tools`.\n\n");
        exit(EXIT_FAILURE);
    }

    /* The first argument should be `$PWD`. Get the entire path to the output binary file. */
    uint8 *path = calloc(strlen(argv[1]), sizeof(*path));
    strcat(path, argv[1]);

    /* To the user, the second argument to the overall program is the first they give to the script.
     * We will concat the second argument(being the output binary file) to the overall `path`, which should just, as of right now, store
     * the CWD(Current Working Directory).
     */
    path = realloc(path, ((strlen(argv[1]) + 1) + strlen(argv[2])) * sizeof(*path));
    strcat(path, argv[2]);

    /* First argument should be the ouput file the user wants the disk image(file system) to dwell in.*/
    disk_image = fopen(path, "wb");
    free(path);

    FAMP_assert(disk_image, "Error opening up the output binary file `%s`.\n", argv[1]);

    fclose(disk_image);

    return 0;
}