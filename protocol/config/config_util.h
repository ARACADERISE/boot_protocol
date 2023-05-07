#ifndef config_util
#define config_util
#include "../YamlParser/parser.h"
#include "format.h"

/* Starting sector to start reading in sectors from. */
#define sector_after_mbr		(uint8)0x02
#define MBR_size				(uint16)bytes_per_sector

/* Disk image for the protocol. */
static uint8 *disk_image = NULL;

/* FAMPS local bin folder. */
#define FAMP_bin_folder			(uint8 *)"boot_protocol/bin"

/* Disk image folder for FAMP, for UNIX(support for other operating systems will be coming soon). */
#define FAMP_disk_image_folder	(uint8 *)"/usr/lib/FAMP_disk_images/"

#define config_assert(cond, err_msg, is_file, file, ...)  	\
    if(!(cond)) {                                   		\
        fprintf(stderr, err_msg, ##__VA_ARGS__);    		\
		if(is_file == true && !(file == NULL)) fclose(file);\
        exit(EXIT_FAILURE);                         		\
    }

#include "format_disk_image.h"

/* Status over checking portions of the disk image. */
enum disk_image_check_status
{
	needs_rework,
	good,
	bad_memory_stamp,
	good_memory_stamp,
	bad_chunk, 			// this should(hopefully) never be used. however, if it is, odds are the user tweaked with something
	unknown_error,		// this should(hopefully) never be used. however, if it is, odds are the user tweaked with something
	chunk_corrupted,	// this should(hopefully) never be used. however, if it is, odds are the user(or a program) manipulated the binary data in the file
	fixed,
	unstated
};

/* Information over the current portion of the disk image. */
typedef struct disk_image_check_data
{
	size_t							bytes_checked;

	/* Status of the check.
	 * `needs_reword` - tells the FAMP configuration program to refill the given chunk with the correct data
	 * `good` - nothing wrong with it
	 * `bad_memory_stamp` - something went wrong when writing the memory stamp, this will be a tough fix
	 */
	enum disk_image_check_status	status[2]; 		// we can end up with `needs_rework` and `bad_memory_stamp`
	uint8							total;			// How many "status reports" did we get? 0, 1, or 2?

	uint8							*image;
	uint8							*disk_image_filename;
	FILE							*disk_image; 	// TODO: change to disk_image, and change all references to "device image" accordingly
	bool							check_mem_stamp;
	bool							reopen_file_when_done;

	/* How many bytes were wrong? How many were able to be fixed? */
	uint16							bad_bytes;
	uint16							corrected_bytes;

	/* Where did we begin checking the chunk of data? */
	size_t							begin_pos;

	/* Memory Stamp. */
	_memory_stamp					*chunks_memory_stamp;
} _disk_image_check_data;
static _disk_image_check_data dicd[5];
static uint8 ind = 0;

// Dumb little function to shorten the amount of code
uint8 *initiate_path(uint8 data1[], uint8 data2[])
{
	/* `data1`, at least, has to be valid and not NULL. */
	config_assert(data1, "Cannot initiate the path. No data given to configure the path.\n", false, NULL)

	/* Initiate `array` and concat `data1` to it. */
	uint8 *array = calloc(strlen(data1), sizeof(*array));
	strcat(array, data1);

	/* If `data2` is not NULL, go ahead and perform the same action as above. */
	if(data2 != NULL)
	{
		array = realloc(array, ((strlen(data1) + 1) + strlen(data2)) * sizeof(*array));
		
		config_assert(strlen(data1) + strlen(data2) <= 80, 
			"Path is too large: %s.\nFAMP only allows up to 50 characters for a path.\n", false, NULL, strcat(array, data2))
		
		strcat(array, data2);
	}

	return array;
}

FILE *open_and_assert(const uint8 *filename, uint8 perm[2])
{
	config_assert(strcmp(perm, "wb") == 0 || strcmp(perm, "w") == 0 || strcmp(perm, "r") == 0 || strcmp(perm, "rb") == 0,
		"The permission for opening the file %s must be `wb` or `rb`.\n", false, NULL, filename)
	
	FILE *f = fopen(filename, perm);
	config_assert(f,
		strcmp(perm, "rb") == 0 ? "Error opening file %s.\n" : "Error creating file %s.\n", true, f, filename)
	
	return f;
}

/*
 * get_file_size - helper function that will return the size of a file(in bytes)
 *
 * Input: FILE *f(file of which we want to get the size of)
 * Output: size_t(the size of the file)
 * On Error: This function will error if both arguments are NULL, or if there was an error with opening up a file, and will exit with `EXIT_FAILURE`
 *
 * */
size_t get_file_size(FILE *f, uint8 *filename)
{
	if(f == NULL)
	{
		config_assert(filename, "Cannot finish `get_file_size`: No valid arguments given.\n", false, NULL)
		
		/* Open the file, according to `filename`, and check if it's valid. */
		f = open_and_assert(filename, "rb");
	}

	fseek(f, 0, SEEK_END);
	size_t fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	return fsize;
}

void write_file(const uint8 *filename, uint8 *buffer, size_t buffer_size)
{
	config_assert(buffer != NULL,
		"There was no buffer to write to the file %s.\n", false, NULL, filename)
	
	FILE *temp_file = open_and_assert(filename, "w");

	size_t bytes_written = fwrite(buffer, sizeof(*buffer), buffer_size, temp_file);
	config_assert(bytes_written == buffer_size,
		"Not all of the buffer was written to the file %s.\n", true, temp_file, filename)

	fclose(temp_file);
}

uint8 *read_format(const uint8 *filename, uint8 access[2])
{
	FILE* format_ = fopen(filename, access);
	uint8 *raw_format = NULL;

	config_assert(format_, "Error openeing ../boot/boot_format.\n\tWas it deleted?\n", false, NULL)

	size_t format_size = get_file_size(format_, NULL);

	config_assert(format_size > 0, "Error with size of ../boot/bformat2.\n\tWas all the content removed?\n", false, NULL)

	raw_format = calloc(format_size, sizeof(*format_));
	fread(raw_format, format_size, sizeof(*raw_format), format_);
	fclose(format_);

	return raw_format;
}

void write_MBR(_yaml_os_data *odata, uint16 first_sector, uint16 ssb_size, size_t fs_size, uint8 *base_path, uint8 *OS_name, uint8 FS_type, uint8 *OS_version, uint8 OS_type, size_t kernel_size)
{
	uint8 *fo = NULL;
	if(base_path != NULL) fo = read_format((const uint8 *)initiate_path(base_path, "boot_protocol/config/formats/boot_format"), "rb");
	else fo = read_format((const uint8 *)"formats/boot_format", "rb");
	uint8 format2[bytes_per_sector * 9];

	if(base_path != NULL)
	{
		sprintf(format2, fo,
			FS_type,
			OS_name,
			OS_version,
			OS_type,
			first_sector,
			first_sector + (ssb_size / 512),
			ssb_size / 512,
			first_sector + (ssb_size / 512),
			first_sector + ((ssb_size / 512) + (kernel_size / 512)),
			kernel_size / 512,
			first_sector + ((ssb_size / 512) + (kernel_size / 512)),
			first_sector + ((ssb_size / 512) + (kernel_size / 512)) + (fs_size / 512),
			fs_size / 512);

			write_file(initiate_path(base_path, "boot_protocol/boot/boot.s"), format2, strlen(format2));
			free(fo);
			return;
	}

	if(strlen(odata->OS_name) < 15)
	{
		
		uint8 pad[15 - strlen(odata->OS_name)];
		memset(pad, ' ', 15 - strlen(odata->OS_name));
		strcat(odata->OS_name, pad);
	}
	
	sprintf(format2, fo,
			odata->FS_type,
			odata->OS_name,
			odata->OS_version,
			odata->type,
			first_sector,
			first_sector + (ssb_size / 512),
			ssb_size / 512,
			first_sector + (ssb_size / 512),
			first_sector + ((ssb_size / 512) + (odata->kern_filename_bin_size / 512)),
			odata->kern_filename_bin_size/ 512,
			first_sector + ((ssb_size / 512) + (odata->kern_filename_bin_size / 512)),
			first_sector + ((ssb_size / 512) + (odata->kern_filename_bin_size / 512)) + (fs_size / 512),
			fs_size / 512);
	
	write_file("../boot/boot.s", format2, strlen(format2));

	free(fo);
}

/*
 * strdel - function to remove portions of text from a pointer
 * 
 * Input: uint8 *text(the text of which is getting modified)
 *		  uint32 deleteStartPosition(where to start deleting)
 *		  uint32 deleteLength(how many character to delete)
 * Output: uint8 *(the modified version of `uint8 *text`)
 * On Error: This function will prompt that the end position surpasses the length and exit with `EXIT_FAILURE`
 *
 * */
uint8* strdel(uint8* text, uint32 deleteStartPosition, uint32 deleteLength) {
	// Get end position
	uint32 deleteEndPosition = deleteStartPosition + deleteLength;

	config_assert(deleteEndPosition > strlen(text), "End position surpasses length of text.\n", false, NULL)

	// Copy the remaining text to the start of the deleted fragment
	// text + deleteStartPosition = destination
	// text + deleteEndPosition = src
	strcpy(text + deleteStartPosition, text + deleteEndPosition);

	// Return the modified text
	return text;
}

/* 
 * reallocate_disk_image_size - function that reallocates the size of `disk_image`
 * 
 * Input: size_t pos(the current position in `disk_image`, also refers to the overall size of `disk_image`)
 *		  size_t new_size(how many bytes do we need to allocate?)
 * Ouput: size_t(the amount of bytes allocated)
 * On Error: This function will prompt that an unexpected error has occurred and will exit with `EXIT_FAILURE`
 *
 * */
size_t reallocate_disk_image_size(size_t pos, size_t new_size)
{
	/* This should never happen, but lets plan for the unexpected. */
	config_assert(disk_image, "An unexpected error occurred: the disk image has no memory.\n", false, NULL)

	/* Allocate new memory. */
	disk_image = realloc(disk_image, ((pos + 1) + new_size) * sizeof(*disk_image));
	memset(&disk_image[pos], 0, new_size);

	return new_size;
}

/*
 * fill_disk_image - function that will fill out `disk_image` 512 byte chunks at a time(it fills out in multiples of 512 bytes to make sure the files are padded correctly)
 *
 * Input: FILE *binary_data(the file from which we obtain the binary data)
 *		  size_t buffer_size(how many bytes are we reading from the file?)
 *		  size_t start_index(where at in `disk_image` are we placing the binary data?)
 *		  size_t max_size(what is the overall size(capacity) of `disk_image`? how many bytes, in total, can we assign)
 * Output: size_t(End position of where we're located at in `disk_image` after filling it out with the according data)
 * On Error: This function can error on three different occasions; upon an error happening, the function will prompt an error message and exit with `EXIT_FAILURE`
 *
 * */
size_t fill_disk_image(FILE *binary_data, size_t buffer_size, size_t start_index, size_t max_size)
{
	config_assert(disk_image, 
		"Something went wrong while filling out the disk image.\nCannot fill disk image: no memory allocated.\n", false, NULL)

	config_assert(binary_data,
		"The binary file passed to `fill_disk_image` must be closed, or does not exist.\n", false, NULL)

	/* Make sure `start_index` plus `data_length` does not surpass the overall size of `disk_image`. */
	/* If `start_index` plus `data_length` is larger than `max_size`, go ahead and allocate new memory so everything will fit.
	 * TODO: Should we keep this to make things more convinient, or should we go ahead and error?
	 *		Obviously if the `disk_image` array cannot store all the data than there was a internal problem with the code.
	 *		We can go ahead and keep this to have some sort of "patch", but with this: we won't ever know of any bugs existing.
	 * */
	if(start_index + buffer_size > max_size)
		config_assert(reallocate_disk_image_size(max_size, (start_index + buffer_size) - max_size) == (start_index + buffer_size) - max_size, 
			"Failed to allocate needed memory for `disk_image.\n", false, NULL)

	//config_assert(start_index + buffer_size <= max_size, "There is not enough memory allocated for the disk image.\n", false, NULL)

	size_t bytes_read = fread(&disk_image[start_index], sizeof(uint8), buffer_size, binary_data);
	config_assert(bytes_read >= 1, 
		"Error reading in binary data: nothing got read or the file is corrupted.\n", false, NULL)

	return start_index + buffer_size;
}

enum disk_image_check_status approve(_disk_image_check_data *dicd);
_disk_image_check_data *rework_chunk(_disk_image_check_data *dicd, uint8 *binary_file_content);

/* Total amount of tries being made. Once this variable accumulates up to 3, the configuration will error. */
static uint8 tries = 0;

/*
 * check_disk_chunk - check a portion(chunk) of the disk image against the individual binary files that the overall disk image consists of
 *
 * Input: uint8 *dimg_buffer(the `disk_image` array)
 *		  FILE *bin_file(binary file of the isolated binary data for each portion of the overall disk image)
 *		  size_t bytes(the size of the chunk we are checking)
 * Output: _disk_image_check_data(stores some useful information over the chunk of data)
 * On Error: This function does not error
 *
 * */
_disk_image_check_data check_disk_chunk(uint8 *dimg_buffer, FILE* bin_file, uint8 *filename, size_t bytes, size_t pos, bool check_mem_stamp, bool open_file_when_done)
{
	config_assert(filename != NULL,
		"An argument passed to `check_disk_chunk` resulted in being NULL. This is an invalid argument.\n", true, bin_file)

	if(!(bin_file)) bin_file = fopen(filename, "rb");
	
	/* All data over the current check. */
	_disk_image_check_data temp_dicd = {
		.bytes_checked = 0, 
		.status = {unstated, unstated}, 
		.total = 0, 
		.image = dimg_buffer,
		.disk_image_filename = filename,
		.disk_image = bin_file,
		.check_mem_stamp = check_mem_stamp,
		.reopen_file_when_done = open_file_when_done,
		.bad_bytes = 0,
		.corrected_bytes = 0,
		.begin_pos = pos,
		.chunks_memory_stamp = calloc(1, sizeof(_memory_stamp))
	};

	/* Initiate a temporary buffer, then read. */
	uint8 temp_buffer[bytes];
	memset(temp_buffer, 0, bytes);

	config_assert(get_file_size(bin_file, NULL) == bytes,
		"There was an error checking portions of the given binary file.\n", true, bin_file)
	
	fread(temp_buffer, bytes, sizeof(uint8), bin_file);

	/* Get the memory stamp. */
	if(check_mem_stamp)
	{
		fseek(bin_file, bytes - memory_stamp_size, SEEK_SET);
		fread(temp_dicd.chunks_memory_stamp, 1, memory_stamp_size, bin_file);
		fseek(bin_file, 0, SEEK_SET);
	}

	/* Make sure the file is closed just in case another program does manipulate its data. 
	 * We will be able to tell if the chunk is corrupted efficiently then.
	 */
	fclose(bin_file);
	temp_buffer[0] = 'A';

	/* Perform the check. */
	size_t i = 0;
	while(i < bytes)
	{
		if(!(dimg_buffer[pos + i] == temp_buffer[i]))
		{
			temp_dicd.status[temp_dicd.total] = needs_rework;
			temp_dicd.total++;

			if(temp_dicd.total >= 2) goto rework;
			temp_dicd.bad_bytes++;
		}

		temp_dicd.bytes_checked++;
		i++;
	}

	if(temp_dicd.total != 0)
	{
		rework:
		enum disk_image_check_status stat = approve(rework_chunk(&temp_dicd, temp_buffer));
		config_assert(stat != bad_chunk && stat != unknown_error,
			"A unknown error occurred, or there is a bad chunk residing in the disk image.\nThe error occurrs with the binary file %s.\n", false, NULL, temp_dicd.disk_image_filename)
		
		/* If `temp_dicd.status[1]` is `chunk_corrupted` then there is a process that continues to manipulate the given binary file, or the user themselves are
		 * manipulating the binary file.
		 */
		config_assert(temp_dicd.status[1] != chunk_corrupted,
			"The binary file is corrupted. There must be some sort of application or process that continues to change the data residing within the needed binary file %s.\n", false, NULL, temp_dicd.disk_image_filename)
		
		/* Check what status we got from `rework_chunk`. If it is not `unstated` then `approve` returned `fixed`. 
		 * If it is not `fixed`, error. Something went wrong.
		 */
		config_assert(temp_dicd.status[0] == fixed,
			"There are some underlying problems with a specific chunk(located ~%ld bytes into the disk image).\nThe error occurrs with the binary file %s.\n", false, NULL, pos, temp_dicd.disk_image_filename)
	}

	if(tries > 0) tries = 0;
	else {
		temp_dicd.status[0] = good;
		temp_dicd.status[1] = unstated;
	}

	/* Reopen the file so the `main.c` program can still use it. */
	if(open_file_when_done)
		bin_file = open_and_assert(filename, "rb");

	return temp_dicd;
}

enum disk_image_check_status approve(_disk_image_check_data *dicd)
{
	enum disk_image_check_status ret_status = unstated;

	/* If, for some unknown reason, `dicd.image` is NULL return `unknown_error` for there should be no reason that would happen. */
	if(dicd->image == NULL) { ret_status = unknown_error; goto end; }

	recheck:
	config_assert(tries < 4,
		"Attempted to fix the disk image 3 times while checking the chunk of data resembling the binary file %s. Aborting.\n", false, NULL, dicd->disk_image_filename)
	
	/* If we corrected less bytes than were found, and we are on our last try, we will just go ahead and return `bad_chunk`.
	 * If a chunk gets tested up to 3 times and FAMP still has no luck with fixing it, there is no point trying again.
	 */
	if(dicd->corrected_bytes < dicd->bad_bytes && tries + 1 == 4) { ret_status = bad_chunk; goto end; }

	if(dicd->corrected_bytes == dicd->bad_bytes)
	{
		dicd->status[0] = fixed;
		ret_status = fixed;
		goto end;
	}

	/* Lets make sure that, upon checking the chunk from the disk, we check the same amount of bytes.
	 * If `prev_bytes_checkes` does not match with `dicd->bytes_checked` the function will return `bad_chunk`.
	 * We also want to know if `check_disk_chunk` found more bad bytes.. or less. If it found more we will continue to
	 * rework the chunk until three tries have accumulated, then we will just throw an error and exit.
	 */
	retry_check:
	size_t prev_bytes_checked = dicd->bytes_checked;
	uint16 prev_bad_bytes = dicd->bad_bytes;
	enum disk_image_check_status prev_status[2] = {dicd->status[0], dicd->status[1]};
	*dicd = check_disk_chunk(dicd->image, dicd->disk_image, dicd->disk_image_filename, dicd->bytes_checked, dicd->begin_pos, dicd->check_mem_stamp, dicd->reopen_file_when_done);

	/* This check occurrs just in case some sort of program manipulates the binary file and its data while this program is running.
	 * If this happens, this check will tell us the sizes of the chunks do not match, and we will return `bad_chunk`.
	 */
	if(dicd->bytes_checked < prev_bytes_checked) 
	{
		/* Reread the binary data.
		 * If `check_disk_chunk` gives us data that does not match what we previously had, lets retry.
		 * Accumulate the total tries, read in the binary data again and reset the data.
		 */
		if(tries < 4) { tries++; goto retry_check; }

		/* If `tries` is greater than 3, then the chunk is simply corrupted and there is something wrong with it.
		 * Or there is a background process running(or a application running) that continues to change the binaries size.
		 */
		dicd->status[0] = bad_chunk;
		dicd->status[1] = chunk_corrupted;
		ret_status = bad_chunk; 
		goto end; 
	}

	if((dicd->status[0] == prev_status[0] || dicd->status[1] == prev_status[1]) ||
	   (dicd->status[0] == prev_status[1] || dicd->status[1] == prev_status[0]) ||
	   (dicd->bad_bytes >= prev_bad_bytes)) { dicd = rework_chunk(dicd, dicd->image); goto recheck; }

	/* If `check_disk_chunk` returns `good` then just return `good`. */
	if(dicd->status[0] == good || dicd->status[1] == good)
	{
		tries = 0;
		
		/* The chunk is now `good`, however it had to get fixed before it was considered good.
		 * Make sure the status resembles the chunk got fixed.
		 */
		dicd->status[0] = fixed;
		dicd->status[1] = unstated;

		ret_status = fixed;
	}

	end:
	tries = 0;
	return ret_status;
}

_disk_image_check_data *rework_chunk(_disk_image_check_data *dicd, uint8 *binary_file_content)
{
	size_t i = 0;
	while(i < dicd->bytes_checked)
	{
		if(dicd->image[i + dicd->begin_pos] != binary_file_content[i])
		{
			memset(&dicd->image[i + dicd->begin_pos], binary_file_content[i], 1);
			dicd->corrected_bytes++;
		}
		i++;
	}

	/* If for some reason we didn't correct all the bad bytes above, refill the chunk of data in `disk_image` with what data resides in the actual binary file(`dicd->disk_image`). */
	if(dicd->corrected_bytes < dicd->bad_bytes)
	{
		/* Make sure the binary file is opened. */
		if(!(dicd->disk_image)) dicd->disk_image = fopen(dicd->disk_image_filename, "rb");

		/* `dicd->disk_image` is the file, `disk_image`, the variable, is the actual array representing the disk image. */
		size_t bytes_filled = fill_disk_image(dicd->disk_image, dicd->bytes_checked, dicd->begin_pos, strlen(dicd->image));

		config_assert(bytes_filled == dicd->bytes_checked,
			"There was a random mismatch of chunk sizes when fixing a chunk of data from the disk image.\nThis error occurrs for the binary file %s.\n", false, NULL, dicd->disk_image_filename)
	}
	
	/* We really don't know the status until `approve` does some more checking. Let it be `unstated`. */
	dicd->status[0] = unstated;
	dicd->status[0] = unstated;
	dicd->total = 0;

	tries++;
	return dicd;
}

enum disk_image_check_status is_memory_stamp_good(_disk_image_check_data dicd, uint8 expected_memory_id)
{
	if(dicd.chunks_memory_stamp->memory_id == expected_memory_id && dicd.chunks_memory_stamp->estimate_size_in_bytes == dicd.bytes_checked) return good_memory_stamp;
	return bad_memory_stamp;
}

#endif